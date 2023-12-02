
#ifdef _DEBUG
#define noexcept_on_release
#else
#define noexcept_on_release noexcept
#endif



export module libksn.mdvector;

import <ksn/ksn.hpp>;



_KSN_BEGIN
_KSN_DETAIL_BEGIN


template<size_t level, class base_t>
class index_proxy
{
	base_t* base;
	size_t offset;

	static constexpr size_t N = base_t::dimensions;

	using my_t = index_proxy<level, base_t>;

	template<size_t, class>
	friend class index_proxy;

public:
	static decltype(auto) create_index_proxy(base_t* base, size_t new_offset) noexcept;

	decltype(auto) operator[](size_t) noexcept_on_release;

	void operator&() const = delete;
};
template<class base_t>
class index_proxy<0, base_t>
{
public:
	static auto& create_index_proxy(base_t* base, size_t new_offset) noexcept_on_release;
};

_KSN_DETAIL_END
_KSN_END





_KSN_EXPORT_BEGIN


template<class T, size_t N>
class multivector
{
	static_assert(!std::is_const_v<T>, "");
	static_assert(!std::is_reference_v<T>, "");
	static_assert(!std::is_void_v<T>, "");
	static_assert(N != 0, "");

	using my_t = multivector<T, N>;
	using index_sequence = std::array<size_t, N>;
	using index_sequence_ref = const index_sequence&;
	using Alloc = std::allocator<T>;

	T* m_data;
	index_sequence m_size{};

	void _allocate_total_storage(size_t);
	template<class ...Args>
	void _construct_storage_from_args(Args&& ...args);
	template<class Iter>
	void _construct_storage_from_range(Iter, Iter);
	void _deallocate_storage();
	Alloc _get_allocator();

	//friend class detail::index_proxy<0, my_t>;
	//friend class detail::index_proxy<0, const my_t>;
	template<size_t level, class base_t>
	friend class detail::index_proxy;

	static size_t total(index_sequence_ref);
	void validate_bounds(index_sequence_ref) const noexcept_on_release;
	void validate_index(size_t) const noexcept_on_release;


public:
	using value_type = T;

	using reference = T&;
	using const_reference = const T&;

	using iterator = T*;
	using const_iterator = const T*;
	using reverse_iterator = std::reverse_iterator<T*>;
	using const_reverse_iterator = std::reverse_iterator<const T*>;

	static constexpr size_t dimensions = N;


	template<class ...Args>
	multivector(index_sequence_ref, Args&& ...args);

	multivector() noexcept;
	multivector(const my_t&);
	multivector(my_t&&) noexcept;

	my_t& operator=(const my_t&);
	my_t& operator=(my_t&&) noexcept;

	~multivector() noexcept;


	size_t get_linear_index(index_sequence_ref) const noexcept;

	void swap(my_t& other) noexcept;


	decltype(auto) operator[](size_t) noexcept_on_release;
	decltype(auto) operator[](size_t) const noexcept_on_release;

	reference operator[](index_sequence_ref) noexcept_on_release;
	const_reference operator[](index_sequence_ref) const noexcept_on_release;

	T* data() noexcept;
	const T* data() const noexcept;


	//Expensive, performes full copy
	void resize(index_sequence_ref);
	size_t size() const noexcept;
	index_sequence_ref size_dims() const noexcept;

	void clear();

	iterator begin() noexcept;
	const_iterator begin() const noexcept;
	const_iterator cbegin() const noexcept;

	iterator end() noexcept;
	const_iterator end() const noexcept;
	const_iterator cend() const noexcept;

	reverse_iterator rbegin() noexcept;
	const_reverse_iterator rbegin() const noexcept;
	const_reverse_iterator crbegin() const noexcept;

	reverse_iterator rend() noexcept;
	const_reverse_iterator rend() const noexcept;
	const_reverse_iterator crend() const noexcept;
};


_KSN_EXPORT_END





_KSN_BEGIN

template<class T, size_t N>
inline void multivector<T, N>::_allocate_total_storage(size_t n)
{
	auto&& al = this->_get_allocator();
	this->m_data = std::allocator_traits<Alloc>::allocate(al, n);
}
template<class T, size_t N>
template<class ...Args>
inline void multivector<T, N>::_construct_storage_from_args(Args&& ...args)
{
	auto&& al = this->_get_allocator();
	for (size_t i = 0; i < this->size(); ++i)
		std::allocator_traits<Alloc>::construct(al, this->m_data + i, std::forward<Args>(args)...);
}

template<class T, size_t N>
inline void multivector<T, N>::_deallocate_storage()
{
	auto&& al = this->_get_allocator();
	for (size_t i = 0; i < this->size(); ++i)
		std::allocator_traits<Alloc>::destroy(al, this->m_data + i);
	std::allocator_traits<Alloc>::deallocate(al, this->m_data, this->size());
	this->m_size = {};
	this->m_data = nullptr;
}
template<class T, size_t N>
inline multivector<T, N>::Alloc multivector<T, N>::_get_allocator()
{
	return Alloc();
}
template<class T, size_t N>
template<class Iter>
inline void multivector<T, N>::_construct_storage_from_range(Iter begin, Iter end)
{
	auto&& al = this->_get_allocator();
	auto ptr = this->m_data;
	while (begin != end)
		std::allocator_traits<Alloc>::construct(al, ptr++, *begin++);
}

template<class T, size_t N>
inline size_t multivector<T, N>::total(index_sequence_ref seq)
{
	size_t val = 1;
	for (size_t i = 0; i < N; ++i)
		val *= seq[i];
	return val;
}

template<class T, size_t N>
inline void multivector<T, N>::validate_bounds(index_sequence_ref seq) const noexcept_on_release
{
#ifdef _DEBUG
	for (size_t i = 0; i < N; ++i)
		if (seq[i] >= this->m_size[i])
			throw std::out_of_range("multivector: index out of range");
#endif
}

template<class T, size_t N>
inline void multivector<T, N>::validate_index(size_t idx) const noexcept_on_release
{
#ifdef _DEBUG
	if (idx >= this->size())
		throw std::out_of_range("multivector: index out of range");
#endif
}


template<class T, size_t N>
template<class ...Args>
inline multivector<T, N>::multivector(index_sequence_ref seq, Args&& ...args)
{
	const size_t total = this->total(seq);
	this->m_size = seq;
	this->_allocate_total_storage(total);
	this->_construct_storage_from_args(std::forward<Args>(args)...);
}
template<class T, size_t N>
inline multivector<T, N>::multivector() noexcept
{
	this->m_data = nullptr;
}
template<class T, size_t N>
inline multivector<T, N>::multivector(const my_t& other)
{
	const size_t total = this->total(other.m_size);
	this->m_size = other.m_size;
	this->_allocate_total_storage(total);
	this->_construct_storage_from_range(other.cbegin(), other.cend());
}
template<class T, size_t N>
inline multivector<T, N>::multivector(my_t&& other) noexcept
{
	this->my_t::multivector();
	this->swap(other);
}
template<class T, size_t N>
inline multivector<T, N>& multivector<T, N>::operator=(const my_t& rhs)
{
	this->~multivector();
	this->my_t::multivector(rhs);
	return *this;
}
template<class T, size_t N>
inline multivector<T, N>& multivector<T, N>::operator=(my_t&& rhs) noexcept
{
	this->swap(rhs);
	return *this;
}
template<class T, size_t N>
inline multivector<T, N>::~multivector() noexcept
{
	this->clear();
}

template<class T, size_t N>
inline size_t multivector<T, N>::get_linear_index(index_sequence_ref seq) const noexcept
{
	size_t idx = seq[0];
	for (size_t i = 1; i < N; ++i)
		idx = idx * this->m_size[i] + seq[i];
	return idx;
}

template<class T, size_t N>
inline void multivector<T, N>::swap(my_t& other) noexcept
{
	std::swap(this->m_data, other.m_data);
	std::swap(this->m_size, other.m_size);
}

template<class T, size_t N>
inline decltype(auto) multivector<T, N>::operator[](size_t idx) noexcept_on_release
{
	return detail::index_proxy<N - 1, my_t>::create_index_proxy(this, idx);
}

template<class T, size_t N>
inline decltype(auto) multivector<T, N>::operator[](size_t idx) const noexcept_on_release
{
	return detail::index_proxy<N - 1, const my_t>::create_index_proxy(this, idx);
}

template<class T, size_t N>
inline T& multivector<T, N>::operator[](index_sequence_ref seq) noexcept_on_release
{
	size_t idx = this->get_linear_index(seq);
	this->validate_index(idx);
	return this->m_data[idx];
}

template<class T, size_t N>
inline const T& multivector<T, N>::operator[](index_sequence_ref seq) const noexcept_on_release
{
	size_t idx = this->get_linear_index(seq);
	this->validate_index(idx);
	return this->m_data[idx];
}

template<class T, size_t N>
inline void multivector<T, N>::clear()
{
	this->_deallocate_storage();
}



template<class T, size_t N>
inline typename multivector<T, N>::iterator multivector<T, N>::begin() noexcept
{
	return this->m_data;
}
template<class T, size_t N>
inline typename multivector<T, N>::const_iterator multivector<T, N>::begin() const noexcept
{
	return this->m_data;
}
template<class T, size_t N>
inline typename multivector<T, N>::const_iterator multivector<T, N>::cbegin() const noexcept
{
	return this->begin();
}

template<class T, size_t N>
inline typename multivector<T, N>::iterator multivector<T, N>::end() noexcept
{
	return this->m_data + this->size();
}
template<class T, size_t N>
inline typename multivector<T, N>::const_iterator multivector<T, N>::end() const noexcept
{
	return this->m_data + this->size();
}
template<class T, size_t N>
inline typename multivector<T, N>::const_iterator multivector<T, N>::cend() const noexcept
{
	return this->end();
}

template<class T, size_t N>
inline typename multivector<T, N>::reverse_iterator multivector<T, N>::rbegin() noexcept
{
	return reverse_iterator(this->begin());
}
template<class T, size_t N>
inline typename multivector<T, N>::const_reverse_iterator multivector<T, N>::rbegin() const noexcept
{
	return const_reverse_iterator(this->begin());
}
template<class T, size_t N>
inline typename multivector<T, N>::const_reverse_iterator multivector<T, N>::crbegin() const noexcept
{
	return this->rbegin();
}

template<class T, size_t N>
inline typename multivector<T, N>::reverse_iterator multivector<T, N>::rend() noexcept
{
	return reverse_iterator(this->end());
}
template<class T, size_t N>
inline typename multivector<T, N>::const_reverse_iterator multivector<T, N>::rend() const noexcept
{
	return const_reverse_iterator(this->end());
}
template<class T, size_t N>
inline typename multivector<T, N>::const_reverse_iterator multivector<T, N>::crend() const noexcept
{
	return this->rend();
}



template<class T, size_t N>
inline T* multivector<T, N>::data() noexcept
{
	return this->m_data;
}

template<class T, size_t N>
inline const T* multivector<T, N>::data() const noexcept
{
	return this->m_data;
}

template<class T, size_t N>
inline void multivector<T, N>::resize(index_sequence_ref seq)
{
	if (this->total(seq) == 0)
		return this->clear();

	my_t other(seq);
	index_sequence max(seq);
	index_sequence current{};

	for (size_t i = 0; i < N; ++i)
		if (max[i] > this->m_size[i])
			max[i] = this->m_size[i];

	size_t iters = total(max);
	while (iters-- > 0)
	{
		other[current] = std::move((*this)[current]);

		size_t updatee_idx = 0;
		while (updatee_idx < N)
		{
			if (++current[updatee_idx] == max[updatee_idx])
			{
				current[updatee_idx] = 0;
				++updatee_idx;
			}
			else
				break;
		}
	}

	this->swap(other);
}

template<class T, size_t N>
inline size_t multivector<T, N>::size() const noexcept
{
	return this->total(this->m_size);
}

template<class T, size_t N>
inline typename multivector<T, N>::index_sequence_ref multivector<T, N>::size_dims() const noexcept
{
	return this->m_size;
}



_KSN_DETAIL_BEGIN

template<size_t level, class base_t>
inline decltype(auto) index_proxy<level, base_t>::create_index_proxy(base_t* base, size_t new_offset) noexcept
{
	index_proxy<level, base_t> result{};
	result.base = base;
	result.offset = new_offset;
	return result;
}
template<size_t level, class base_t>
inline decltype(auto) index_proxy<level, base_t>::operator[](size_t idx) noexcept_on_release
{
	return index_proxy<level - 1, base_t>::create_index_proxy(this->base, idx + this->offset * this->base->m_size[N - level]);
}
template<class base_t>
inline auto& index_proxy<0, base_t>::create_index_proxy(base_t* base, size_t new_offset) noexcept_on_release
{
	base->validate_index(new_offset);
	return base->data()[new_offset];
}

_KSN_DETAIL_END

_KSN_END
