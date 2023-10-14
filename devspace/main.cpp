
import libksn.math;

import std;

int main()
{
	ksn::sqmatrix<float, 3> m, n;
	m(0, 0) = m(1, 0) = 1;
	n(1, 0) = 4;
	auto x = m + n;
 }
