#include"print.h"
#include<vector>
#include<algorithm>

int main() {
	std::vector<std::string>vs1{ "dog","cat","veloiraptor" };
	std::vector<std::string>vs2{ "kirk","sulu","spock" };
	std::vector<std::string>dest{};
	printc(vs1, "vs1");
	printc(vs2, "vs2");

	std::ranges::sort(vs1);
	std::ranges::sort(vs2);
	printc(vs1, "vs1");
	printc(vs2, "vs2");

	std::merge(vs1.begin(), vs1.end(), vs2.begin(), vs2.end(), std::back_inserter(dest));
	printc(dest, "dest");
}