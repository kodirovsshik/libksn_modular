
import libksn.math;
//import libksn.time;

int main()
{
	ksn::modular_integer<int, 35> x(5);
	x /= x;
}