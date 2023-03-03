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
concept ContainerChecker = requires (const Container & c) {//与std::ranges::range等价
	std::ranges::begin(c);
	std::ranges::end(c);
};

template <ContainerChecker Ranges, typename CharT>
struct std::formatter<Ranges, CharT> {
	constexpr auto format(const Ranges& rg, auto& format_context) {
		auto iter = std::format_to(format_context.out(), "{}", '[');//相当于先往format_context插入一个'['，使用的{}格式化的方式，并且会返回末尾迭代器，后面都将操作它
		auto begin = std::ranges::begin(rg);
		auto end = std::ranges::end(rg);
		std::basic_string_view<CharT> fmt(m_fmt, m_fmt + m_buffer_len);//使用数据成员字符数组来构造一个string_view，使其拥有和字符数组一样的数据
		for (auto vec_iter = begin; vec_iter != end; ++vec_iter) {
			if (vec_iter != begin) {//第一次是==begin的，不会执行，后面会执行
				iter = ',', iter = ' ';//看似是给iter重复赋值，实际上它会自己往后增加，相当于往插入迭代器插入了','和' '
			}
			iter = std::vformat_to(format_context.out(), fmt, std::make_format_args(*vec_iter));//将一个元素插入到format_context，返回末尾迭代器
		}
		iter = ']';//相当于再插入了一个']'
		return iter;
	}
	constexpr auto parse(auto& context) {//将会先调用parse成员函数，得到正确的格式化字符串再由format使用
		m_fmt[m_buffer_len++] = '{';//先放入一个'{'
		auto iter = context.begin();
		if (iter == context.end() || *iter == '}') {
			m_fmt[m_buffer_len++] = '}';
			return iter;
		}
		m_fmt[m_buffer_len++] = ':';//如果上一步没return 退出，也就是不是单纯的{}，那么就需要给一个:，在这里相当于已经插入了{:
		for (; iter != context.end() && *iter != '}'; ++iter) {
			m_fmt[m_buffer_len++] = *iter;
		}
		m_fmt[m_buffer_len++] = '}';//最后放入'}'
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