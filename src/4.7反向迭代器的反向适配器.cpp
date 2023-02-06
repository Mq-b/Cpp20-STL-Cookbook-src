#include"print.h"
#include<string>
#include<vector>

void printr(const auto& r, std::string_view s = "") {
	auto rbegin = std::rbegin(r);
	auto rend = std::rend(r);
	for (auto it = rbegin; it != rend; ++it) {
		print("{} ", *it);
	}
	print("\n");
}

int main() {
	int array[]{ 1,2,3,4,5 };
	printc(array, "c-array");
	auto it = std::begin(array);
	auto it_end = std::end(array);
	while (it != it_end) {
		print("{} ", *it++);
	}
	print("\n");

	auto it2 = std::rbegin(array);
	auto it_end2 = std::rend(array);
	while (it2 != it_end2) {
		print("{} ", *it2++);
	}
	print("\n");

	printr(array, "rev c-array");

	std::vector<int>v{ 1,2,3,4,5 };
	printc(v, "vector");
	printr(v, "rev vector");
}