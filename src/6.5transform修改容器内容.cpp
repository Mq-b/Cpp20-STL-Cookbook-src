#include"print.h"
#include<vector>

std::string str_lower(const std::string& s) {
	std::string outstr{};
	for (const char& c : s) {
		outstr += tolower(c);
	}
	return outstr;
}

int main() {
	std::vector<int>v1{ 1,2,3,4,5,6,7,8,9,10 };
	std::vector<int>v2;
	printc(v1, "v1");
	std::transform(v1.begin(), v1.end(), std::back_inserter(v2), [](int x) {return x * x; });
	printc(v2, "v2");

	std::vector<std::string>vstr1{ "Aaa","Bbb","Ccc","DDD" };
	std::vector<std::string>vstr2;
	printc(vstr1, "vstr1");
	print("str_lower:\n");
	std::transform(vstr1.begin(), vstr1.end(), std::back_inserter(vstr2),
		[](std::string& x) {return str_lower(x); });
	printc(vstr2, "vstr2");

	print("ranges sequares:\n");
	auto view1 = std::views::transform(v1, [](int x) {return x * x; });
	printc(view1, "view1");

	v2.clear();
	std::ranges::transform(v1, std::back_inserter(v2), [](int x) {return x * x; });
	printc(v2, "v2");
}