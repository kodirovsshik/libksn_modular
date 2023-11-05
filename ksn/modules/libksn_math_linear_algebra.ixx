
export module libksn.math.linear_algebra;

import <ksn/ksn.hpp>;
import libksn.type_traits;



_KSN_BEGIN

_KSN_DETAIL_BEGIN

template<class T, size_t n_rows, size_t n_columns = 1>
class static_matrix_storage_t
{
private:
	T m_data[n_rows][n_columns]{};

public:
	static constexpr bool is_constexpr_size = true;
	static constexpr size_t n_rows = n_rows;
	static constexpr size_t n_columns = n_columns;

	//using transposed_t = static_matrix_storage_t<T, n_columns, n_rows>;
	using element_type = T;

	//auto&& operator()(this auto&& self, size_t row, size_t column = 0) noexcept { return self.m_data[row][column]; };
	T& operator()(size_t n, size_t m = 0) noexcept { return m_data[n][m]; };
	const T& operator()(size_t n, size_t m = 0) const noexcept { return m_data[n][m]; };

	T* begin() noexcept { return &this->m_data[0][0]; }
	const T* begin() const noexcept { return &this->m_data[0][0]; }
	T* end() noexcept { return this->begin() + this->size(); }
	const T* end() const noexcept { return this->begin() + this->size(); }

	static consteval size_t size() noexcept { return n_rows * n_columns; }
	static consteval size_t rows() noexcept { return n_rows; }
	static consteval size_t columns() noexcept { return n_columns; }
};

template<class T, size_t n_rows, size_t n_columns = 1>
class fixed_dymanic_matrix_storage_t
{
	using row_t = std::array<T, n_columns>;
	using storage_t = std::array<row_t, n_rows>;

private:
	std::unique_ptr<storage_t> m_data_ptr = std::make_unique<storage_t>();
	storage_t& data() noexcept { return *this->m_data_ptr; }
	const storage_t& data() const noexcept { return *this->m_data_ptr; }

public:

	fixed_dymanic_matrix_storage_t() = default;
	fixed_dymanic_matrix_storage_t(fixed_dymanic_matrix_storage_t&&) = default;
	fixed_dymanic_matrix_storage_t(const fixed_dymanic_matrix_storage_t& other)
	{
		for (size_t i = 0; i < this->size(); ++i)
			this->begin()[i] = other.begin()[i];
	}

	static constexpr bool is_constexpr_size = true;
	static constexpr size_t n_rows = n_rows;
	static constexpr size_t n_columns = n_columns;

	//using transposed_t = fixed_dymanic_matrix_storage_t<T, n_columns, n_rows>;
	using element_type = T;

	//auto&& operator()(this auto&& self, size_t row, size_t column = 0) noexcept { return self.data()[row][column]; };
	T& operator()(size_t n, size_t m = 0) noexcept { return this->data()[n][m]; };
	const T& operator()(size_t n, size_t m = 0) const noexcept { return this->data()[n][m]; };

	T* begin() noexcept { return &this->data()[0][0]; }
	const T* begin() const noexcept { return &this->data()[0][0]; }
	T* end() noexcept { return this->begin() + this->size(); }
	const T* end() const noexcept { return this->begin() + this->size(); }

	static consteval size_t size() noexcept { return n_rows * n_columns; }
	static consteval size_t rows() noexcept { return n_rows; }
	static consteval size_t columns() noexcept { return n_columns; }
};

template<class T>
concept matrix_storage = std::is_default_constructible_v<T> &&
	requires(T t, const T ct, size_t sz)
{
	typename T::element_type;
	{ t(sz, sz) } -> std::same_as<typename T::element_type&>;
	{ ct(sz, sz) } -> std::same_as<const typename T::element_type&>;
	{ ct.begin() } -> std::forward_iterator;
	{ ct.end() } -> std::forward_iterator;
	{ ct.size() } -> same_to_cvref<size_t>;
	{ ct.rows() } -> same_to_cvref<size_t>;
	{ ct.columns() } -> same_to_cvref<size_t>;
	{ T::is_constexpr_size } -> same_to_cvref<bool>;
};
template<class T>
concept fixed_ct_matrix_storage = matrix_storage<T> && T::is_constexpr_size && requires()
{
	{ T::size() } -> same_to_cvref<size_t>;
	{ T::n_rows } -> same_to_cvref<size_t>;
	{ T::n_columns } -> same_to_cvref<size_t>;
};

template<class S1, class S2>
concept ct_same_size_matrix = S1::is_constexpr_size && S2::is_constexpr_size && S1::n_rows == S2::n_rows && S1::n_columns == S2::n_columns;
//template<class S1, class S2>
//concept possibly_same_size_matrix = ct_same_size_matrix<S1, S2> || !S1::is_constexpr_size || !S2::is_constexpr_size;

_KSN_DETAIL_END

_KSN_END



_KSN_EXPORT_BEGIN

template<detail::matrix_storage storage_t>
class generic_matrix_t
{
	storage_t storage;
	
	using self_t = generic_matrix_t;

	template<std::forward_iterator Iter>
	void init_from(Iter begin, size_t size)
	{
		const size_t to_be_copied = std::min(this->size(), size);
		std::copy(begin, begin + to_be_copied, this->storage.begin());
	}

public:
	using element_type = storage_t::element_type;

	generic_matrix_t() = default;

	template<std::forward_iterator Iter>
	constexpr generic_matrix_t(Iter begin, size_t count)
	{
		this->init_from(begin, count);
	}
	constexpr generic_matrix_t(std::initializer_list<element_type> l)
	{
		this->init_from(l.begin(), l.size());
	}

	constexpr size_t rows() const noexcept
	{
		return this->storage.rows();
	}
	constexpr size_t columns() const noexcept
	{
		return this->storage.columns();
	}
	constexpr size_t size() const noexcept
	{
		return this->rows() * this->columns();
	}

	//auto&& operator()(this auto&& self, size_t row, size_t column = 0) noexcept { return self.storage(row)(column); };
	constexpr element_type& operator()(size_t n, size_t m = 0) noexcept { return this->storage(n, m); }
	constexpr const element_type& operator()(size_t n, size_t m = 0) const noexcept { return this->storage(n, m); }

	template<class other_storage_t>
		requires(detail::ct_same_size_matrix<storage_t, other_storage_t>)
	friend constexpr auto operator+(const self_t& a, const generic_matrix_t<other_storage_t>& b)
	{
		static_assert(std::is_same_v<storage_t, other_storage_t>);
		using combined_storage = storage_t;

		generic_matrix_t<combined_storage> result(a);
		
		for (size_t i = 0; i < storage_t::n_rows; ++i)
			for (size_t j = 0; j < storage_t::n_columns; ++j)
				result(i, j) += b(i, j);

		return result;
	}

	template<class other_storage_t> requires(
		detail::fixed_ct_matrix_storage<storage_t> && 
		detail::fixed_ct_matrix_storage<other_storage_t> &&
		storage_t::n_columns == other_storage_t::n_rows)
	friend constexpr auto operator*(const self_t& a, const generic_matrix_t<other_storage_t>& b)
	{
		using common_t = storage_t::element_type;
		//using common_t = std::common_type_t<storage_t::element_type, other_storage_t::element_type>;
		using combined_storage = detail::static_matrix_storage_t<common_t, storage_t::n_rows, other_storage_t::n_columns>;
		generic_matrix_t<combined_storage> result;

		for (size_t i = 0; i < storage_t::n_rows; ++i)
			for (size_t j = 0; j < storage_t::n_columns; ++j)
				for (size_t k = 0; k < other_storage_t::n_columns; ++k)
					result(i, k) += a(i, j) * b(j, k);

		return result;
	}

	constexpr self_t& operator*=(const element_type& scalar) noexcept
	{
		for (auto& x : this->storage)
			x *= scalar;
		return *this;
	}
	constexpr self_t& operator/=(const element_type& scalar) noexcept
	{
		for (auto& x : this->storage)
			x /= scalar;
		return *this;
	}
	friend constexpr self_t operator*(const element_type& scalar, const self_t& matrix) noexcept
	{
		return matrix * scalar;
	}
	friend constexpr self_t operator*(const self_t& matrix, const element_type& scalar) noexcept
	{
		auto copy = matrix;
		copy *= scalar;
		return copy;
	}
	friend constexpr self_t operator/(const self_t& matrix, const element_type& scalar) noexcept
	{
		auto copy = matrix;
		copy /= scalar;
		return copy;
	}
};

template<detail::fixed_ct_matrix_storage storage1_t, detail::fixed_ct_matrix_storage storage2_t>
constexpr auto convolution(const generic_matrix_t<storage1_t>& matrix, const generic_matrix_t<storage2_t>& kernel)
{
	static constexpr size_t result_rows = storage1_t::n_rows - storage2_t::n_rows + 1;
	static constexpr size_t result_columns = storage1_t::n_columns - storage2_t::n_columns + 1;
	//using common_t = std::common_type_t<storage1_t::element_type, storage2_t::element_type>;
	using common_t = storage1_t::element_type;
	using result_storage = detail::static_matrix_storage_t<common_t, result_rows, result_columns>;
	generic_matrix_t<result_storage> result;

	for (size_t y = 0; y < result.rows(); ++y)
	{
		for (size_t x = 0; x < result.columns(); ++x)
		{
			for (size_t i = 0; i < kernel.rows(); ++i)
			{
				for (size_t j = 0; j < kernel.columns(); ++j)
				{
					result(y, x) += matrix(y + i, x + j) * kernel(i, j);
				}
			}
		}
	}

	return result;
}


template<class T, size_t n, size_t m>
using matrix = generic_matrix_t<detail::static_matrix_storage_t<T, n, m>>;
template<class T, size_t n>
using sqmatrix = matrix<T, n, n>;
template<class T, size_t n>
using vector = matrix<T, n, 1>;

template<class T, size_t n, size_t m>
using hmatrix = generic_matrix_t<detail::fixed_dymanic_matrix_storage_t<T, n, m>>;
template<class T, size_t n>
using hsqmatrix = hmatrix<T, n, n>;
template<class T, size_t n>
using hvector = hmatrix<T, n, 1>;

_KSN_EXPORT_END
