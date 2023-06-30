
import libksn.math;

import std;

int main()
{
	ksn::int128 x;
	x[0] = 0;
	x[1] = 0x8000000000000000;

	ksn::uint128 y = x;
	std::cout << x << "\n " << y;
}
