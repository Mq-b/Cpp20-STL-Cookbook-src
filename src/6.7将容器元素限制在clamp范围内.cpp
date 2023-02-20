#include"print.h"
#include<vector>
#include<list>

constexpr int ilow{ 0 };
constexpr int ihigh{ 500 };

void printc_(const std::ranges::range auto& v, std::string_view s = "") {
	for (const auto& i : v)print("{:>5} ", i);
	print("\n");
}

int main() {
	auto il = { 0,-12,2001,4,5,-14,100,200 };
	std::vector<int>voi{ il };
	print("vector voi before:\n");
	printc_(voi);

	print("vector voi after:\n");
	for (auto& e : voi)e = std::clamp(e, ilow, ihigh);
	printc_(voi);

	print("list loi before:\n");
	std::list<int>loi{ il };
	printc_(loi);
	std::transform(loi.begin(), loi.end(), loi.begin(), [](auto e) {
		return std::clamp(e, ilow, ihigh);
	});
	print("list loi after:\n");
	printc_(loi);
}