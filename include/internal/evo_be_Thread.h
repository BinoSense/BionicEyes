#pragma once
#ifndef _BEThread_
#define _BEThread_

#ifdef __linux__
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <pthread.h>

#elif defined(_WIN32)
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#endif

#include <pthread.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <future>
#include <chrono>
#include <iostream>

using namespace std;

namespace evo_be
{
	/**< ???????????????? */
	enum ThreadPriorityType
	{
#ifdef __linux__
		enumThreadPriority_Idle = 0,               /**< ?????--??? */
		enumThreadPriority_Lowest = 1,           /**< ?????--?? */
		enumThreadPriority_Below = 2,      /**< ?????--??? */
		enumThreadPriority_Normal = 3,           /**< ?????--???? */
		enumThreadPriority_Above = 4,      /**< ?????--??? */
		enumThreadPriority_Highest = 5,         /**< ?????--?? */
		enumThreadPriority_Critical = 6,  /**< ?????--??? */

#elif defined(_WIN32)
		enumThreadPriority_Idle = THREAD_PRIORITY_IDLE,               /**< ?????--??? */
		enumThreadPriority_Lowest = THREAD_PRIORITY_LOWEST,           /**< ?????--?? */
		enumThreadPriority_Below = THREAD_PRIORITY_BELOW_NORMAL,      /**< ?????--??? */
		enumThreadPriority_Normal = THREAD_PRIORITY_NORMAL,           /**< ?????--???? */
		enumThreadPriority_Above = THREAD_PRIORITY_ABOVE_NORMAL,      /**< ?????--??? */
		enumThreadPriority_Highest = THREAD_PRIORITY_HIGHEST,         /**< ?????--?? */
		enumThreadPriority_Critical = THREAD_PRIORITY_TIME_CRITICAL,  /**< ?????--??? */
#endif
	};

	/**< ?????????????? */
	static int BE_getThreadPriority(std::thread& t)
	{
#ifdef __linux__
		sched_param sch;
		int policy;
		pthread_getschedparam(t.native_handle(), &policy, &sch);
		return sch.sched_priority;
#elif defined(_WIN32)
		return GetThreadPriority(t.native_handle());
#endif
	};

	/**< ?????????????? */
	static bool BE_setThreadPriority(std::thread& t, ThreadPriorityType PriortType)
	{
#ifdef __linux__
		sched_param sch;
		int policy;
		pthread_getschedparam(t.native_handle(), &policy, &sch);
		sch.sched_priority = PriortType;
		if (pthread_setschedparam(t.native_handle(), SCHED_FIFO, &sch))
			return false;
		else return true;
#elif defined(_WIN32)
		//std::cout <<"PriortType:"<< PriortType << std::endl;
		return SetThreadPriority(t.native_handle(), PriortType);
#endif
	};

	/* 搂脮?????搂脮??
	* @brief : std::thread c++14??boost::thread??????搂脮??
	* @from : https://blog.csdn.net/mymodian9612/article/details/52794980
	*/
	class WfirstRWLock
	{
	public:
		WfirstRWLock() = default;
		~WfirstRWLock() = default;
	public:
		void lock_read()
		{
			std::unique_lock<std::mutex> ulk(counter_mutex);
			cond_r.wait(ulk, [=]()->bool {return write_cnt == 0; });
			++read_cnt;
		}
		void lock_write()
		{
			std::unique_lock<std::mutex> ulk(counter_mutex);
			++write_cnt;
			cond_w.wait(ulk, [=]()->bool {return read_cnt == 0 && !inwriteflag; });
			inwriteflag = true;
		}
		void release_read()
		{
			std::unique_lock<std::mutex> ulk(counter_mutex);
			if (--read_cnt == 0 && write_cnt > 0)
			{
				cond_w.notify_one();
			}
		}
		void release_write()
		{
			std::unique_lock<std::mutex> ulk(counter_mutex);
			if (--write_cnt == 0)
			{
				cond_r.notify_all();
			}
			else
			{
				cond_w.notify_one();
			}
			inwriteflag = false;
		}

	private:
		volatile size_t read_cnt{ 0 };
		volatile size_t write_cnt{ 0 };
		volatile bool inwriteflag{ false };
		std::mutex counter_mutex;
		std::condition_variable cond_w;
		std::condition_variable cond_r;
	};

	template <typename _RWLockable>
	class unique_writeguard
	{
	public:
		explicit unique_writeguard(_RWLockable &rw_lockable)
				: rw_lockable_(rw_lockable)
		{
			rw_lockable_.lock_write();
		}
		~unique_writeguard()
		{
			rw_lockable_.release_write();
		}
	private:
		unique_writeguard() = delete;
		unique_writeguard(const unique_writeguard&) = delete;
		unique_writeguard& operator=(const unique_writeguard&) = delete;
	private:
		_RWLockable &rw_lockable_;
	};

	template <typename _RWLockable>
	class unique_readguard
	{
	public:
		explicit unique_readguard(_RWLockable &rw_lockable)
				: rw_lockable_(rw_lockable)
		{
			rw_lockable_.lock_read();
		}
		~unique_readguard()
		{
			rw_lockable_.release_read();
		}
	private:
		unique_readguard() = delete;
		unique_readguard(const unique_readguard&) = delete;
		unique_readguard& operator=(const unique_readguard&) = delete;
	private:
		_RWLockable &rw_lockable_;
	};

}

#endif