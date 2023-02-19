#include"print.h"
#include<vector>
#include<random>

void check_sorted(auto& c) {
	if (!std::is_sorted(c.begin(), c.end()))print("un");
	print("sorted: ");
}

void printc_(const auto& c) {
	check_sorted(c);
	for (auto& e : c)print("{} ", e);
	print("\n");
}

void randomize(auto& c) {
	static std::random_device rd;
	static std::default_random_engine rng(rd());
	std::shuffle(c.begin(), c.end(), rng);
}

struct things {
	std::string s_;
	int i_;
	std::string str()const {
		return std::format("({}, {})", s_, i_);
	}
};

void print_things(const auto& c) {
	for (auto& v : c)print("{} ", v.str());
	print("\n");
}

int main() {
	std::vector<int>v{ 1,2,3,4,5,6,7,8,9,10 };
	check_sorted(v);
	printc_(v);

	for (int i{ 3 }; i; i--) {
		randomize(v);
		printc_(v);
	}
	std::sort(v.begin(), v.end());
	printc_(v);

	print("partial_sort:\n");
	randomize(v);
	auto middle{ v.begin() + (v.size() / 2) };
	std::partial_sort(v.begin(), middle, v.end());
	printc_(v);

	std::partition(v.begin(), v.end(), [](int i) {return i > 5; });
	printc_(v);

	std::vector<things>vthings{ {"🐴",1},{"😘",2},{"🤣",3},{"🥵",4},{"🤡",5} };
	std::sort(vthings.begin(), vthings.end(),
		[](const things& lhs, const things& rhs) {
			return lhs.i_ > rhs.i_;
	});

	print_things(vthings);
}