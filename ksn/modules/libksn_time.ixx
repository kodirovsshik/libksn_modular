
module;

#ifdef _WIN32
#pragma warning(disable : 5105 5106 4005) //<windows.h> doesn't get along with modules
#include <Windows.h>
#endif

export module libksn.time;

import <ksn/ksn.hpp>;
import <concepts>;
import libksn.meta;



_KSN_EXPORT_BEGIN

class time_point;

class duration
{
private:
	int64_t dt;

public:
	constexpr duration() noexcept;

	constexpr duration fininity() noexcept;

	constexpr friend duration operator+(duration) noexcept;
	constexpr friend duration operator-(duration) noexcept;

	constexpr friend duration operator+(duration, duration) noexcept;
	constexpr friend duration operator-(duration, duration) noexcept;

	template<arithmetic T = int64_t>
	constexpr T as_nsec() noexcept;
	template<arithmetic T = float>
	constexpr T as_sec() noexcept;

	template<arithmetic T = int64_t>
	constexpr static duration from_nsec(T) noexcept;
	template<arithmetic T = float>
	constexpr static duration from_sec(T) noexcept;
};



class time_point
{
private:
	uint64_t time_since_epoch;

public:
	//01.01.1970
	constexpr time_point() noexcept;

	constexpr duration as_duration() noexcept;

	constexpr friend duration operator-(time_point, time_point) noexcept;
	constexpr friend time_point operator+(time_point, duration) noexcept;
	constexpr friend time_point operator+(duration, time_point) noexcept;

	friend time_point now() noexcept;
};


time_point now() noexcept;


class stopwatch
{
private:
	time_point start_time;

public:
	void start() noexcept;
	duration stop() noexcept;
	duration current() noexcept;
	duration restart() noexcept;
};

_KSN_EXPORT_END
