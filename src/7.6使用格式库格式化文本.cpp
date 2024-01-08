#include"print.h"
#include<numbers>
#include<vector>

template<typename T>
struct Frac {
	T n;
	T d;
};
template<typename T>//十分简略，不支持很多操作
struct std::formatter<Frac<T>> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext& ctx) {
		return ctx.begin();
	}
	template<typename FormatContext>
	constexpr auto format(const Frac<T>& f, FormatContext& ctx) const {
		return std::format_to(ctx.out(), "{0:d}/{1:d}", f.n, f.d);
	}
};

template<typename T>
struct Frac2 {
	T n;
	T d;
};
template<typename T>
struct std::formatter<Frac2<T>> {
	constexpr auto parse(auto& ctx) {
		m_fmt[m_buffer_len++] = '{';
		auto iter = ctx.begin();
		if (iter == ctx.end() || *iter == '}') {
			m_fmt[m_buffer_len++] = '}';
			return iter;
		}
		m_fmt[m_buffer_len++] = ':';
		for (; iter != ctx.end() && *iter != '}'; ++iter)
			m_fmt[m_buffer_len++] = *iter;
		m_fmt[m_buffer_len++] = '}';
		return iter;
	}
	constexpr auto format(const Frac2<T>& f, auto& ctx) const {
		std::string fmt{};
		fmt += m_fmt, fmt += "/", fmt += m_fmt;
		auto iter = std::vformat_to(ctx.out(), fmt, std::make_format_args(f.n,f.d));
		return iter;
	}
private:
	char m_fmt[16]{};
	size_t m_buffer_len = 0;
};

int main() {
	const int a{ 47 };
	const char* human{ "earthlings" };
	const std::string_view alien{ "vulacans" };
	const double df_pi{ std::numbers::pi };
	const int inta{ 47 };

	print("Hex: {0:x} Octal: {0:o} Decunak {0:d}\n", a);
	print("Hello {1} we are {0}\n", human, alien);

	print("π is {}\n", df_pi);
	print("π is {:.5}\n", df_pi);

	print("inta is [{:10}]\n", inta);
	print("inta is [{:<10}]\n", inta);
	print("inta is [{:>10}]\n", inta);

	print("inta is [{:*<10}]\n", inta);
	print("inta is [{:0>10}]\n", inta);

	print("inta is [{:^10}]\n", inta);
	print("inta is [{:_^10}]\n", inta);

	print("{:>8}: [{:04x}]\n", "Hex", inta);
	print("{:>8}: [{:4o}]\n", "Octal", inta);
	print("{:>8}: [{:4d}]\n", "Decimal", inta);

	Frac<long>n{ 3,5 };
	print("{}\n", n);
	//print("{:0x}\n", n);//error，因为我们的特化过于简单
	Frac2<long>n2{ 10,5 };
	print("{:0x}\n", n2);

	int array[] = { 1,2,3,4,5,6 };
	std::vector v = { 122, 1222, 1222 };
	print("{:0x}\n", v);
	print("{:0x}\n", array);
}