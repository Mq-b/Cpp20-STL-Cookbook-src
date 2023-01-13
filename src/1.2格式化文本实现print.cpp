#include<iostream>
#include<string_view>
#include<format>

template < typename... Args>
void print(const std::string_view fmt_str, Args&&... args) {
	auto fmt_args{ std::make_format_args(args...) };
	std::string outstr{ std::vformat(fmt_str, fmt_args) };
	fputs(outstr.c_str(), stdout);
}

int main() {
	print("Hello, {}!\n", "who");
	print("Hello, {}!\n", '*');
	print("{:b}\n", 0b01010101);
}
/*打印结果:
Hello, who!
Hello, *!
1010101
*/