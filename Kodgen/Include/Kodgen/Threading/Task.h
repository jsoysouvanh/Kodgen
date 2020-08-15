/**
*	Copyright (c) 2020 Julien SOYSOUVANH - All Rights Reserved
*
*	This file is part of the Kodgen library project which is released under the MIT License.
*	See the README.md file for full license details.
*/

#pragma once

#include <functional>	//std::function

namespace kodgen
{
	class Task
	{
		private:
			/** Underlying task to execute. */
			std::function<void()>	_task;

		public:
			Task()								= delete;
			Task(std::function<void()>&& task)	noexcept;
			Task(Task const&)					= default;
			Task(Task&&)						= default;
			~Task()								= default;

			/**
			*	@brief	Check if this task is ready to execute, i.e. it has no dependency or
			*			all its dependencies have finished their execution.
			*	
			*	@return true if this task is ready to execute, else false.
			*/
			inline bool	isReadyToExecute() const noexcept;

			/**
			*	@brief Execute the underlying task.
			*/
			inline void execute() const noexcept;

			Task& operator=(Task&&)	= default;
	};

	#include "Kodgen/Threading/Task.inl"
}