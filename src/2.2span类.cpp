#include<iostream>
#include<format>
#include<span>

template < typename... Args>
void print(const std::string_view fmt_str, Args&&... args) {
	auto fmt_args{ std::make_format_args(args...) };
	std::string outstr{ std::vformat(fmt_str, fmt_args) };
	fputs(outstr.c_str(), stdout);
}

template<class T>
void pspan(std::span<T>s) {
	print("number of elemnts: {}\n", s.size());
	print("size of span: {}\n", s.size_bytes());
	for (auto i : s)print("{} ", i);
	endl(std::cout);
}

int main() {
	int array[]{ 1,2,3,4,5,6 };
	pspan<int>(array);
}

//span文档: https://zh.cppreference.com/w/cpp/container/span
//span简单实现: https://github.com/13870517674/c-plus-plus/blob/master/src/lib/span.hpp