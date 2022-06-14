
/* The following code does not work because Windows™

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
	int64_t m_nsec;


public:

	constexpr time() noexcept; //0s (since 01.01.1980)
	template<std::integral T>
	constexpr time(T nanoseconds) noexcept;
	template<std::floating_point T>
	constexpr time(T seconds) noexcept;

	constexpr time(const time&) noexcept = default;


	constexpr int64_t as_nsec() const noexcept;
	constexpr int64_t as_usec() const noexcept;
	constexpr int64_t as_msec() const noexcept;
	constexpr int64_t as_sec() const noexcept;
	constexpr float as_float_sec() const noexcept;
	constexpr double as_double_sec() const noexcept;


	constexpr static time from_nsec(int64_t nanoseconds_since_epoch) noexcept;
	constexpr static time from_usec(int64_t microseconds_since_epoch) noexcept;
	constexpr static time from_msec(int64_t milliseconds_since_epoch) noexcept;
	constexpr static time from_sec(int64_t seconds_since_epoch) noexcept;
	constexpr static time from_float_sec(float seconds_since_epoch) noexcept;
	constexpr static time from_double_sec(double seconds_since_epoch) noexcept;


	static time now() noexcept; //Current time


	constexpr friend time operator+(time lhs, time rhs) noexcept;
	constexpr friend time operator-(time lhs, time rhs) noexcept;

	//nanoseconds
	template<std::integral T>
	constexpr operator T() const noexcept;
	//seconds
	template<std::floating_point T>
	constexpr operator T() const noexcept;

	constexpr explicit operator bool() const noexcept;

	constexpr friend bool operator<(time lhs, time rhs) noexcept;
	constexpr friend bool operator>(time lhs, time rhs) noexcept;
	constexpr friend bool operator<=(time lhs, time rhs) noexcept;
	constexpr friend bool operator>=(time lhs, time rhs) noexcept;
	constexpr friend bool operator==(time lhs, time rhs) noexcept;
	constexpr friend bool operator!=(time lhs, time rhs) noexcept;
};

constexpr time operator+(time lhs, time rhs) noexcept;
constexpr time operator-(time lhs, time rhs) noexcept;

constexpr bool operator<(time lhs, time rhs) noexcept;
constexpr bool operator>(time lhs, time rhs) noexcept;
constexpr bool operator<=(time lhs, time rhs) noexcept;
constexpr bool operator>=(time lhs, time rhs) noexcept;
constexpr bool operator==(time lhs, time rhs) noexcept;
constexpr bool operator!=(time lhs, time rhs) noexcept;


class stopwatch
{
private:

	time m_last_time;


public:

	void start() noexcept;
	time restart() noexcept; //returns 0 if not started
	time current() const noexcept; //returns 0 if not started
	time stop() noexcept;

	bool is_started() const noexcept;
};


void sleep_for(time dt);
void sleep_until(time point);

void busy_sleep_for(time dt);
void busy_sleep_until(time point);

void hybrid_sleep_for(time dt);
void hybrid_sleep_until(time point);


ksn::time init_hybrid_sleep_threshold(
	long double max_sleep_time_relative_error = 0.0164,
	ksn::time max_wait_time = ksn::time::from_float_sec(0.1f),
	ksn::time lower_bound = 1e-3,
	ksn::time upper_bound = 2e-2f) noexcept;
ksn::time get_hybrid_sleep_threshold() noexcept;

_KSN_EXPORT_END





_KSN_BEGIN


constexpr time::time() noexcept
	: m_nsec(0)
{
}
template<std::integral T>
constexpr time::time(T ns) noexcept
	: m_nsec(int64_t(ns))
{
}

template<std::floating_point T>
constexpr time::time(T s) noexcept
	: m_nsec(int64_t(s * (T)1e9))
{
}



constexpr int64_t time::as_nsec() const noexcept
{
	return this->m_nsec;
}
constexpr int64_t time::as_usec() const noexcept
{
	return this->m_nsec / 1000;
}
constexpr int64_t time::as_msec() const noexcept
{
	return this->m_nsec / 1000000;
}
constexpr int64_t time::as_sec() const noexcept
{
	return this->m_nsec / 1000000000;
}
constexpr float time::as_float_sec() const noexcept
{
	return this->m_nsec / 1e9f;
}
constexpr double time::as_double_sec() const noexcept
{
	return this->m_nsec / 1e9;
}



constexpr time time::from_nsec(int64_t ns) noexcept
{
	return time(ns);
}
constexpr time time::from_usec(int64_t us) noexcept
{
	return time(us * 1000);
}
constexpr time time::from_msec(int64_t ms) noexcept
{
	return time(ms * 1000000);
}
constexpr time time::from_sec(int64_t s) noexcept
{
	return time(s * 1000000000);
}
constexpr time time::from_float_sec(float seconds_since_epoch) noexcept
{
	return time::from_nsec(int64_t(seconds_since_epoch * 1e9f));
}
constexpr time time::from_double_sec(double seconds_since_epoch) noexcept
{
	return time::from_nsec(int64_t(seconds_since_epoch * 1e9));
}



time time::now() noexcept
{
	using namespace std;
	using namespace chrono;
	return time(duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count());
}



//nanoseconds
template<std::integral T>
constexpr time::operator T() const noexcept
{
	return (T)this->m_nsec;
}
//seconds
template<std::floating_point T>
constexpr time::operator T() const noexcept
{
	return this->m_nsec * (T)1e-9;
}
constexpr time::operator bool() const noexcept
{
	return this->m_nsec != 0;
}



constexpr time operator+(time lhs, time rhs) noexcept
{
	return time(lhs.m_nsec + rhs.m_nsec);
}
constexpr time operator-(time lhs, time rhs) noexcept
{
	return time(lhs.m_nsec - rhs.m_nsec);
}

constexpr bool operator<(time lhs, time rhs) noexcept
{
	return lhs.m_nsec < rhs.m_nsec;
}
constexpr bool operator>(time lhs, time rhs) noexcept
{
	return lhs.m_nsec > rhs.m_nsec;
}
constexpr bool operator<=(time lhs, time rhs) noexcept
{
	return lhs.m_nsec <= rhs.m_nsec;
}
constexpr bool operator>=(time lhs, time rhs) noexcept
{
	return lhs.m_nsec >= rhs.m_nsec;
}
constexpr bool operator==(time lhs, time rhs) noexcept
{
	return lhs.m_nsec == rhs.m_nsec;
}
constexpr bool operator!=(time lhs, time rhs) noexcept
{
	return lhs.m_nsec != rhs.m_nsec;
}





void stopwatch::start() noexcept
{
	this->m_last_time = time::now();
}
time stopwatch::restart() noexcept
{
	if (this->m_last_time)
	{
		time now = time::now();
		time dt = now - this->m_last_time;
		this->m_last_time = now;
		return dt;
	}
	else
	{
		this->m_last_time = time::now();
		return time();
	}
}
time stopwatch::current() const noexcept
{
	if (this->m_last_time)
		return time::now() - this->m_last_time;
	else
		return time();
}
time stopwatch::stop() noexcept
{
	if (this->m_last_time)
	{
		time saved_time = this->m_last_time;
		this->m_last_time = time();
		return time::now() - saved_time;
	}
	else
		return time();
}
bool stopwatch::is_started() const noexcept
{
	return (bool)this->m_last_time;
}





namespace
{
	void busy_nanosleep(int64_t dt, bool absolute)
	{
		if (dt < 0)
			return;

		if (!absolute)
			dt += ksn::time::now().as_nsec();

		while (ksn::time::now().as_nsec() < dt);
	}

	ksn::time hybrid_sleep_threshold;
}



#ifdef _WIN32

namespace
{
	static thread_local HANDLE ksn_sleep_timer;

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
}


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


ksn::time init_hybrid_sleep_threshold(long double tolerance, ksn::time max_wait_time, ksn::time lower_bound, ksn::time upper_bound) noexcept
{
	using fp_t = long double;

	tolerance = std::clamp<fp_t>(tolerance, 0, INFINITY);
	lower_bound = std::clamp<fp_t>(lower_bound, (fp_t)1e-10, INFINITY);
	upper_bound = std::clamp<fp_t>(upper_bound, (fp_t)1e-10, INFINITY);

	auto single_test = [&]() -> fp_t
	{
		ksn::stopwatch sw;

		fp_t low = lower_bound;
		fp_t high = upper_bound;

		//yes i AM doing a binary search on sleep time and i like this idea
		//go cry about it if you disagree
		while (fabs(high - low) > (fp_t)lower_bound)
		{
			fp_t mid = (low + high) / 2;

			auto sleep_time = ksn::time(mid);
			sw.start();
			ksn::sleep_for(sleep_time);
			fp_t dt = sw.stop().as_float_sec();

			fp_t diff = dt / mid - 1;
			if (diff < 0) diff = -diff;
			if (diff > tolerance)
				low = mid + (fp_t)lower_bound;
			else
				high = mid;
		}

		return low;
	};

	const ksn::time max_test_time = (fp_t)upper_bound * std::log2((fp_t)upper_bound / (fp_t)lower_bound);
	if (max_test_time > max_wait_time)
		return hybrid_sleep_threshold = {};

	//max_wait_time = max_wait_time - max_test_time;

	size_t N = 0;
	fp_t total = 0;

	ksn::stopwatch sw;
	sw.start();

	(void)single_test();
	while (N < 100)
	{
		++N;
		if (sw.current() + max_test_time > max_wait_time)
			break;
		total += single_test();
	}

	return hybrid_sleep_threshold = ksn::time(total / N);
}
ksn::time get_hybrid_sleep_threshold() noexcept
{
	return hybrid_sleep_threshold;
}

_KSN_END
