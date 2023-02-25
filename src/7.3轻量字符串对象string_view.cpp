#include"print.h"
#include<string>
using namespace std::literals;

std::string_view sv() {
	const char text[]{ "hello" };
	std::string_view greeting{ text };
	return greeting;
}

void f(const std::string& str) {

}
void f2(std::string_view str) {

}

int main() {
	char str[10]{ "hello" };
	std::string str2{ str };
	print("{}\n", str2);
	str[0] = 'a';
	print("{}\n", str2);

	std::string_view sview{ str };
	print("{}\n", sview);
	str[0] = 'b';
	print("{}\n", sview);

	auto t = sv();
	print("{}\n", t);

	print("{}\n", "hello"sv.substr(1,4));

	constexpr std::string_view str3{ "哈哈" };
	//constexpr std::string str4{ "哈哈" };//error

	print("{}\n", str3);

	std::string str4{ "1" };
	const std::string str5{ "1" };
	f(str4);
	f(str5);
	f("1");//开销大，需要构造临时的std::string对象

	f2("1");
	f2(str4);
	f2(str5);
}