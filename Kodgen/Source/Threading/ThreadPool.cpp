#include "Kodgen/Threading/ThreadPool.h"

#include <cassert>
#include <iostream>		//TO DELETE

using namespace kodgen;

ThreadPool::ThreadPool(uint32 threadCount, ETerminationMode	terminationMode) noexcept:
	_destructorCalled{false},
	_workingWorkers{threadCount},
	terminationMode{terminationMode}
{
	assert(threadCount > 0u);

	std::cout << "Start a thread pool with " << threadCount << " workers." << std::endl;
	std::cout << "Working: " << _workingWorkers << std::endl;

	//Preallocate enough space to avoid reallocations
	_workers.reserve(threadCount);

	for (uint32 i = 0u; i < threadCount; i++)
	{
		_workers.emplace_back(std::thread(std::bind(&ThreadPool::workerRoutine, this)));
	}
}

ThreadPool::~ThreadPool() noexcept
{
	_destructorCalled = true;

	joinWorkers();
}

void ThreadPool::workerRoutine() noexcept
{
	std::unique_lock lock(_taskMutex);

	while (shouldKeepRunning())
	{
		if (!_tasks.empty())
		{
			std::optional<Task> task = getTask();
			
			lock.unlock();

			//Execute the task without the lock to allow other workers to grab tasks
			if (task.has_value())
			{
				task->execute();
			}

			lock.lock();
		}

		_workingWorkers.fetch_sub(1u);

		_taskCondition.wait(lock, [this]() { return !_tasks.empty() || _destructorCalled; });

		_workingWorkers.fetch_add(1u);
	}
}

opt::optional<Task>	ThreadPool::getTask() noexcept
{
	opt::optional<Task>	result = opt::nullopt;

	//Iterate over all tasks
	for (decltype(_tasks)::iterator it = _tasks.begin(); it != _tasks.end(); it++)
	{
		//Get the first ready task
		if (it->isReadyToExecute())
		{
			result.emplace(std::move(*it));
			_tasks.erase(it);

			break;
		}
	}

	return result;
}

void ThreadPool::submitTask(Task&& task) noexcept
{
	std::unique_lock lock(_taskMutex);

	_tasks.emplace_back(std::forward<Task>(task));

	lock.unlock();
	_taskCondition.notify_one();
}

void ThreadPool::killWorkers() noexcept
{
	for (std::thread& worker : _workers)
	{
		if (worker.joinable())
		{
			worker.detach();
		}
	}
}

void ThreadPool::joinWorkers() noexcept
{
	if (_destructorCalled)
	{
		//Awake threads so that they can perform necessary tests to exit their routine
		_taskCondition.notify_all();

		//Explicitely join all threads to terminate
		for (std::thread& worker : _workers)
		{
			if (worker.joinable())
			{
				worker.join();
			}
		}
	}
	else
	{
		//If the destructor hasn't been called, just wait for all workers to be idle
		while (_workingWorkers.load() != 0u)
		{
			std::this_thread::yield();
		}
	}
}

bool ThreadPool::shouldKeepRunning() const noexcept
{
	return	!_destructorCalled	|| (terminationMode == ETerminationMode::FinishAll && _tasks.size() != 0u);
}