# 《C++20 STL Cookbook》2023

###  环境
**Visual Studio Enterprise 2022(64位) 版本 17.4.3**

**CMake 3.8**

###  [B站视频讲解](https://www.bilibili.com/video/BV1r8411N75b/?spm_id_from=333.999.0.0&vd_source=1992ca910d6cd0582931f6f985dc7fa0)

<br>

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

<br>

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
#include"print.h"

using namespace std::string_literals;

template<class T>
struct X {
	T v{};
	template<class...Args>
	X(Args&&...args) :v{ (args + ...) } {}
};

template<class...Ts>
X(Ts...ts) -> X<std::common_type_t<Ts...>>;

int main() {
	X x("10","🤣"s);
	print("{}\n", x.v);
}
```

### [2.6编译期if](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/2.6%E7%BC%96%E8%AF%91%E6%9C%9Fif.cpp)
```cpp
#include"print.h"

template<class T>
auto f(const T& v) {
	if constexpr (std::is_pointer_v<T>)
		print("pointer\n");
	else
		print("no pointer\n");
}

template<class T,class...Args>
void show(T t, Args&&...args) {
	print("{}\t",t);
	if constexpr (sizeof...(args)) {
		show(args...);
	}
}

int main() {
	int* p{};
	f(p);
	f(1);
	show(5,314, "🤣", '*');
	print("\n");
}
```
### 第二章总结
第二章内容总体比较简单，并没有什么困难的，重在运用，最好这些demo都自己抄或者写一遍。
加深理解

<br>

---
## 第三章STL容器
### [3.3使用擦除函数从容器中擦除项](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/3.3%E4%BD%BF%E7%94%A8%E6%93%A6%E9%99%A4%E5%87%BD%E6%95%B0%E4%BB%8E%E5%AE%B9%E5%99%A8%E4%B8%AD%E6%93%A6%E9%99%A4%E9%A1%B9.cpp)
```cpp
#include"print.h"
#include<vector>
#include<list>

template<typename Tc,typename Tv>
void remove_value(Tc& c, const Tv& v) {//C++20之前的做法
	//std::remove将和传入元素相同的元素移动放到末尾，并返回迭代器位置，还有一个std::remove_if的版本
	auto remove_it = std::remove(c.begin(), c.end(), v);//remove_it是首个需要被删除元素的位置
	c.erase(remove_it, c.end());//删除remove_it到end()这个范围的元素
}

int main() {
	std::vector v{ 1,2,3,4,5 };
	print(v);
	::remove_value(v, 1);
	print(v);
	std::erase(v,5);//C++20起，功能和remove_value()相同
	print(v);
	std::erase_if(v, [](int i) {return i % 2 != 0; });//第二个版本
	print(v);

	std::list list{ 1,2,3,4,5,6,7,8,9,10 };
	std::erase(list, 5);
	std::erase_if(list, [](int i) {return i % 2 == 0; });
	print(list);

	std::map<int, std::string> map{ {1,"🤣"},{2,"🥵"},{3,"🐴"},{4,"🐭"} };
	print(map);
	std::erase_if(map, [](auto& i) {
		const auto& [k, v] = i;
		return v == "🥵";
	});
	print(map);
}
```

### [3.4常数时间内从未排序的向量中删除项](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/3.4%E5%B8%B8%E6%95%B0%E6%97%B6%E9%97%B4%E5%86%85%E4%BB%8E%E6%9C%AA%E6%8E%92%E5%BA%8F%E7%9A%84%E5%90%91%E9%87%8F%E4%B8%AD%E5%88%A0%E9%99%A4%E9%A1%B9.cpp)
```cpp
#include"print.h"
#include<vector>
#include<ranges>
namespace stdr = std::ranges;

//使用下标的版本
template<typename T>
void quick_delete(T& v, size_t idx) {
	if (idx < v.size()) {
		v[idx] = std::move(v.back());
		v.pop_back();
	}
}
//使用迭代器的版本
template<typename T>
void quick_delete(T& v, typename T::iterator it) {
	if (it < v.end()) {
		*it = std::move(v.back());
		v.pop_back();
	}
}
//若 vector 中项目的顺序不重要，就可以优化这个过程，使其花费 O(1)(常数) 时间
//做法很简单，将传入的要删除的迭代器或索引赋值为末尾元素的值，然后将末尾元素删除，就完成了，但是没有顺序

int main() {
	std::vector v{ 1,2,3,4,5 };
	print(v);
	auto it = stdr::find(v, 3);
	quick_delete(v, it);
	print(v);//顺序不对，正常现象

	quick_delete(v, 2);
	print(v);
}
```

### [3.5安全的访问vector元素](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/3.5%E5%AE%89%E5%85%A8%E7%9A%84%E8%AE%BF%E9%97%AEvector%E5%85%83%E7%B4%A0.cpp)
```cpp
#include"print.h"
#include<vector>

void test1() {
	std::vector v{ 1,2,3,4,5 };
	v[5] = 2001;//写入非法内存，访问也是越界
	auto& i = v[5];//引用了错误的内存
	print("{}\n", i);//可能发生错误，不保证
}

void test2()try {
	std::vector v{ 1,2,3,4,5 };
	auto& i = v.at(5);
	print("{}\n", i);
}
catch (std::exception& e) {
	print("{}\n", e.what());
}

void test3()try {
	std::vector v{ 1,2,3,4,5 };
	auto& i = v[5];
	print("{}\n", i);
}
catch (std::exception& e) {
	print("{}\n", e.what());
}
int main() {
	//test1();//error
	test2();
	//test3();//error
}
```

### [3.6保持vector元素的顺序](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/3.6%E4%BF%9D%E6%8C%81vector%E5%85%83%E7%B4%A0%E7%9A%84%E9%A1%BA%E5%BA%8F.cpp)
```cpp
#include"print.h"
#include<string>
#include<vector>
#include<list>
using Vstr = std::vector<std::string>;
namespace stdr = std::ranges;

void psorted(stdr::range auto&& v) {
	if  (stdr::is_sorted(v))
		print("sorted: ");
	else
		print("unsorted: ");
	print(v);
}

void insert_sorted(Vstr& v, const std::string& s) {
	//lower_bound() 算法查找不小于实参的第一个元素的迭代器
	const auto pos{ stdr::lower_bound(v,s) };
	v.insert(pos, s);//使用 lower_bound() 返回的迭代器在正确的位置插入一个元素
}

template<stdr::range C,typename E>
void insert_sorted(C& c, const E& e) {
	const auto pos{ stdr::lower_bound(c,e) };
	c.insert(pos, e);
}

int main() {
	std::vector<std::string> v{ "2","1","3"};
	psorted(v);//无序

	stdr::sort(v);
	psorted(v);//有序

	//v.emplace_back("0");
	//psorted(v);//无序

	::insert_sorted(v, "0");
	psorted(v);//有序，相比于普通插入的优势
	//用list测试改写泛型的版本
	std::list<int>list{ 1,2,3,4,5 };
	psorted(list);//有序
	::insert_sorted(list, 0);
	psorted(list);//有序
}
```

### [3.7高效的将元素插入到map中](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/3.7%E9%AB%98%E6%95%88%E7%9A%84%E5%B0%86%E5%85%83%E7%B4%A0%E6%8F%92%E5%85%A5%E5%88%B0map%E4%B8%AD.cpp)
```cpp
#include"print.h"

struct X {
	std::string s;
	X() { print("default construct\n"); }
	X(const char* s) :s{ s } { print("construct\n"); }
	X(const X&) { print("copy construct\n"); }
};
void printm(const std::map<int, X>& map) {
	for (const auto& [k, v] : map) {
		print("[ {}:{} ]", k, v.s);
	}
	print("\n");
}

int main() {
	std::map<int, X>map{};
	map[1] = "🐴";//两个构造的开销，有参和默认
	print("\n");
	//直接转发，只有一个有参构造的开销,这里使用try_emplace和emplace效果完全一样
	map.emplace(2,"🥵");
	map.emplace(3, "🤣");
	printm(map);
	print("\n");

	map.emplace(1, "乐");//添加一个具有重复键的元素
	map.try_emplace(1, "乐");
	printm(map);
}
//重复键元素的问题参见 https://gcc.gnu.org/bugzilla/show_bug.cgi?id=92300
```

### [3.8高效的修改map项的键值](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/3.8%E9%AB%98%E6%95%88%E7%9A%84%E4%BF%AE%E6%94%B9map%E9%A1%B9%E7%9A%84%E9%94%AE%E5%80%BC.cpp)
```cpp
#include"print.h"
#include<string>

template<typename M,typename K>
bool node_swap(M& m, K k1, K k2) {
	//extract 是更换 map 的键而不重分配的唯一方式
	auto node1{ m.extract(k1) };
	auto node2{ m.extract(k2) };
	if (node1.empty() || node2.empty())
		return false;
	std::swap(node1.key(), node2.key());
	m.insert(std::move(node1));
	m.insert(std::move(node2));
	return true;
}

int main() {
	std::map<uint32_t, std::string>maps{
		{1,"🐴"},{2,"🥵"},{3,"🤣"},{4,"🐭"},{5,"😘"}
	};
	print(maps);
	::node_swap(maps, 3, 5);
	print(maps);

	auto node = maps.extract(maps.begin());
	node.key() = 5;
	auto t =maps.insert(std::move(node));
	print(maps);
	if (!t.inserted) {
		print("插入失败 {}\n",t.position->second);
	}
}
```

### [3.9自定义键值的unordered_map](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/3.9%E8%87%AA%E5%AE%9A%E4%B9%89%E9%94%AE%E5%80%BC%E7%9A%84unordered_map.cpp)
```cpp
#include"print.h"
#include<string>
#include<unordered_map>

struct Coord {
	int x{};
	int y{};
};
auto operator==(const Coord& a, const Coord& b) {
	return a.x == b.x && a.y == b.y;
}
namespace std {
	template<>
	struct hash<Coord> {
		size_t operator()(const Coord&a)const {
			return static_cast<size_t>(a.x) + static_cast<size_t>(a.y);
		}
	};
}
template<class T, class T2>
inline void print(const std::unordered_map<T, T2>& map) {
	print("size: {} ", map.size());
	for (auto& [k, v] : map)print("{{{} {}}}:{} ", k.x, k.y, v);
	print("\n");
}
int main() {
	std::unordered_map<Coord, std::string>map{ {{1,1},"😘"},{{0,0},"🤣"} };
	print(map);
}
```

### [3.10使用set进行输入和筛选](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/3.10%E4%BD%BF%E7%94%A8set%E8%BF%9B%E8%A1%8C%E8%BE%93%E5%85%A5%E5%92%8C%E7%AD%9B%E9%80%89.cpp)
```cpp
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
```

### [3.11实现简单的RPN计算器与deque](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/3.11%E5%AE%9E%E7%8E%B0%E7%AE%80%E5%8D%95%E7%9A%84RPN%E8%AE%A1%E7%AE%97%E5%99%A8%E4%B8%8Edeque.cpp)
```cpp
#include"print.h"
#include<deque>
#include<string>

class RPN {
	std::deque<double>deq_{};
	constexpr static double zero_{ 0.0 };
	constexpr static double inf_{ std::numeric_limits<double>::infinity() };

	bool is_numeric(const std::string str) {
		for (const auto& i:str) {
			if (i != '.' && !std::isdigit(i))
				return false;
		}
		return true;
	}

	std::pair<double, double>pop_get2() {
		if (deq_.size() < 2)return { zero_,zero_ };
		double v1{ deq_.front() };
		deq_.pop_front();
		double v2{ deq_.front() };
		deq_.pop_front();
		return { v2,v1 };
	}

	double optor(const std::string op) {
		std::map<std::string, double(*)(double, double)>opmap{
			{"+",[](double l,double r) {return l + r; }},
			{"-",[](double l,double r) {return l - r; }},
			{"*",[](double l,double r) {return l * r; }},
			{"/",[](double l,double r) {return l / r; }},
			{"^",[](double l,double r) {return std::pow(l,r); }},
			{"%",[](double l,double r) {return std::fmod(l,r); }}
		};
		if (opmap.find(op) == opmap.end())return zero_;
		auto [l, r] = pop_get2();
		if (op == "/" && r == zero_)deq_.push_front(inf_);
		else deq_.push_front(opmap.at(op)(l, r));
		return deq_.front();
	}

public:
	double op(const std::string& s) {
		if (is_numeric(s)) {
			double v{ std::stod(s) };
			deq_.push_front(v);
			return v;
		}
		else return optor(s);
	}

	void clear() {
		deq_.clear();
	}

	std::string get_stack_string()const {
		std::string s{};
		for (const auto& v : deq_) {
			s += std::format("{} ", v);
		}
		return s;
	}
};

int main() {
	RPN rpn;
	for (std::string o{}; std::cin >> o;) {
		rpn.op(o);
		auto stack_str{ rpn.get_stack_string() };
		print("{}: {}\n", o, stack_str);
	}
}
```

### [3.12使用map的词频计数器](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/3.12%E4%BD%BF%E7%94%A8map%E7%9A%84%E8%AF%8D%E9%A2%91%E8%AE%A1%E6%95%B0%E5%99%A8.cpp)
```cpp
#include"print.h"
#include<ranges>
#include<regex>
#include<vector>

namespace stdr = std::ranges;
namespace regex_constants = std::regex_constants;
namespace bw { constexpr const char* re{ "(\\w+)" }; }

int main() {
	std::map<std::string, int>wordmap{};
	std::vector<std::pair<std::string, int>>wordvec{};
	std::regex word_re(bw::re);
	size_t total_words{};

	for (std::string s{}; std::cin >> s;) {
		auto words_begin{ std::sregex_iterator(s.begin(),s.end(),word_re) };
		auto words_end{ std::sregex_iterator() };
		for (auto r_it{ words_begin }; r_it != words_end; ++r_it) {
			std::smatch match{ *r_it };//字符串匹配类
			auto word_str{ match.str() };//得到输入的单词
			stdr::transform(word_str, word_str.begin(), [](uint8_t c) {return tolower(c); });//将字母全部大写
			auto [map_it, result] = wordmap.try_emplace(word_str, 0);//插入到map中，map的键不会有重复，自动去重
			auto& [w, count] = *map_it;
			++total_words;
			++count;//增加单词计数
		}
	}
	auto unique_words = wordmap.size();
	wordvec.reserve(unique_words);
	stdr::move(wordmap, std::back_inserter(wordvec));
	stdr::sort(wordvec, [](const auto& a, const auto& b) {
		return (a.second != b.second) ? (a.second > b.second) : (a.first < b.first);
	});

	print("unique word count: {}\n", total_words);//总共的单词个数
	print("unqiue word count: {}\n", unique_words);//去除重复之后的
	for (int limit{ 20 }; auto & [w, count]:wordvec) {
		print("{}: {}\n", count, w);
		//if (--limit == 0)break;
	}
}
```

### [3.13找出含有相应长句的vector](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/3.13%E6%89%BE%E5%87%BA%E5%90%AB%E6%9C%89%E7%9B%B8%E5%BA%94%E9%95%BF%E5%8F%A5%E7%9A%84vector.cpp)
```cpp
#include"print.h"
#include<string_view>
#include<vector>
#include<ranges>
namespace stdr = std::ranges;

bool is_eos(const std::string_view& str) {
	constexpr const char* end_punct{ ".!?" };
	for (auto c : str) {
		if (strchr(end_punct, c) != nullptr)
			return true;
	}
	return false;
}

int main() {
	std::vector<std::vector<std::string>>vv_sentences{ std::vector<std::string>{} };
	for (std::string s{}; std::cin >> s;) {
		vv_sentences.back().emplace_back(s);
		if (is_eos(s)) {
			vv_sentences.emplace_back(std::vector<std::string>{});
		}
	}

	if (vv_sentences.back().empty())vv_sentences.pop_back();
	stdr::sort(vv_sentences, [](const auto& l, const auto& r) {
		return l.size() > r.size();
	});

	for (const auto& v : vv_sentences) {
		size_t size = v.size();
		print("{}: ", size);
		for (const auto& s : v) {
			print("{} ", s);
		}
		print("\n");
	}
	print("\n");
}
```

### [3.14使用multimap制作待办事项](https://github.com/13870517674/Cpp20-STL-Cookbook-src/blob/master/src/3.14%E4%BD%BF%E7%94%A8multimap%E5%88%B6%E4%BD%9C%E5%BE%85%E5%8A%9E%E4%BA%8B%E9%A1%B9.cpp)
```cpp
#include"print.h"
#include<map>

int main() {
	std::multimap<int, std::string>todo{
		{1,"🤣"},
		{2,"🥵"},
		{3,"🐴"},
		{4,"😘"}
	};
	rprint(todo);
}
```
### 第三章总结
第三章内容较多，需要对STL容器有一定的了解，建议每一个demo都自己写完理解意义后再往下阅读。

---

<bar>