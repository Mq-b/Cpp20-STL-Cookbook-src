#include"print.h"
#include<fstream>
#include<filesystem>

size_t wordcount(auto& is) {
	using it_t = std::istream_iterator<std::string>;
	return std::distance(it_t{ is }, {});
}

int main() {
	const char* fn{ "E:/自制视频教程/《C++20 STL Cookbook》2023/src/src/the-raven.txt" };
	std::ifstream infile{ fn,std::ios_base::in };
	size_t wc{ wordcount(infile) };
	print("There are {} words in the file.\n", wc);
	print("size: {}\n", std::filesystem::file_size(fn));
}