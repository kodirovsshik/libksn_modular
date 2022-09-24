/* 
The following code does not work because Windows™

export module libksn.time;

import <ksn/ksn.hpp>;

//btw here Windows.h has also messed everything up with 258 macro redefenition warnings

import <Windows.h>;

#undef min
#undef max

import <numeric>;
import <chrono>;

*/

module;

#define NOMINMAX
#include <Windows.h>

#include <ksn/ksn.hpp>
#include <chrono>
#include <numeric>



export module libksn.time;



_KSN_EXPORT_BEGIN

class time
{

};

class duration
{
}

_KSN_EXPORT_END


#ifdef _WIN32

static thread_local HANDLE ksn_sleep_timer;

/*
void nanosleep(int64_t dt, bool absolute)
{
	//negative absolute time or duration means no sleep
	if (dt < 0) return;

	//At first try to sleep using multimedia timers from winmm
	bool ok = true;

	LARGE_INTEGER li;
	li.QuadPart = absolute ? dt / 100 : -(dt / 100); //negative time interval means relative to current time
		
	if (!ksn_sleep_timer)
		ok = false;

	if (ok) ok = SetWaitableTimer(ksn_sleep_timer, &li, 0, NULL, NULL, FALSE);
	if (ok) ok = WaitForSingleObject(ksn_sleep_timer, INFINITE) != WAIT_FAILED;
	if (ok) return;

	//If sometring went wrong, fall back to a simple Sleep function
	if (absolute)
	{ //turn an absolute interval into a relative one
		dt -= time::now().as_nsec();
		if (dt < 0) return;
	}
	dt /= 1000000; //time can only be measured in milliseconds for this

	static constexpr DWORD dword_max = std::numeric_limits<DWORD>::max();
	if ((uint64_t)dt > dword_max)
		Sleep(dword_max); //infinite sleep
	else
		Sleep((DWORD)dt);
}
*/


struct _lib_sentry //Library constructor and destructor
{
	_lib_sentry()
	{
		ksn_sleep_timer = CreateWaitableTimerA(NULL, TRUE, NULL);
	}
	~_lib_sentry()
	{
		if (ksn_sleep_timer) CloseHandle(ksn_sleep_timer);
	}
} thread_local _sentry;


#else

namespace
{
	void nanosleep(int64_t dt, bool absolute)
	{
		if (dt < 0)
			return;

		if (!absolute)
			dt += ksn::time::now().as_nsec();

		std::this_thread::sleep_for(std::chrono::nanoseconds(dt));
	}
}

#endif


/*
void sleep_for(time dt)
{
	nanosleep(dt.as_nsec(), false);
}
void sleep_until(time point)
{
	nanosleep(point.as_nsec(), true);
}

void busy_sleep_for(time dt)
{
	busy_nanosleep(dt.as_nsec(), false);
}
void busy_sleep_until(time point)
{
	busy_nanosleep(point.as_nsec(), true);
}

void hybrid_sleep_for(time dt)
{
	if (!hybrid_sleep_threshold || dt < hybrid_sleep_threshold)
		busy_sleep_for(dt);
	else
		sleep_for(dt);
}
void hybrid_sleep_until(time point)
{
	if (!hybrid_sleep_threshold || (point - ksn::time::now()) < hybrid_sleep_threshold)
		busy_sleep_until(point);
	else
		sleep_until(point);
}


ksn::time init_hybrid_sleep_threshold(long double tolerance, ksn::time max_wait_time, ksn::time lower_bound, ksn::time upper_bound)
{
	return {};
	//TODO
}
ksn::time get_hybrid_sleep_threshold() noexcept
{
	return hybrid_sleep_threshold;
}
*/
