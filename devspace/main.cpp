
//import libksn.math;

#pragma warning(disable : 4005 4067 4996)

#include <fstream>;
#include <variant>;
#include <print>;
#include <source_location>;

#include <winsock2.h>;
#include <Windows.h>;
#include <mstcpip.h>;


#pragma comment(lib, "ws2_32.lib")


#define invoke_variant(variant, method, ...) std::visit([&](auto&& _invoke_variant_lambda_arg){ _invoke_variant_lambda_arg.method(__VA_ARGS__); }, variant)

void wsa_check(std::source_location loc = std::source_location::current())
{
	const auto err = WSAGetLastError();
	if (err == 0)
		return;

	std::println("{}:{}: WSA error {}", loc.file_name(), loc.line(), err);
	__debugbreak();
	exit(-1);
}

int __init_result = []
{
	WSAData wsadata{};
	(void)WSAStartup(MAKEWORD(2, 0), &wsadata);
	wsa_check();

	return 0;
}
();


int main()
{
	auto s = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	
	{
		sockaddr_in bind_addr{};
		bind_addr.sin_family = AF_INET;
		bind_addr.sin_addr.s_addr = inet_addr("192.168.100.14");
		bind(s, (sockaddr*)&bind_addr, sizeof(bind_addr));
		wsa_check();
	}
	
	{
		int value = RCVALL_ON;
		DWORD out = 0;
		int err = WSAIoctl(s, SIO_RCVALL, &value, sizeof(value), NULL, 0, &out, NULL, NULL);
		wsa_check();
		//RCVALL_
		//WSAIoctl(s, SIO_RCVALL, )
	}

	
	for (size_t n = 0; true;)
	{
		static thread_local char buff[65550];

		int bytes = recv(s, buff, sizeof(buff), 0);
		wsa_check();

		if (bytes == 0)
			break;



		std::println("Recieved {} bytes", bytes);
		std::ofstream(std::format("packets/{}", n++), std::ios::out | std::ios::binary).write(buff, bytes);
	}

 }
