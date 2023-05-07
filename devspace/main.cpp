
import libksn.math;

import <stddef.h>;
import <stdint.h>;
import <intrin.h>;


uint64_t isign(uint64_t x)
{
	return (uint64_t)((int64_t)x >> 63);
}

template<size_t N>
void long_multiply(const uint64_t(&a)[N], const uint64_t(&b)[N], uint64_t(&c)[2 * N])
{
	using ull = unsigned long long;

	static constexpr size_t N2 = 2 * N;
	const uint64_t sign[2]{ isign(a[N - 1]), isign(b[N - 1]) };

	for (size_t i = 0; i < N2; ++i)
	{
		uint64_t multiplication_carry = 0;
		unsigned char addition_carry = 0;
		for (size_t j = 0; j < N2 - i; ++j)
		{
			uint64_t l, h;
			l = _umul128(
				i < N ? a[i] : sign[0],
				j < N ? b[j] : sign[1],
				&h
			);

			addition_carry = _addcarry_u64(addition_carry, l + multiplication_carry, c[j + i], &c[j + i]);
			multiplication_carry = h;
		}
	}
}

int main()
{
	ksn::long_int x, y;
	x.resize(1);
	x[0] = 0x8000000000000000;
	y.resize(1);
	y[0] = 1;
	x -= y;
}
