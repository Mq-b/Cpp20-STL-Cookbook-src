#pragma once
#include<format>
#include<iostream>
#include<ranges>
#include<map>

template < typename... Args>
void print(const std::string_view fmt_str, Args&&... args) {
	auto fmt_args{ std::make_format_args(args...) };
	std::string outstr{ std::vformat(fmt_str, fmt_args) };
	fputs(outstr.c_str(), stdout);
}

template <typename Container>
concept ContainerChecker = requires (const Container & c) {
	std::ranges::begin(c);
	std::ranges::end(c);
};

template <ContainerChecker Ranges, typename CharT>
struct std::formatter<Ranges, CharT> {
	constexpr auto format(const Ranges& rg, auto& format_context) {
		auto iter = std::format_to(format_context.out(), "{}", '[');
		auto begin = std::ranges::begin(rg);
		auto end = std::ranges::end(rg);
		std::basic_string_view<CharT> fmt(m_fmt, m_fmt + m_buffer_len);
		for (auto vec_iter = begin; vec_iter != end; ++vec_iter) {
			if (vec_iter != begin) {
				iter = ',', iter = ' ';
			}
			iter = std::vformat_to(format_context.out(), fmt, std::make_format_args(*vec_iter));
		}
		iter = ']';
		return iter;
	}
	constexpr auto parse(auto& context) {//将会先调用parse成员函数，得到正确的格式化字符串再由format使用
		m_fmt[m_buffer_len++] = '{';
		auto iter = context.begin();
		if (iter == context.end() || *iter == '}') {
			m_fmt[m_buffer_len++] = '}';
			return iter;
		}
		m_fmt[m_buffer_len++] = ':';
		for (; iter != context.end() && *iter != '}'; ++iter) {
			m_fmt[m_buffer_len++] = *iter;
		}
		m_fmt[m_buffer_len++] = '}';
		return iter;
	}
private:
	CharT m_fmt[16]{};  //存储格式化字符串的缓冲区
	size_t m_buffer_len = 0;
};

void print(std::ranges::range auto v){
	print("size: {}  ", v.size());
	print("[ ");
	for (const auto& i : v)print("{} ", i);
	print("]\n");
}

template<class T,class T2>
void print(const std::map<T, T2>& map) {
	print("size: {} ", map.size());
	print("[ ");
	for (auto& [k, v] : map)print("{}:{} ", k, v);
	print("]\n");
}

template<class T, class T2>
void rprint(std::multimap<T, T2>& todo) {
	for (const auto& i : todo | std::views::reverse) {
		print("{}: {}\n", i.first, i.second);
	}
	print("\n");
}

void printc(const std::ranges::range auto& v, std::string_view s = "") {
	if (s.size())print("{}: ", s);
	for (const auto& i : v)print("[{}] ", i);
	print("\n");
}

void printr(const auto& r, std::string_view s = "") {
	auto rbegin = std::rbegin(r);
	auto rend = std::rend(r);
	for (auto it = rbegin; it != rend; ++it) {
		print("{} ", *it);
	}
	print("\n");
}