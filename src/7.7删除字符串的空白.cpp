#include"print.h"
#include<ranges>

std::string trimstr(const std::string& s) {
	constexpr const char* whitespace{ " \t\r\n\v\f" };
	if (s.empty())return s;
	const auto first{ s.find_first_not_of(whitespace) };
	if (first == std::string::npos)return{};
	const auto last{ s.find_last_not_of(whitespace) };
	return s.substr(first, (last - first + 1));
}

int main() {
	std::string s{ " \t ten-thumbed input \t  \n \t" };
	print("[{}]\n", s);
	print("[{}]\n", trimstr(s));
}