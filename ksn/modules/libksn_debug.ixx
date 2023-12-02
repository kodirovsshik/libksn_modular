
export module libksn.debug;

import <ksn/ksn.hpp>;



_KSN_EXPORT_BEGIN


void trap()
{
#ifdef _KSN_COMPILER_MSVC
	__debugbreak();
#else
	static_assert(false, "Not implemented");
#endif
}



class instance_debugger
{
	uint32_t m_signature = signature;

	//static int instance_count;

public:
	int payload = 0;

	enum class failed_test_action{ ignore, trap, exception};

	static bool enable_logging;
	static failed_test_action on_failed_test;

	static constexpr uint32_t signature = 0xABCDEF01;


	instance_debugger() noexcept;
	instance_debugger(int payload) noexcept;
	instance_debugger(const instance_debugger&);
	instance_debugger(instance_debugger&&);

	instance_debugger& operator=(const instance_debugger& other);
	instance_debugger& operator=(instance_debugger&& other);

	~instance_debugger();

	bool test() const;
};


//int instance_debugger::instance_count = 0;
bool instance_debugger::enable_logging = true;
instance_debugger::failed_test_action instance_debugger::on_failed_test = failed_test_action::trap;

#define log(fmt, ...) { if(enable_logging) std::print(fmt __VA_OPT__(,) __VA_ARGS__); }

instance_debugger::instance_debugger() noexcept
{
	log("instance_debugger()\n");
}
instance_debugger::instance_debugger(int x) noexcept
	: payload(x)
{
	log("instance_debugger(int)\n");
}
instance_debugger::instance_debugger(const instance_debugger& x)
	: payload(x.payload)
{
	x.test();
	log("instance_debugger(const instance_debugger&)\n");
}
instance_debugger::instance_debugger(instance_debugger&& x)
	: payload(x.payload)
{
	x.test();
	log("instance_debugger(instance_debugger&&)\n");
	x.payload = 0;
}

instance_debugger& instance_debugger::operator=(const instance_debugger& other)
{
	this->test();
	other.test();
	this->payload = other.payload;
	return *this;
}
instance_debugger& instance_debugger::operator=(instance_debugger&& other)
{
	this->test();
	other.test();
	this->payload = other.payload;
	other.payload = 0;
	return *this;
}

instance_debugger::~instance_debugger()
{
	this->test();
}


#define TEST_FAILED_MSG_ARGS "instance_debugger: test failed for address 0x{:016x}\n", (uint64_t)this

bool instance_debugger::test() const
{
	if (this->m_signature == this->signature)
		return true;

	log(TEST_FAILED_MSG_ARGS);

	switch (this->on_failed_test)
	{
	case failed_test_action::exception:
#if _KSN_HAS_EXCEPTIONS
		throw std::runtime_error(std::format(TEST_FAILED_MSG_ARGS));
#else
		log("instance_debugger: failed to raise expection")
#endif

	case failed_test_action::trap:
		trap();
		break;
	}

	return false;
}



_KSN_EXPORT_END
