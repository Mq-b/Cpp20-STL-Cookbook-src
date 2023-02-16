#include"print.h"
#include<vector>
namespace stdr = std::ranges;

int main() {
	std::vector<std::string>v1{ "alpha","beta","gamma","delta","epsilon" };
	printc(v1,"v1");
	std::vector<std::string>v2(v1.size());
	std::copy(v1.begin(), v1.end(), v2.begin());
	printc(v2, "v2");

	std::copy(v1.begin(), v1.end(), std::back_inserter(v2));
	printc(v2, "v2");

	std::vector<std::string>v3{};
	std::copy_n(v1.begin(), 3, std::back_inserter(v3));
	printc(v3, "v3");

	std::vector<std::string>v4{};
	/*std::copy_if(v1.begin(), v1.end(), std::back_inserter(v4), [](auto& s) {
		return s.size() > 4;
	});*/
	stdr::copy_if(v1,std::back_inserter(v4), [](auto& s) {
		return s.size() > 4;
		});
	printc(v4, "v4");

	stdr::copy(v1, std::ostream_iterator<std::string>{std::cout, " "});
	print("\n");

	stdr::move(v1, v2.begin());
	printc(v1, "after move: v1");
	printc(v2, "after move: v2");
}