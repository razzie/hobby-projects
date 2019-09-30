/**
 * Copyright (c) 2014-2015 Gábor Görzsöny (www.gorzsony.com)
 *
 * This source is a private work and can be used only with the
 * written permission of the author. Do not redistribute it!
 * All rights reserved.
 */

#pragma once

#include <atomic>
#include <mutex>

namespace gg
{
	class ReadWriteMutex
	{
	public:
		ReadWriteMutex() :
			m_read_count(0)
		{
		}

		~ReadWriteMutex() = default;

		void readLock()
		{
			unsigned read_count = m_read_count.fetch_add(1);
			if (read_count == 0)
				m_lock.lock();
		}

		void readUnlock()
		{
			unsigned read_count = m_read_count.fetch_sub(1);
			if (read_count == 1)
				m_lock.unlock();
		}

		void writeLock()
		{
			m_lock.lock();
		}

		void writeUnlock()
		{
			m_lock.unlock();
		}

	private:
		std::recursive_mutex m_lock;
		std::atomic<unsigned> m_read_count;
	};

	template<class T>
	class ReadMutexGuard
	{
	public:
		ReadMutexGuard(T& mutex) :
			m_mutex(mutex)
		{
			m_mutex.readLock();
		}

		~ReadMutexGuard()
		{
			m_mutex.readUnlock();
		}

	private:
		T& m_mutex;
	};

	template<class T>
	class WriteMutexGuard
	{
	public:
		WriteMutexGuard(T& mutex) :
			m_mutex(mutex)
		{
			m_mutex.writeLock();
		}

		~WriteMutexGuard()
		{
			m_mutex.writeUnlock();
		}

	private:
		T& m_mutex;
	};
};
