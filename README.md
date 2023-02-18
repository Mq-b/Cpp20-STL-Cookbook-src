# 《C++20 STL Cookbook》2023

##  环境
**Visual Studio Enterprise 2022(64位) 版本 17.4.3**

**CMake 3.8**

##  [B站视频讲解](https://www.bilibili.com/video/BV1r8411N75b/?spm_id_from=333.999.0.0&vd_source=1992ca910d6cd0582931f6f985dc7fa0)

## 第一章 C++20的新特性
### [1.2格式化文本](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/1.2%E6%A0%BC%E5%BC%8F%E5%8C%96%E7%89%B9%E5%8C%96formatter.cpp)
 ```cpp
#include<iostream>
#include<string_view>
#include<format>

template < typename... Args>
void print(const std::string_view fmt_str, Args&&... args) {
	auto fmt_args{ std::msake_format_args(args...) };
	std::string outstr{ std::vformat(fmt_str, fmt_args) };
	fputs(outstr.c_str(), stdout);
}

struct Frac {
	int a, b;
};

template<>
struct std::formatter<Frac> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext& ctx) {
		return ctx.begin();
	}
	template<typename FormatContext>
	auto format(const Frac& f, FormatContext& ctx) {
		return std::format_to(ctx.out(), "{0:d}/{1:d}", f.a, f.b);
	}
};

int main() {
	Frac f{ 1,10 };
	print("{}", f);
}
//格式化规则参见 https://zh.cppreference.com/w/cpp/utility/format/formatter
//特化规则参见:	https://zh.cppreference.com/w/cpp/named_req/Formatter
 ```

### [1.3使用编译时constexpr vector和string](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/1.3%E4%BD%BF%E7%94%A8%E7%BC%96%E8%AF%91%E6%97%B6constexpr%20vector%E5%92%8Cstring.cpp)
```cpp
#include<iostream>
#include<vector>
constexpr auto f() {
	std::vector<int>v{ 1,2,3 };
	return v;
}
constexpr auto f2() {
	int* p = new int{ 10 };
	//未delete解除分配
	return *p;
}

int main() {
	constexpr auto n = f().size();//√
	//constexpr auto n2 = f()//error
	//constexpr auto n3 = f2()//error
}

//constexpr: https://zh.cppreference.com/w/cpp/language/constexpr
```

### [1.4安全比较不同类型的整数cmp_less](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/1.4%E5%AE%89%E5%85%A8%E6%AF%94%E8%BE%83%E4%B8%8D%E5%90%8C%E7%B1%BB%E5%9E%8B%E7%9A%84%E6%95%B4%E6%95%B0cmp_less.cpp)
``` cpp
#include<iostream>

template<class T,class U>
constexpr bool cmp_less(T t, U u)noexcept {
	using UT = std::make_unsigned_t<T>;//有符号类型到无符号类型的安全转换。
	using UU = std::make_unsigned_t<U>;
	if constexpr (std::is_signed_v <T> == std::is_signed_v<U>)
		return t < u;
	else if constexpr (std::is_signed_v<T>)
		return t < 0 ? true : UT(t) < u;
	else
		return u < 0 ? false : t < UU(u);
}
int main() {
	std::cout << std::boolalpha << (5u < -1) << '\n';//true
	std::cout << std::boolalpha << ::cmp_less(5u, 1) << '\n';//false
	std::cout << std::boolalpha << ::cmp_less(5u, 2u) << '\n';//false
}

//整数比较函数: https://zh.cppreference.com/w/cpp/utility/intcmp
```

### [1.5三路比较运算符](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/1.5%E4%B8%89%E8%B7%AF%E6%AF%94%E8%BE%83%E8%BF%90%E7%AE%97%E7%AC%A6.cpp)
``` cpp
#include<iostream>

struct X {
	int a{};
	double b{};
	char c{};
	friend auto operator<=>(const X&,const X&) = default;
};

struct Y {
	int a = 6;
};

auto operator<=>(const Y& y, const Y& y2)noexcept->int {//自定义
	if (y.a == y2.a)return 0;
	if (y.a > y2.a)return 1;
	if (y.a < y2.a)return -1;
}
auto operator==(const Y& y, const Y& y2)noexcept->bool {
	return y.a == y2.a;
}

int main() {
	X x{ 10,1.2,'*' };
	X x2{ 10,1,'*' };
	x == x2;
	x <= x2;
	x != x2;
	x >= x2;
	Y y{ 1 };
	Y y2{ 2 };
	std::cout << (y <=> y2) << '\n';				//-1
	std::cout << std::boolalpha << (y > y2) << '\n';//false
	std::cout << std::boolalpha << (y < y2) << '\n';//true
	std::cout << std::boolalpha << (y != y2) << '\n';//true
}

//三路比较运算符: https://zh.cppreference.com/w/cpp/language/operator_comparison
//默认比较: https://zh.cppreference.com/w/cpp/language/default_comparisons
```

### [1.6查找特性测试宏](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/1.6%E6%9F%A5%E6%89%BE%E7%89%B9%E6%80%A7%E6%B5%8B%E8%AF%95%E5%AE%8F.cpp)
``` cpp
#include<iostream>
#include<version>

#ifdef __cpp_lib_three_way_comparison
# include<compare>
#else
# error 没有与之对应的头文件
#endif // __cpp_lib_three_way_comparison

#ifdef __cpp_lib_format
# include<format>
#else
# error 没有与之对应的头文件
#endif // __cpp_lib_three_way_comparison

#if __has_include(<iostream>)//检查能不能找到这个文件，如果能找到这个宏就返回1
#  include <iostream>
#endif

int main() {
	std::cout << __cpp_lib_three_way_comparison << '\n';//为 201907，意味着其在 2019 年 7 月采纳。
	std::cout << __cpp_lib_format << '\n'; //为 202110，意味着其在 2021 年 10 月采纳。
}

//库功能性测试宏: https://zh.cppreference.com/w/cpp/utility/feature_test
//诊断指令: https://zh.cppreference.com/w/cpp/preprocessor/error
```

### [1.7概念(concept)和约束(constraint)-创建更安全的模板](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/1.7%E6%A6%82%E5%BF%B5(concept)%E5%92%8C%E7%BA%A6%E6%9D%9F(constraint)-%E5%88%9B%E5%BB%BA%E6%9B%B4%E5%AE%89%E5%85%A8%E7%9A%84%E6%A8%A1%E6%9D%BF.cpp)
``` cpp
#include<iostream>

template<std::integral T>
void f(T t){}

template<class T>
	requires std::integral<T> || std::is_pointer_v<T>
struct X {

};

template<class T>
	requires std::is_integral_v<T>
T n{};

template<class T>
concept love = std::is_integral_v<T> && (std::is_same_v<int, T> || std::is_same_v<uint32_t, T>);

void f2(love auto){}

int main() {
	f(1);
	f('*');
	//f(1.2);
	X<int>x;
	//X<double>x2;
	X<double*>x3;
	n<int> = 3;
	//n<double>;
	std::cout << n<int> << '\n';
	f2(1);
	f2(1u);
	//f2(1l);
}

//Requires表达式 https://zh.cppreference.com/w/cpp/language/requires
//约束与概念 https://zh.cppreference.com/w/cpp/language/constraints
```

### [1.8模块](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/1.8%E6%A8%A1%E5%9D%97.cpp)

`1.8模块.cpp`

``` cpp
import test;

int main() {
	/*int n[]{
#include"t.txt"
	};
	for (auto i : n) {
		std::cout << i << ' ';
	}*/

	std::cout << mylib::add(1, 2) << '\n';
	//mylib::print("*");
	t();
}

//模块: https://zh.cppreference.com/w/cpp/language/modules
//编译设置:add_executable (Test1 "src/1.8模块.cpp" "src/test.ixx" "src/test2.ixx")
```

`test.ixx`

```cpp
module;
#define PI 3.14

export module test;
export import<iostream>;
export import test2;

namespace mylib {

	export auto add(std::integral auto a, std::integral auto b) {
		return a + b;
	}

	auto print(auto t) {
		std::cout << t << '\n';
	}
}
```

`test2.ixx`

```cpp
export module test2;
import<iostream>;

export void t() {
	std::cout << "乐\n";
}
```

`t.txt`

```
1,2,3,4,5
```



### [1.9视图](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/1.9%E8%A7%86%E5%9B%BE.cpp)

``` cpp
#include<iostream>
#include<ranges>
#include<vector>
namespace stdv = std::views;
namespace stdr = std::ranges;

void print(stdr::range auto v) {
	for (const auto& i : v)std::cout << i << ' ';
	endl(std::cout);
}

int main() {
	std::vector nums{ 1,2,3,4,5,6,7,8,9,10 };
	auto ret = nums | stdv::take(5) | stdv::reverse;
	print(ret);
	auto ret2 = nums | stdv::filter([](int i) {return i > 6; });
	print(ret2);
	auto ret3 = nums | stdv::transform([](int i) {return i * i; });
	print(ret3);
	print(nums);//视图是不会改变原来的数据的

	std::vector<std::string>strs{ "🐴","🐭","🥵","🤣" };
	auto ret4 = strs | stdv::reverse;
	print(ret4);

	auto ret5 = nums | stdv::filter([](int i) {return i % 2 != 0; }) | stdv::transform([](int i) {return i * 2; });
	print(ret5);
	
	auto nums_ = stdv::iota(1, 10);
	print(nums_);

	auto rnums = stdv::iota(1) | stdv::take(200);
	print(rnums);

	stdr::copy(strs | stdv::reverse | stdv::drop(2), std::ostream_iterator<std::string>(std::cout," "));
}

//范围库: https://zh.cppreference.com/w/cpp/ranges
```

### 第一章总结
第一章的内容需要细看，很多其实书说的并不全面，比如范围，模块，约束与概念，自己注意去看我们提到的之前讲过的视频，以及这些demo
如果你是初学，最好都自己写一下运行，顺便提一下`print.h`后面我们会经常用到这个头文件，我们其实是一步步补充的，但是我们直接把
这个头文件的内容先放出来吧，有不少的打印模板函数

**`print.h`**
```cpp
#pragma once
#include<format>
#include<iostream>
#include<ranges>
#include<map>

template < typename... Args>
void print(const std::string_view fmt_str, const Args&... args) {
	auto fmt_args{ std::make_format_args(args...) };
	std::string outstr{ std::vformat(fmt_str, fmt_args) };
	fputs(outstr.c_str(), stdout);
}

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
```

---
## 第二章 STL的泛型特性
### [2.2span类](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/2.2span%E7%B1%BB.cpp)

```cpp
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
```

### [2.3结构化绑定](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/2.3%E7%BB%93%E6%9E%84%E5%8C%96%E7%BB%91%E5%AE%9A.cpp)
```cpp
#include<iostream>
#include<format>
#include<array>
#include<tuple>
#include<map>

template < typename... Args>
void print(const std::string_view fmt_str, Args&&... args) {
	auto fmt_args{ std::make_format_args(args...) };
	std::string outstr{ std::vformat(fmt_str, fmt_args) };
	fputs(outstr.c_str(), stdout);
}

struct X { int a; double b; std::string str; };

auto f()->std::tuple<int,int> {
	return { 1,2 };
}

int main() {
	int array[]{ 1,2,3,4,5 };
	auto& [a, b, c, d, e] = array;
	print("{} {} {} {} {}\n", a, b, c, d, e);
	a = 10;
	print("{}\n", array[0]);

	std::array arr{ '*','a','b','&' };
	auto [a_, b_, c_, d_] = arr;
	print("{} {} {} {}\n", a_, b_, c_, d_);

	std::tuple<int, double, std::string>tu{ 10,3.14,"🥵" };
	auto [t1, t2, t3] = tu;
	print("{} {} {}\n", t1, t2, t3);

	X x{ 1,5.2,"🤣" };
	auto [x1, x2, x3] = x;
	print("{} {} {}\n", x1, x2, x3);

	const std::array arr2{ 1,2 };
	//auto& [c_arr1, c_arr2] = arr2;
	//c_arr1 = 10;//error

	auto [f1, f2] = f();
	print("{} {}\n", f1, f2);

	std::map<int, std::string>Map{ {1,"*"},{2,"😘"} };
	for (const auto& [m_a, m_b] : Map) {
		print("{} {}\n", m_a, m_b);
	}
}
```

### [2.4if&switch中的初始化](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/2.4if%26switch%E4%B8%AD%E7%9A%84%E5%88%9D%E5%A7%8B%E5%8C%96.cpp)
```cpp
#include"print.h"
#include<mutex>
#include<thread>

std::mutex m;
bool flag = true;

void f(int n) {
	if (std::lock_guard lg{ m }; flag) {
		print("乐\t");
		print("🤣🤣🤣\n");
	}
}

void t() {
	if (auto flag = [](int n) {return n * n; }(10); flag != 0) {
		print("🐴🐴🐴\n");
	}
}

void t2() {
	switch (char c = getchar();c)
	{
	case 'a':
		print("a\n"); 
		break;
	case 'b':
		print("b\n");
		break;
	case 'c':
		print("c\n");
		break;
	case 'd':
		print("d\n");
		break;
	default:
		print("error\n");
		break;
	}
}

int main() {
	for (int i = 0; i < 10; i++) {
		std::jthread t{ f,0 };
		std::jthread t2{ f,0 };
	}
	t();
	t2();
}
```

### [2.5模板参数推导](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/2.5%E6%A8%A1%E6%9D%BF%E5%8F%82%E6%95%B0%E6%8E%A8%E5%AF%BC.cpp)
```cpp

```

### [2.6编译期if](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/2.6%E7%BC%96%E8%AF%91%E6%9C%9Fif.cpp)
```cpp

```
---
