
import libksn.math;

import std;

int main()
{
	using fp = float;
	ksn::matrix<fp, 4, 3> A{1,2,3,4,5,6,7,8,9,0,1,2,};
	ksn::matrix<fp, 3, 2> B{1,2,3,4,5,6,};
	auto C = 2 * A * B / 2;
 }
