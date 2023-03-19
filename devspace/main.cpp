
import libksn.unicode;
import <string>;

int main()
{
	std::u8string str(u8"Привет"), str2;
	std::wstring winapi_str;
	ksn::unicode_string_convert<ksn::bom::mandate>(str2, str);
	ksn::unicode_string_convert<ksn::bom::keep>(winapi_str, str2);
}
