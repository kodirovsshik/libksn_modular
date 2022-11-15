
export module libksn.multithreading;

import <ksn/ksn.hpp>;

import <thread>;
import <utility>;
import <vector>;

import <cstdint>;
import <cstddef>;



_KSN_BEGIN

_KSN_DETAIL_BEGIN

template<class Callable, class... Params>
void parallel_work(uint64_t start, uint64_t size, Callable&& f, Params&& ...params)
{
	for (uint64_t i = start; i < start + size; ++i)
		f(i, std::forward<Params>(params)...);
}

_KSN_DETAIL_END

_KSN_END



_KSN_EXPORT_BEGIN

template<class Callable, class... Params>
void run_parallel_work(uint64_t first, uint64_t last, Callable&& f, size_t max_threads = -1, Params&& ...params)
{
	if (max_threads == 0)
		max_threads = -1;

	const size_t n_workers = std::min<size_t>(std::thread::hardware_concurrency(), max_threads);

	uint64_t total_work_size = last - first;
	if (total_work_size != UINT64_MAX)
		++total_work_size;
	const uint64_t split = total_work_size / n_workers;

	uint64_t current_start = first;
	std::vector<std::thread> workers(n_workers);
	for (size_t i = 0; i < n_workers; ++i)
	{
		std::thread th(detail::parallel_work<Callable&&, Params&&...>,
			current_start, split,
			std::forward<Callable>(f),
			std::forward<Params>(params)...);
		workers[i] = std::move(th);
		current_start += split;
	}
	for (auto& th : workers)
		th.join();

	uint64_t leftover = total_work_size - current_start;
	detail::parallel_work(current_start, leftover, std::forward<Callable>(f), std::forward<Params>(params)...);
}

_KSN_EXPORT_END
