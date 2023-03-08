#include"print.h"
#include<regex>
#include<fstream>

template<typename It>
void get_links(It it) {
	for (It end_it{}; it != end_it;) {
		const std::string link{ *it++ };
		if (it == end_it)break;
		const std::string desc{ *it++ };
		print("{:.<24} {}\n", link, desc);
	}
}

int main() {
	const char* fn{ R"(E:\自制视频教程\《C++20 STL Cookbook》2023\src\src\the-end.html)" };
	const std::regex link_re{ "<a href=\"([^\"]*)\"[^<]*>([^<]*)</a>" };
	std::string in{};
	std::ifstream infile{ fn,std::ios_base::in };
	for (std::string line{}; std::getline(infile, line);)in += line;

	std::sregex_token_iterator it{ in.begin(),in.end(),link_re,{1,2} };
	get_links(it);
}