#include"print.h"
#include<numbers>

template<typename T>
struct Frac {
	T n;
	T d;
};
template<typename T>
struct std::formatter<Frac<T>> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext& ctx) {
		return ctx.begin();
	}
	template<typename FormatContext>
	auto format(const Frac<T>& f, FormatContext& ctx) {
		return std::format_to(ctx.out(), "{0:d}/{1:d}", f.n, f.d);
	}
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
}