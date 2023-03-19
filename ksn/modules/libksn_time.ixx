
module;

#include <ksn/ksn.hpp>


export module libksn.time;

import <chrono>;
import <compare>;
import <thread>;




_KSN_EXPORT_BEGIN

class duration
{
private:
	int64_t dt;
	
public:
	constexpr duration() noexcept;

	constexpr static duration infinity() noexcept;

	constexpr friend duration operator+(duration) noexcept;
	constexpr friend duration operator-(duration) noexcept;

	constexpr friend duration operator+(duration, duration) noexcept;
	constexpr friend duration operator-(duration, duration) noexcept;

	constexpr int64_t as_nsec() noexcept;
	constexpr double as_sec() noexcept;

	constexpr static duration from_nsec(int64_t) noexcept;
	constexpr static duration from_sec(double) noexcept;

	friend std::strong_ordering operator<=>(duration, duration) = default;
};



class time_point
{
private:
	uint64_t time_since_epoch;

public:
	constexpr time_point() noexcept;

	constexpr duration as_duration() noexcept;

	constexpr friend duration operator-(time_point, time_point) noexcept;
	constexpr friend time_point operator+(time_point, duration) noexcept;
	constexpr friend time_point operator+(duration, time_point) noexcept;

	static time_point now() noexcept;

	friend std::strong_ordering operator<=>(time_point, time_point) = default;
};



class stopwatch
{
private:
	time_point start_time;

public:
	void start() noexcept;
	duration stop() noexcept;
	duration current() noexcept;
	duration restart() noexcept;

	time_point get_start_time() const noexcept;
	bool running() const noexcept;
};



void sleep_for(duration);
void busy_sleep_for(duration);
void hybrid_sleep_for(duration);



_KSN_EXPORT_END





_KSN_EXPORT_BEGIN

inline constexpr duration::duration() noexcept
	: dt(0)
{
}
inline constexpr duration duration::infinity() noexcept
{
	return duration::from_nsec(INT64_MAX);
}

constexpr int64_t duration::as_nsec() noexcept
{
	return this->dt;
}
constexpr double duration::as_sec() noexcept
{
	return double(this->dt) * 1e-9;
}

constexpr duration duration::from_nsec(int64_t dt) noexcept
{
	duration d;
	d.dt = dt;
	return d;
}
constexpr duration duration::from_sec(double dt) noexcept
{
	return duration::from_nsec(int64_t(dt * 1e9));
}

constexpr duration operator+(duration d) noexcept
{
	return d;
}
constexpr duration operator-(duration d) noexcept
{
	return duration::from_nsec(-d.as_nsec());
}

constexpr duration operator+(duration a, duration b) noexcept
{
	return duration::from_nsec(a.as_nsec() + b.as_nsec());
}
constexpr duration operator-(duration a, duration b) noexcept
{
	return duration::from_nsec(a.as_nsec() - b.as_nsec());
}






inline constexpr time_point::time_point() noexcept
	: time_since_epoch(0)
{
}

constexpr duration time_point::as_duration() noexcept
{
	return *this - time_point();
}

time_point time_point::now() noexcept
{
	time_point t;
	t.time_since_epoch = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
	return t;

}

constexpr duration operator-(time_point a, time_point b) noexcept
{
	return duration::from_nsec(a.time_since_epoch - b.time_since_epoch);
}
constexpr time_point operator+(time_point t, duration dt) noexcept
{
	t.time_since_epoch += dt.as_nsec();
	return t;
}
constexpr time_point operator+(duration dt, time_point t) noexcept
{
	return t + dt;
}




void stopwatch::start() noexcept
{
	this->start_time = time_point::now();
}

duration stopwatch::stop() noexcept
{
	time_point t;
	std::swap(t, this->start_time);
	return time_point::now() - t;
}

duration stopwatch::current() noexcept
{
	return time_point::now() - this->start_time;
}

duration stopwatch::restart() noexcept
{
	auto now = time_point::now();
	auto dt = now - this->start_time;
	this->start_time = now;
	return dt;
}

time_point stopwatch::get_start_time() const noexcept
{
	return this->start_time;
}

bool stopwatch::running() const noexcept
{
	return this->start_time != time_point();
}




void sleep_for(duration dt)
{
	std::this_thread::sleep_for(std::chrono::nanoseconds(dt.as_nsec()));
}

void busy_sleep_for(duration dt)
{
	auto stop = time_point::now() + dt;
	while (time_point::now() != stop);
}

void hybrid_sleep_for(duration dt)
{
	//TODO
	busy_sleep_for(dt);
}

_KSN_END

