#include"print.h"
#include<vector>
#include<sstream>
#include<list>
#include<numbers>

namespace bw {
	template<typename T>
	std::ostream& join(T it, T end_it, std::ostream& o, std::string_view sep = "") {
		if (it != end_it)o << *it++;
		while (it != end_it)o << sep << *it++;
		return o;
	}

	template<typename I>
	std::string join(I it, I end_it, std::string_view sep = "") {
		std::ostringstream ostr;
		join(it, end_it, ostr, sep);
		return ostr.str();
	}

	std::string join(const auto& c, std::string_view sep = "") {
		return join(std::begin(c), std::end(c), sep);
	}
}

int main() {
	std::vector<std::string>greek{ "alpha","beta","gamma",
		"delta","epsilon" };
	for (const auto& c : greek) std::cout << c << ",";
	print("\n");
	auto greek_view = greek | std::views::join;
	for (const auto& c : greek_view) std::cout << c;
	print("\n");

	bw::join(greek.begin(), greek.end(), std::cout, ", ") << '\n';

	auto s = bw::join(greek.begin(), greek.end(), ", ");
	print("{}\n", s);

	auto s2 = bw::join(greek, ", ");
	print("{}\n", s2);

	std::list<double>list{ std::numbers::pi,std::numbers::e,std::numbers::sqrt2 };
	print("{}\n", bw::join(list, ": "));
}