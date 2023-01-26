#include"print.h"
#include<set>
#include<string>
#include<ranges>

int main() {
	std::set<std::string>sets;
	std::copy(std::istream_iterator<std::string>{std::cin}, {}, 
		std::inserter(sets, sets.end()));
	print(sets);
}