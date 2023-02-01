#pragma once
#include<format>
#include<iostream>
#include<ranges>
#include<map>

template < typename... Args>
inline void print(const std::string_view fmt_str, Args&&... args) {
	auto fmt_args{ std::make_format_args(args...) };
	std::string outstr{ std::vformat(fmt_str, fmt_args) };
	fputs(outstr.c_str(), stdout);
}

inline void print(std::ranges::range auto v){
	print("size: {}  ", v.size());
	print("[ ");
	for (const auto& i : v)print("{} ", i);
	print("]\n");
}

template<class T,class T2>
inline void print(const std::map<T, T2>& map) {
	print("size: {} ", map.size());
	for (auto& [k, v] : map)print("{}:{} ", k, v);
	print("\n");
}

template<class T, class T2>
void rprint(std::multimap<T, T2>& todo) {
	for (const auto& i : todo | std::views::reverse) {
		print("{}: {}\n", i.first, i.second);
	}
	print("\n");
}