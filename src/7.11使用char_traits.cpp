#include"print.h"

constexpr char char_lower(const char& c) {//全部转换为小写
	if (c >= 'A' && c <= 'Z')return c + 32;
	else return c;
}

class ci_traits :public std::char_traits<char> {
public:
	static constexpr bool lt(char_type a, char_type b)noexcept {
		return char_lower(a) < char_lower(b);
	}
	static constexpr bool eq(char_type a, char_type b)noexcept {
		return char_lower(a) == char_lower(b);
	}
	static constexpr int compare(const char_type* s1, const char_type* s2, size_t count) {
		for (size_t i{ 0 }; i < count; ++i) {
			auto diff{ char_lower(s1[i]) <=> char_lower(s2[i]) };
			if (diff > 0)return 1;
			if (diff < 0)return -1;
		}
		return 0;
	}
	static constexpr const char_type* find(const char_type* p, size_t count, const char_type& ch) {
		const char_type find_c{ char_lower(ch) };
		for (size_t i{ 0 }; i < count; ++i) {
			if (find_c == char_lower(p[i]))return p + i;
		}
		return nullptr;
	}
};

using ci_string = std::basic_string<char, ci_traits>;

template<>
struct std::formatter<ci_string> {
	constexpr auto format(const ci_string& rg, auto& format_context) {
		return std::format_to(format_context.out(), "{}", rg.c_str());
	}
	constexpr auto parse(auto& context) {
		context.begin();
	}
};

std::ostream& operator<<(std::ostream& os, const ci_string& str) {
	return os << str.c_str();
}

int main() {
	std::string s{ "🤣🤣" };
	ci_string ci_s{ "🐴🐴" };
	std::cout << s << '\n' << ci_s << '\n';

	ci_string compare1{"CompArE StRiNg"};
	ci_string compare2{ "compare string" };
	if (compare1 == compare2) {
		printf("Match! %s == %s\n", compare1.data(), compare2.data());
	}else {
		printf("no match %s != %s\n", compare1.data(), compare2.data());
	}
}