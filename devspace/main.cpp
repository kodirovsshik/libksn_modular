
import libksn.aads;

import <stdexcept>;
import <cassert>;

void run_test1()
{
	//Try popping an empty ring

	ksn::lock_free_pod_ring<int, 4> ring;

	try
	{
		ring.pop();
		assert(false);
	}
	catch (const std::out_of_range&) {}
}
void run_test2()
{
	//See if ring rings (no overwrite)

	ksn::lock_free_pod_ring<int, 4> ring;
	ring.push(1);
	ring.push(2);
	ring.push(3);
	assert(ring.pop() == 1);
	assert(ring.pop() == 2);
	assert(ring.pop() == 3);
	try
	{
		ring.pop();
		assert(false);
	}
	catch (const std::out_of_range&) {}
}

void run_test3()
{
	//See if ring rings (yes overwrite)

	ksn::lock_free_pod_ring<int, 4> ring;
	for (int i = 1; i <= 10; ++i)
		ring.push(i);

	assert(ring.pop() == 7);
	assert(ring.pop() == 8);
	assert(ring.pop() == 9);
	assert(ring.pop() == 10);

	try
	{
		ring.pop();
		assert(false);
	}
	catch (const std::out_of_range&) {}
}

void run_tests()
{
	run_test1();
	run_test2();
	run_test3();
}

int main()
{
	run_tests();
}
