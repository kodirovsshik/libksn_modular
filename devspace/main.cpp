
import libksn.math;
import libksn.time;
import <format>;
import <iostream>;

int main()
{
	ksn::stopwatch sw, swt;
	swt.start();
	sw.start();

	ksn::wilcoxon_Udistribution_data distrib(100, 100);
	auto dt1 = sw.restart();

	auto x = distrib.density(5000);
	auto dt2 = sw.current();

	std::cout << std::format("{}\n{} s\n{} s\n{} s total\n", x, dt1.as_sec(), dt2.as_sec(), swt.stop().as_sec());
	auto stat = distrib._storage_usage_statistics();

	std::cout << std::format("Alloc: {}/{} ({:.2f}%)\n", stat[0], stat[1], 100.0 * stat[0] / stat[1]);
	std::cout << std::format("Used: {}/{} ({:.2f}%)\n", stat[2], stat[3], 100.0 * stat[2] / stat[3]);
}
