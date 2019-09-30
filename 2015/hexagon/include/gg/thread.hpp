/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include "gg/event.hpp"

#if defined GGTHREAD_BUILD
#	define GG_API __declspec(dllexport)
#else
#	define GG_API __declspec(dllimport)
#endif

namespace gg
{
	class IThread;
	class IThreadPool;
	class ITask;
	class ITaskOptions;

	typedef std::shared_ptr<IThread> ThreadPtr;
	typedef std::shared_ptr<IThreadPool> ThreadPoolPtr;
	typedef std::unique_ptr<ITask> TaskPtr;

	class IThread
	{
	public:
		typedef uint16_t State;

		enum Mode
		{
			LOCAL, // run in current thread (blocks)
			REMOTE // creates remote thread
		};

		virtual ~IThread() = default;
		virtual const std::string& getName() const = 0;
		virtual State getState() const = 0;
		virtual void setState(State) = 0;
		virtual void sendEvent(EventPtr) = 0;
		virtual void addTask(TaskPtr&&, State = 0) = 0;
		virtual void finish() = 0; // stops thread
		virtual void finishTasks() = 0; // thread becomes zombie if there is no task to run in current state
		virtual void finishTasksInState(State) = 0;
		virtual bool run(Mode = Mode::REMOTE) = 0;
		virtual bool isAlive() const = 0;
		virtual void join() = 0;

		template<class Task, State state = 0, class... Params>
		void addTask(Params... params)
		{
			TaskPtr task(new Task(std::forward<Params>(params)...));
			addTask(std::move(task), state);
		}
	};

	class IThreadPool
	{
	public:
		virtual ~IThreadPool() = default;
		virtual ThreadPtr createAndAddThread(const std::string& name) = 0;
		virtual ThreadPtr getThread(const std::string& name) const = 0;
		virtual void addThread(ThreadPtr) = 0;
		virtual bool removeThread(const std::string& name) = 0;
		virtual void removeThreads() = 0;
		virtual void sendEvent(EventPtr) = 0;

		ThreadPtr operator[](const std::string& name) const
		{
			return getThread(name);
		}
	};

	class ITask
	{
	public:
		typedef uint16_t ID;

		virtual ~ITask() = default;
		// called once before the first update
		virtual void onStart(ITaskOptions&) {}
		// called for each event the task received right before onUpdate
		virtual void onEvent(ITaskOptions&, EventPtr) = 0;
		// called periodically until the task finishes
		virtual void onUpdate(ITaskOptions&) = 0;
		// called when thread changed state and task got activated/deactivated
		virtual void onStateChange(ITaskOptions&, IThread::State old_state, IThread::State new_state) {}
		// called each time an exception is encountered while processing events/update
		virtual void onError(ITaskOptions&, std::exception&) {}
		// called once after the task has finished
		virtual void onFinish(ITaskOptions&) {}
	};

	class ITaskOptions
	{
	public:
		virtual ~ITaskOptions() = default;
		virtual IThread& getThread() = 0;
		virtual ITask::ID getTaskID() const = 0;
		virtual IThread::State getState() const = 0;
		virtual void setState(IThread::State) = 0;
		virtual void subscribe(IEvent::Type) = 0;
		virtual void subscribe(IEventDefinitionBase&) = 0;
		virtual void unsubscribe(IEvent::Type) = 0;
		virtual void unsubscribe(IEventDefinitionBase&) = 0;
		virtual uint32_t getElapsedMs() const = 0;
		virtual void finish() = 0;
	};

	class IThreadManager
	{
	public:
		virtual ~IThreadManager() = default;
		virtual ThreadPtr createThread(const std::string& name) const = 0;
		virtual ThreadPoolPtr createThreadPool() const = 0;
		virtual ThreadPoolPtr getDefaultThreadPool() = 0;
	};

	extern GG_API IThreadManager& threadmgr;
};
