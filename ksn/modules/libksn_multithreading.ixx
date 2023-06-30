
export module libksn.multithreading;

import <ksn/ksn.hpp>;



_KSN_BEGIN

_KSN_DETAIL_BEGIN

template<class LocalStorage, class Callable, class... Params>
void parallel_work(uint64_t start, uint64_t size, Callable&& f, Params&& ...params)
{
	static constexpr bool has_local_storage = !std::is_void_v<LocalStorage>;
	std::conditional_t<has_local_storage, LocalStorage, int> storage{};

	for (uint64_t i = start; i < start + size; ++i)
	{
		if constexpr (has_local_storage)
			f(i, storage, std::forward<Params>(params)...);
		else
			f(i, std::forward<Params>(params)...);
	}
}

_KSN_DETAIL_END

_KSN_END



_KSN_EXPORT_BEGIN

template<class LocalStorageT = void, class Callable, class... Params>
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

	const auto& worker_func = detail::parallel_work<LocalStorageT, Callable&&, Params&&...>;
	for (size_t i = 0; i < n_workers; ++i)
	{
		std::thread th(worker_func,
			current_start, split,
			std::forward<Callable>(f),
			std::forward<Params>(params)...);
		workers[i] = std::move(th);
		current_start += split;
	}
	for (auto& th : workers)
		th.join();

	uint64_t leftover = total_work_size - current_start;
	worker_func(current_start, leftover, std::forward<Callable>(f), std::forward<Params>(params)...);
}

_KSN_EXPORT_END
