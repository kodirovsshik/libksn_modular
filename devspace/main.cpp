
import libksn.math;

namespace ksn
{
	template<>
	struct phi <int, 10>
	{
		static constexpr int value = 69;
	};
}

int main()
{
	ksn::modular_integer<int, 3> x = 0;
	++x;
	++x;
	++x;
	++x;
	++x;
}