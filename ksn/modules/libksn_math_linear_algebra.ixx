
export module libksn.math.linear_algebra;

import <ksn/ksn.hpp>;



_KSN_BEGIN

_KSN_DETAIL_BEGIN

template<class T, size_t n_rows, size_t n_cols = 1>
class static_matrix_storage_t
{
private:
	T m_data[n_cols][n_rows]{};

public:
	//auto&& operator()(this auto&& self, size_t row, size_t column = 0) noexcept { return self.m_data[row][column]; };
	T& operator()(size_t n, size_t m = 0) noexcept { return m_data[n][m]; };
	const T& operator()(size_t n, size_t m = 0) const noexcept { return m_data[n][m]; };

	consteval size_t size() const noexcept { return n_rows * n_cols; }

	static constexpr bool is_constexpr_size = true;
	static constexpr size_t rows = n_rows;
	static constexpr size_t cols = n_cols;

	using transposed_t = static_matrix_storage_t<T, n_cols, n_rows>;
	using element_type = T;
};

template<class T>
concept matrix_storage = requires(T t, const T ct, size_t sz)
{
	typename T::element_type;
	{ t(sz, sz) } -> std::same_as<typename T::element_type&>;
	{ ct(sz, sz) } -> std::same_as<const typename T::element_type&>;
	{ ct.size() } -> std::same_as<size_t>;
	{ T::is_constexpr_size } -> std::same_as<bool>;
};
template<class T>
concept fixed_ct_matrix_storage = matrix_storage<T> && T::is_constexpr_size && requires()
{
	{ T::size() } -> std::same_as<size_t>;
};

template<class S1, class S2>
concept ct_same_size_storage = S1::is_constexpr_size && S2::is_constexpr_size && S1::rows == S2::rows && S1::cols == S2::cols;
template<class S1, class S2>
concept possibly_same_size_storage = ct_same_size_storage<S1, S2> || !S1::is_constexpr_size || !S2::is_constexpr_size;

_KSN_DETAIL_END

_KSN_END



_KSN_EXPORT_BEGIN

template<class storage_t>
class generic_matrix_t
{
	storage_t storage;
	
	using self_t = generic_matrix_t;

public:
	using element_type = storage_t::element_type;

	generic_matrix_t() = default;

	//auto&& operator()(this auto&& self, size_t row, size_t column = 0) noexcept { return self.storage(row)(column); };
	element_type& operator()(size_t n, size_t m) noexcept { return this->storage(n, m); }
	const element_type& operator()(size_t n, size_t m) const noexcept { return this->storage(n, m); }

	template<class other_storage_t>
		requires(detail::ct_same_size_storage<storage_t, other_storage_t>)
	friend auto operator+(const self_t& a, const generic_matrix_t<other_storage_t>& b)
	{
		static_assert(std::is_same_v<storage_t, other_storage_t>);
		using combined_storage = storage_t;

		generic_matrix_t<combined_storage> result(a);
		
		for (size_t i = 0; i < storage_t::rows; ++i)
			for (size_t j = 0; j < storage_t::cols; ++j)
				result(i, j) += b(i, j);

		return result;
	}

	template<class other_storage_t> requires(
		detail::fixed_ct_matrix_storage<storage_t> && 
		detail::fixed_ct_matrix_storage<other_storage_t> &&
		storage_t::columns == other_storage_t::rows)
	friend auto operator*(const self_t&, const generic_matrix_t<other_storage_t>&);
};

template<class T, size_t n, size_t m>
using matrix = generic_matrix_t<detail::static_matrix_storage_t<T, n, m>>;
template<class T, size_t n>
using sqmatrix = matrix<T, n, n>;

template<class T, size_t n>
using vector = matrix<T, n, 1>;

_KSN_EXPORT_END
