#include"print.h"
#include<string>
#include<deque>
#include<algorithm>
#include<vector>
#include<sstream>

int main() {
	std::deque<int>d1{ 1,2,3,4,5 };
	std::deque<int>d2(d1.size());
	std::copy(d1.begin(), d1.end(), d2.begin());
	printc(d2, "d2 after copy");

	std::copy(d1.begin(), d1.end(), std::back_inserter(d2));
	printc(d2, "d2 after back_inserter");

	std::deque<int>d3{ 47,73,114,138,54 };
	std::copy(d3.begin(), d3.end(), std::front_inserter(d2));
	printc(d2, "d2 after front_inserter");

	auto it2{ d2.begin() + 2 };
	std::copy(d1.begin(), d1.end(), std::inserter(d2, it2));
	printc(d2, "d2 after minddle insert");

	print("ostream_iterator:");
	std::copy(d1.begin(), d1.end(), std::ostream_iterator<int>{std::cout," "});
	print("\n");
	
	/*std::vector<std::string>vs{};
	std::copy(std::istream_iterator<std::string>(std::cin), {}, std::back_inserter(vs));
	printc(vs, "vs2");*/

	/*std::vector<int> V(std::istream_iterator<int>(std::cin), {});
	printc(V, "V");*/

	for (auto it = std::istream_iterator<std::string>(std::cin);
		it != std::istream_iterator<std::string>{}; ++it) {
		print("{} ", *it);
	}
}