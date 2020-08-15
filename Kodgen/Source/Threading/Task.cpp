#include "Kodgen/Threading/Task.h"

using namespace kodgen;

Task::Task(std::function<void()>&& task) noexcept:
	_task{std::forward<std::function<void()>>(task)}
{
}