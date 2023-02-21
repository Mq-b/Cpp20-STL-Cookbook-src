#include"print.h"
#include<vector>
#include<algorithm>

int main() {
	std::vector<std::string>vs{ "dog","cat","velociraptor" };   
	std::sort(vs.begin(), vs.end());
	do {
		printc(vs);
	} while (std::next_permutation(vs.begin(), vs.end()));

	std::vector<int>v{ 1,2,3 };
	do
	{
		printc(v);
	} while (std::next_permutation(v.begin(),v.end()));
	print("从大到小:\n");
	std::sort(v.begin(), v.end(), std::greater<int>{});
	do
	{
		printc(v);
	} while (std::next_permutation(v.begin(), v.end()));
}