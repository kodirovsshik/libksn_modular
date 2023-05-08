
import libksn.math;

import <stddef.h>;
import <stdint.h>;
import <intrin.h>;

import <iostream>;
import <fstream>;
import <string>;
import <vector>;
import <algorithm>;

int main()
{
	ksn::long_int a, b;
	
	a.resize(1);
	a[0] = -1;

	b.resize(2);
	b[0] = 0x8000000000000000;
	b[1] = 0;

	b *= a;
	
	a.resize(2);
	a[0] = 0;
	a[1] = 1;
	b += a;

	std::cout << b << '\n';
	std::cout << a << '\n';
}
