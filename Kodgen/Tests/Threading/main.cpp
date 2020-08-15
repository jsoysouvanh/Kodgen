#include <iostream>

#include <Kodgen/Threading/ThreadPool.h>

using namespace kodgen;

int main()
{
	ThreadPool threadPool;

	for (int i = 0; i < 10; i++)
		threadPool.submitTask(Task([]() { std::cout << "This is a task" << std::endl; }));

	return EXIT_SUCCESS;
}