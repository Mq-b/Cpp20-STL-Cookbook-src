#include"print.h"
#include<vector>
#include<algorithm>

struct City {
	std::string name{};
	unsigned pop{};
	bool operator==(const City& o)const {
		return name == o.name;
	}
	std::string str()const {
		return std::format("[{}, {}]", name, pop);
	}
};

int main() {
	const std::vector<int>v{ 1,2,3,4,5,6,7,8,9,10 };

	auto it1 = std::find(v.begin(), v.end(), 7);
	if (it1 != v.end())print("found: {}\n", *it1);
	else print("not found:\n");

	const std::vector<City>c{
		{"London",8425622},
		{"Berlin",3566791},
		{"Tokyo",37435191},
		{"Cairo",20485965}
	};
	auto it2 = std::find(c.begin(), c.end(), City{ "Berlin" });
	if (it2 != c.end())print("found: {}\n", it2->str());
	else print("not found:\n");

	auto it3 = std::find_if(begin(c), end(c), [](const City& item) {
		return item.pop > 20000000;
	});
	if (it3 != c.end())print("found: {}\n", it3->str());
	else print("not found:\n");

	auto vwl = std::views::filter(c, [](const City& item) {
		return item.pop > 20000000;
	});
	for (const City& e : vwl)print("{}\n", e.str());
}