# 《C++20 STL Cookbook》2023

###  环境
**Visual Studio Enterprise 2022(64位) 版本 17.4.3**

**CMake 3.8**

###  [B站视频讲解](https://www.bilibili.com/video/BV1r8411N75b/?spm_id_from=333.999.0.0&vd_source=1992ca910d6cd0582931f6f985dc7fa0)

<br>

## 第一章 C++20的新特性
### [1.2格式化文本](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/1.2%E6%A0%BC%E5%BC%8F%E5%8C%96%E7%89%B9%E5%8C%96formatter.cpp)

 ```cpp
#include<iostream>
#include<string_view>
#include<format>

template < typename... Args>
void print(const std::string_view fmt_str, Args&&... args) {
	auto fmt_args{ std::make_format_args(args...) };
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
//特化规则参见:	https://zh.cppreference.com/w/cpp/named_req/Formatter
 ```

运行结果:

	1/10

你可以把这个内容分为两个部分:
1. 实现模板函数 **`print`**

使用与`std::format()`函数相同的参数。第一个参数是格式字符串的 `std::string_view` 对象，后面作为参数 的可变参数包。

[`std::make_format_args()`](https://zh.cppreference.com/w/cpp/utility/format/make_format_args) 函数的作用: *接受参数包并返回一个对象，该对象包含适合格式化的已擦除 类型的值。*

`fmt_str`就是传递的格式化字符串，`fmt_args`是一个保有格式化参数的对象，使用[`std::vformat(fmt_str, fmt_args)`](https://zh.cppreference.com/w/cpp/utility/format/vformat)即可返回格式化完毕的字符串。我们使用 `fputs()` 将值输出到控制台上 (这比 `cout` 高效得多)。

2. [**`std::formatter`**](https://zh.cppreference.com/w/cpp/utility/format/formatter) 特化

对于自定义，或者说标准没有对其有特化的类型，需要我们自行特化`std::formatter`才可以正确的格式化。

**`parse() `** 函数解析格式字符串，从冒号之后 (若没有冒号，则在开大括号之后) 直到但不包括结 束大括号 (就是指定对象类型的部分)。其接受一个 `ParseContext `对象，**并返回一个迭代器**。这里，可以只返回 `begin()` 迭代器。因为我们的类型不需要新语法，所以无需准备任何东西。 

**`format()`** 函数接受一个 `Frac` 对象和一个 `FormatContext` 对象，**返回结束迭代器**。**`format_to()`** 函数可使这变得很容易，**其可以接受一个迭代器、一个格式字符串和一个参数包**。本例中，参数包是 Frac 类的两个属性，分子和分母。 需要做的就是提供一个简单的格式字符串`“{0}/{1}”`以及分子和分母的值 (0 和 1 表示参数的 位置)。

<br>

### [1.3使用编译时constexpr `std::vector`和`std::string`](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/1.3%E4%BD%BF%E7%94%A8%E7%BC%96%E8%AF%91%E6%97%B6constexpr%20vector%E5%92%8Cstring.cpp)
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

```

`C++20` 允许在新的上下文中使用 [**`constexpr`**](https://zh.cppreference.com/w/cpp/language/constexpr)，这些语句可以在编译时计算，从而提高了效率(此关键字自`c++11`诞生，一直在增加和改进，我们不再强调)。

<br>

其中包括在 `constexpr` 上下文中使用 [`string`](https://zh.cppreference.com/w/cpp/string/basic_string) 和 [`vector`](https://zh.cppreference.com/w/cpp/container/vector) 对象的能力。所以 **，这些对象本身可能不声 明为 constexpr**，**但可以在编译时上下文中使用。**

```cpp
constexpr void f() {
	constexpr std::string s{ "乐" };
}//错误
constexpr void f() {
	std::string s{ "乐" };
}//正确
```

<br>

也可以在`constexpr`上下文中使用算法:

```cpp
constexpr int use_vector() {
	std::vector<int> vec{ 1, 2, 3, 4, 5 };
	return accumulate(begin(vec), end(vec), 0);
}

int main() {
	constexpr int ret = use_vector();
}
```

<br>

`C++20` 开始，标准 `string` 和 `vector` 类具有`constexpr`限定的构造函数和析构函数，这是可在编译时使用的
前提。所以，分配给 `string` 或 `vector` 对象的内存，也必须在编译时释放。

例如，`constexpr` 函数返回一个 `vector`，编译时不会出错(但是实测 **`gcc msvc clang`** 全部编译错误):
```cpp
constexpr auto f() {
	std::vector<int>v{ 1,2,3 };
	return v;
}

int main() {
	constexpr auto ret = f();//error
}
```

<br>

在编译期间分配和释放了 `vector` 对象，该对象在运行时不可用，理论上可以返回通过编译，实际不可，就算可，你返回了也没有
办法去使用。在运行时使用一些 `vector` 对象的适配 `constexpr` 的方法，比如 `size()`，它是`constexpr`限定的。

```cpp
constexpr auto f() {
	std::vector<int>v{ 1,2,3 };
	return v;
}
int main(){
	constexpr auto n = f();//√
}
```

<br>

### [1.4安全比较不同类型的整数`cmp_less`](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/1.4%E5%AE%89%E5%85%A8%E6%AF%94%E8%BE%83%E4%B8%8D%E5%90%8C%E7%B1%BB%E5%9E%8B%E7%9A%84%E6%95%B4%E6%95%B0cmp_less.cpp)

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

```
`C++20` 在 [**`utility`**](https://zh.cppreference.com/w/cpp/header/utility) 引入了一组[**比较函数**](https://zh.cppreference.com/w/cpp/utility/intcmp)，他们分别是：

- `std::cmp_equal`
- `std::cmp_not_equal`
- `std::cmp_less`
- `std::cmp_greater`
- `std::cmp_less_equal`
- `std::cmp_greater_equal`

<br>

如上述[代码](https://zh.cppreference.com/w/cpp/utility/intcmp#.E5.8F.AF.E8.83.BD.E7.9A.84.E5.AE.9E.E7.8E.B0)中的例子一样，它与内建比较运算符不同，**负有符号整数**与**无符号整数**的比较结果始终为小于，且不为等于。

<br>

### [1.5三路比较运算符](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/1.5%E4%B8%89%E8%B7%AF%E6%AF%94%E8%BE%83%E8%BF%90%E7%AE%97%E7%AC%A6.cpp)
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

三路比较运算符表达式的形式为`表达式1 <=> 表达式2`该表达式将返回一个对象

若`表达式1 < 表达式2`，则`(表达式1 <=> 表达式2) < 0`

若`表达式1 > 表达式2`，则`(表达式1 <=> 表达式2) > 0`

若`表达式1 == 表达式2`，则`(表达式1 <=> 表达式2) == 0`

每当`<` `>` `<=` `>=` `<=>`被比较且重载决议选择该重载时，`operator<=>`都会被调用

若`operator<=>`是默认版本且`operator==`完全没有被声明，则`operator==`将隐式采用默认版本
```cpp
struct C
{
    int num;
    auto operator<=>(const C& c)const = default;
};

int main()
{
    C c1{1};
    C c2{2};

    std::cout << std::boolalpha << (c1 == c2) << '\n';//隐式调用默认版本
}
```

<br>

### [1.6查找特性测试宏](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/1.6%E6%9F%A5%E6%89%BE%E7%89%B9%E6%80%A7%E6%B5%8B%E8%AF%95%E5%AE%8F.cpp)
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

<br>

### [1.7概念(`concept`)和约束(`constraint`)-创建更安全的模板](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/1.7%E6%A6%82%E5%BF%B5(concept)%E5%92%8C%E7%BA%A6%E6%9D%9F(constraint)-%E5%88%9B%E5%BB%BA%E6%9B%B4%E5%AE%89%E5%85%A8%E7%9A%84%E6%A8%A1%E6%9D%BF.cpp)
``` cpp
#include<iostream>

template<std::integral T>
void f(T t) {}

template<class T>
requires std::integral<T> || std::is_pointer_v<T>
struct X {};

template <class T>
requires std::is_integral_v<T>
T n{};

template <class T>
concept love = std::is_integral_v<T> && (std::is_same_v<int, T> || std::is_same_v<uint32_t, T>);

void f2(love auto){}

int main() {
	f(1);            // 1 是 int，约束满足
	f('*');          // '*' 是整数类型(Integer Type)之一，约束满足
	//f(1.2);
	X<int> x;        // int 满足两个约束的析取之一：std::integral<T>，约束满足
	//X<double>x2;
	X<double*> x3;   // double* 满足两个约束的析取之一：std::is_pointer_t<T>，约束满足
	n<int> = 3;
	//n<double>;
	std::cout << n<int> << '\n';
	f2(1);           // 满足合取 std::is_integral_v<T> 和 std::is_same_v<int, T>
	f2(1u);          // 满足合取 std::is_integral_v<T>，std::is_same_v<uint32_t, T>
	//f2(1l);
}

//Requires表达式 https://zh.cppreference.com/w/cpp/language/requires
//约束与概念 https://zh.cppreference.com/w/cpp/language/constraints
```
>*约束(Constraint)是对模板形参提出的一种要求，这种要求的具名集合被称为概念(Concept)。每一个概念均为[**谓词**](https://zh.cppreference.com/w/cpp/named_req/Predicate) ，且在**编译期求值**，并在用作约束时成为模板接口的一部分*。

作为  `C++20` 引入的四大新特性之一：`Concept` ，提出了一种比 *SFINAE* 更好的约束方法，它易于理解和编写，也能在出现问题时给出更可读的编译期报错。概念的定义形式如下：

> **_template_** < <font color=grey><i>模板形参列表</i></font> >
> **_concept_**?<font color=grey><i>概念名</i></font> <font color=grey><i>属性</i></font><font color=green>(可选)</font>?**=**?<font color=grey><i>约束表达式</i></font> **;**

在上述例子中，概念 `love` 的定义就是这样：
```cpp
template <class T>
concept love = std::is_integral_v<T> && (std::is_same_v<int, T> || std::is_same_v<uint32_t, T>);
```

其中，运算符 `&&` 构成两个约束的合取(*Conjunction*)，两个约束均满足时合取满足。`||` 构成的析取(*Disjunction*)则为两者之一满足则析取满足。
<br>
<br>
`requires` 关键字可用于进行多个约束的分开表达，约束之间的关系均为合取，分为以下多种情况：

- 简单约束
```cpp
// 1. 简单约束
template <typename T>
concept Addable = requires(T a, T b)
{
	a + b;    //编译器会检查该表达式是否 "合法"
}
```

- 类型约束
```cpp
template <typename T>
struct tmp
{
	using value = T;
};

template <typename T, typename = std::enable_if_t<std::is_same_v<T, V>>
struct test {};

template <typename T>
using Ref = T&;

template <typename T>
concept Cpt = requires
{
	typename T::value;    // 检查 T 是否存在成员 T::value
	typename X<T>         // 检查是否存在模板类 S 的特化 S<T>
	typename Ref<T>       // 检查是否存在合法别名模板 Ref<T>
}
```

- 复合约束
复合约束用于约束表达式的返回类型。其定义为：

> { <font color=grey><i>表达式</i></font> } **noexcept**<font color=green>(可选)</font> -> <font color=grey><i>类型约束</i></font> **;**

例如：
```cpp
template <typename T>
concept C = requires(T x) {
  {x * 2} -> typename T::inner;    // 表达式 x * 2 的类型可转换为 T::inner
  {x + 3} -> std::same_as<int>;    // 表达式 x + 3 需要满足约束 std::same_as<int>
};
```

<br>
<br>

复合约束的计算顺序为：
- 计算 <font color=grey><i>表达式</i></font> 是否合法
- 如果有 **noexcept** 限定，则 <font color=grey><i>表达式</i></font> 不能抛出异常
- `decltype((expression))` 的类型必须满足 <font color=grey><i>类型约束</i></font>
全满足则结果为 `true` ，否则为 `false`

<br>

### [1.8模块](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/1.8%E6%A8%A1%E5%9D%97.cpp)

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

[`test.ixx`](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/test.ixx)

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

[`test2.ixx`](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/test2.ixx)

```cpp
export module test2;
import<iostream>;

export void t() {
	std::cout << "乐\n";
}
```

[`t.txt`](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/t.txt)

```
1,2,3,4,5
```



### [1.9视图](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/1.9%E8%A7%86%E5%9B%BE.cpp)

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

	std::vector<std::string>strs{ "?","?","?","?" };
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
### [2.2span类](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/2.2span%E7%B1%BB.cpp)

```cpp
#include <iostream>
#include <format>
#include <span>
#include <vector>
#include <array>

template < typename... Args>
void print(const std::string_view fmt_str, Args&&... args) {
	auto fmt_args{ std::make_format_args(args...) };
	std::string outstr{ std::vformat(fmt_str, fmt_args) };
	fputs(outstr.c_str(), stdout);
}

template<class T>
void pspan(std::span<T> s) {
	print("number of elemnts: {}\n", s.size());//  返回序列中的元素个数
	print("size of span: {}\n", s.size_bytes());// 返回以字节表示的序列大小
	for (auto i : s) print("{} ", i);
	endl(std::cout);
}

int main() {
	int a[]{ 1, 2, 3, 4, 5, 6 };
	pspan<int>(a);

	std::endl(std::cout);
	std::vector<int> b{1, 2, 3, 4, 5 };
	pspan<int>(b);

	std::endl(std::cout);
	std::array<int, 4> c{ 1, 2, 3, 4 };
	pspan<int>(c);
}

//span文档: https://zh.cppreference.com/w/cpp/container/span
//span简单实现: https://github.com/13870517674/c-plus-plus/blob/master/src/lib/span.hpp
```

运行结果:

```cpp
number of elemnts: 6
size of span: 24
1 2 3 4 5 6
```

**std::span** 在C++20中被引入

**std::span** 给具有连续对象的序列提供了轻量级的视图，以更加安全的方式对其进行迭代和索引，比如std::array、 std::vector、原生数组和原生指针。

常用于去包裹原生数组，并提供了更加安全的一系列函数：如front()，begin(), size(), empty()等

经典的实现中只有两个成员：

```cpp
private:
	pointer _ptr;//指向元素的指针
	std::size_t _size;//元素个数
```

### [2.3结构化绑定](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/2.3%E7%BB%93%E6%9E%84%E5%8C%96%E7%BB%91%E5%AE%9A.cpp)
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

	std::tuple<int, double, std::string>tu{ 10,3.14,"?" };
	auto [t1, t2, t3] = tu;
	print("{} {} {}\n", t1, t2, t3);

	X x{ 1,5.2,"?" };
	auto [x1, x2, x3] = x;
	print("{} {} {}\n", x1, x2, x3);

	const std::array arr2{ 1,2 };
	//auto& [c_arr1, c_arr2] = arr2;
	//c_arr1 = 10;//error

	auto [f1, f2] = f();
	print("{} {}\n", f1, f2);

	std::map<int, std::string>Map{ {1,"*"},{2,"?"} };
	for (const auto& [m_a, m_b] : Map) {
		print("{} {}\n", m_a, m_b);
	}
}
```

### [2.4`if`&`switch`中的初始化](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/2.4if%26switch%E4%B8%AD%E7%9A%84%E5%88%9D%E5%A7%8B%E5%8C%96.cpp)
```cpp
#include"print.h"
#include<mutex>
#include<thread>

std::mutex m;
bool flag = true;

void f(int n) {
	if (std::lock_guard lg{ m }; flag) {
		print("乐\t");
		print("???\n");
	}
}

void t() {
	if (auto flag = [](int n) {return n * n; }(10); flag != 0) {
		print("???\n");
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

### [2.5模板参数推导](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/2.5%E6%A8%A1%E6%9D%BF%E5%8F%82%E6%95%B0%E6%8E%A8%E5%AF%BC.cpp)
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
	X x("10","?"s);
	print("{}\n", x.v);
}
```

### [2.6编译期`if`](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/2.6%E7%BC%96%E8%AF%91%E6%9C%9Fif.cpp)
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
	show(5,314, "?", '*');
	print("\n");
}
```
### 第二章总结
第二章内容总体比较简单，并没有什么困难的，重在运用，最好这些demo都自己抄或者写一遍。
加深理解

<br>

---
## 第三章 STL容器


### [3.1使用`set`进行输入和筛选](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/3.10%E4%BD%BF%E7%94%A8set%E8%BF%9B%E8%A1%8C%E8%BE%93%E5%85%A5%E5%92%8C%E7%AD%9B%E9%80%89.cpp)
```cpp
#include"print.h"
#include<set>
#include<string>
#include<ranges>

int main() {
	std::set<std::string> sets;//set 容器用于存储键适用于索引关键字

	std::copy(std::istream_iterator<std::string>{std::cin}, {},
		std::inserter(sets, sets.end()));
		
	print(sets);
}
```
[**``std::copy``**](https://zh.cppreference.com/w/cpp/algorithm/copy)  用于将数据拷贝到对应容器中
- 参数1 `_First` 需要拷贝的起始迭代器(这里使用``istream``的迭代器来读取输入流字符串)   
- 参数2 `_Last`  拷贝的截止迭代器 (这里使用 `{}` 占位 即拷贝所有输入流中的字符)   
- 参数2 `_Dest`  如何拷贝(这里使用``std::inserter``进行插入)   
 
[**``std::inserter``**](https://zh.cppreference.com/w/cpp/algorithm/copy)  将每一组输入的字符串作为 `key` 插入到容器中
- 参数1 `_Cont`	 需要插入数据的容器(这里是sets)   
- 参数2 `_Where` 需要插入的位置(这里始终插入到`sets`的尾部)   

**运行结果**

		输入:  1 12 3 3 3 3 3 ^Z
		输出:  size: 3 [ 1 12 3 ]
 
`set` 容器的 `key` 是不可重复的,如果需要运行重复 `key` 的 `set` 可以使用 `std::multiset`   

`set` 容器内部通过一颗 `R&B树(红黑树)`来存储数据,其对字符串的排序方式是按照 [**字典序**](https://baike.baidu.com/item/%E5%AD%97%E5%85%B8%E5%BA%8F#:~:text=%E5%9C%A8%E6%95%B0%E5%AD%A6%E4%B8%AD%EF%BC%8C%E5%AD%97%E5%85%B8%E6%88%96,%E9%A1%BA%E5%BA%8F%E6%8E%92%E5%88%97%E7%9A%84%E6%96%B9%E6%B3%95%E3%80%82)故输出时 `12` 出现在 `3` 之前

<br>

### [3.3使用擦除函数从容器中擦除项](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/3.3%E4%BD%BF%E7%94%A8%E6%93%A6%E9%99%A4%E5%87%BD%E6%95%B0%E4%BB%8E%E5%AE%B9%E5%99%A8%E4%B8%AD%E6%93%A6%E9%99%A4%E9%A1%B9.cpp)
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

	std::map<int, std::string> map{ {1,"?"},{2,"?"},{3,"?"},{4,"?"} };
	print(map);
	std::erase_if(map, [](auto& i) {
		const auto& [k, v] = i;
		return v == "?";
	});
	print(map);
}
```

### [3.4常数时间内从未排序的向量中删除项](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/3.4%E5%B8%B8%E6%95%B0%E6%97%B6%E9%97%B4%E5%86%85%E4%BB%8E%E6%9C%AA%E6%8E%92%E5%BA%8F%E7%9A%84%E5%90%91%E9%87%8F%E4%B8%AD%E5%88%A0%E9%99%A4%E9%A1%B9.cpp)
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

### [3.5安全的访问`std::vector`元素](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/3.5%E5%AE%89%E5%85%A8%E7%9A%84%E8%AE%BF%E9%97%AEvector%E5%85%83%E7%B4%A0.cpp)
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

### [3.6保持`std::vector`元素的顺序](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/3.6%E4%BF%9D%E6%8C%81vector%E5%85%83%E7%B4%A0%E7%9A%84%E9%A1%BA%E5%BA%8F.cpp)
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

### [3.7高效的将元素插入到`std::map`中](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/3.7%E9%AB%98%E6%95%88%E7%9A%84%E5%B0%86%E5%85%83%E7%B4%A0%E6%8F%92%E5%85%A5%E5%88%B0map%E4%B8%AD.cpp)
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
	map[1] = "?";//两个构造的开销，有参和默认
	print("\n");
	//直接转发，只有一个有参构造的开销,这里使用try_emplace和emplace效果完全一样
	map.emplace(2,"?");
	map.emplace(3, "?");
	printm(map);
	print("\n");

	map.emplace(1, "乐");//添加一个具有重复键的元素
	map.try_emplace(1, "乐");
	printm(map);
}
//重复键元素的问题参见 https://gcc.gnu.org/bugzilla/show_bug.cgi?id=92300
```

### [3.8高效的修改`std::map`项的键值](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/3.8%E9%AB%98%E6%95%88%E7%9A%84%E4%BF%AE%E6%94%B9map%E9%A1%B9%E7%9A%84%E9%94%AE%E5%80%BC.cpp)
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
		{1,"?"},{2,"?"},{3,"?"},{4,"?"},{5,"?"}
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

### [3.9自定义键值的`std::unordered_map`](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/3.9%E8%87%AA%E5%AE%9A%E4%B9%89%E9%94%AE%E5%80%BC%E7%9A%84unordered_map.cpp)
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
	std::unordered_map<Coord, std::string>map{ {{1,1},"?"},{{0,0},"?"} };
	print(map);
}
```

### [3.10使用`std::set`进行输入和筛选](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/3.10%E4%BD%BF%E7%94%A8set%E8%BF%9B%E8%A1%8C%E8%BE%93%E5%85%A5%E5%92%8C%E7%AD%9B%E9%80%89.cpp)
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

### [3.11实现简单的RPN计算器与`deque`](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/3.11%E5%AE%9E%E7%8E%B0%E7%AE%80%E5%8D%95%E7%9A%84RPN%E8%AE%A1%E7%AE%97%E5%99%A8%E4%B8%8Edeque.cpp)
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

### [3.12使用`std::map`的词频计数器](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/3.12%E4%BD%BF%E7%94%A8map%E7%9A%84%E8%AF%8D%E9%A2%91%E8%AE%A1%E6%95%B0%E5%99%A8.cpp)
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

### [3.13找出含有相应长句的`std::vector`](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/3.13%E6%89%BE%E5%87%BA%E5%90%AB%E6%9C%89%E7%9B%B8%E5%BA%94%E9%95%BF%E5%8F%A5%E7%9A%84vector.cpp)
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

### [3.14使用`std::multimap`制作待办事项](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/3.14%E4%BD%BF%E7%94%A8multimap%E5%88%B6%E4%BD%9C%E5%BE%85%E5%8A%9E%E4%BA%8B%E9%A1%B9.cpp)
```cpp
#include"print.h"
#include<map>

int main() {
	std::multimap<int, std::string>todo{
		{1,"?"},
		{2,"?"},
		{3,"?"},
		{4,"?"}
	};
	rprint(todo);
}
```
### 第三章总结
第三章内容较多，需要对STL容器有一定的了解，建议每一个demo都自己写完理解意义后再往下阅读。

---

<br>

## 第四章 兼容迭代器
### [4.3创建可迭代范围](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/4.3%E5%88%9B%E5%BB%BA%E5%8F%AF%E8%BF%AD%E4%BB%A3%E8%8C%83%E5%9B%B4.cpp)
```cpp
#include"print.h"

template<class T>
class Seq {
	T _star{};
	T _end{};
public:
	Seq(T star, T end) :_star(star), _end(end) {}
	struct iterator {
		T value{};
		explicit iterator(T v) :value(v) {}
		iterator& operator++() {
			value++;
			return *this;
		}
		T operator*() {
			return value;
		}
		bool operator!=(const iterator& l) {
			return this->value != l.value;
		}
	};
	iterator begin() {
		return iterator{ _star };
	}
	iterator end() {
		return iterator{ _end };
	}
};

template<class T,size_t size>
struct X {
	T array[size]{};
	T* begin() {
		return array;
	}
	T* end() {
		return array + size;
	}
};

int main() {
	Seq<int>v{ 1,10 };
	for (auto i : v) {
		print("{} ", i);
	}
	print("\n");

	X<int, 10>x{ 1,2,3,4,5,6,7,8,9,10 };
	for (auto i : x) {
		print("{} ", i);
	}
}
```

### [4.4使迭代器与STL迭代器特性兼容](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/4.4%E4%BD%BF%E8%BF%AD%E4%BB%A3%E5%99%A8%E4%B8%8ESTL%E8%BF%AD%E4%BB%A3%E5%99%A8%E7%89%B9%E6%80%A7%E5%85%BC%E5%AE%B9.cpp)
```cpp
#include"print.h"

template<class T>
class Seq {
	T _star{};
	T _end{};
public:
	Seq(T star, T end) :_star(star), _end(end) {}
	struct iterator {
		T value{};

		using value_type        = std::remove_cv_t<T>;
		using difference_type   = std::ptrdiff_t;
		using pointer           = const T*;
		using reference         = const T&;

		explicit iterator(T v=0) :value(v) {}
		iterator& operator++() {
			value++;
			return *this;
		}
		iterator operator++(int) {
			auto t{ *this };
			++ *this;
			return t;
		}
		T operator*()const {
			return value;
		}
		bool operator!=(const iterator& l)const noexcept{
			return this->value != l.value;
		}
		bool operator==(const iterator& l)const noexcept {
			return this->value == l.value;
		}
	};
	iterator begin()const {
		return iterator{ _star };
	}
	iterator end()const {
		return iterator{ _end };
	}
};

template<class T>
requires std::forward_iterator<typename T::iterator>
void printc(const T& r) {
	for (const auto& i : r) {
		print("{} ", i);
	}
	print("\n");
}

int main() {
	Seq<int>r{ 100,110 };
	//auto [min_it, max_it] = std::minmax_element(r.begin(), r.end());
	auto [min_it, max_it] = std::ranges::minmax_element(r);
	print("max:{} min:{}\n", max_it.value, min_it.value);
	printc(r);
	static_assert(std::ranges::forward_range<Seq<int>>);
}
```

### [4.5使用迭代器适配器填充STL容器](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/4.5%E4%BD%BF%E7%94%A8%E8%BF%AD%E4%BB%A3%E5%99%A8%E9%80%82%E9%85%8D%E5%99%A8%E5%A1%AB%E5%85%85STL%E5%AE%B9%E5%99%A8.cpp)
```cpp
#include"print.h"
#include<string>
#include<deque>
#include<algorithm>
#include<vector>
#include<sstream>

inline void printc(const std::ranges::range auto& v,std::string_view s="") {
	if (s.size())print("{}: ", s);
	print("size: {}  ", v.size());
	print("[ ");
	for (const auto& i : v)print("{} ", i);
	print("]\n");
}

int main() {
	std::deque<int>d1{ 1,2,3,4,5 };
	std::deque<int>d2(d1.size());
	std::copy(d1.begin(), d1.end(), d2.begin());
	printc(d2, "d2 after copy");

	std::copy(d1.begin(), d1.end(), std::back_inserter(d2));
	printc(d2, "d2 after back_inserter");

	std::deque<int>d3{ 47,73,114,138,54 };
	std::copy(d3.begin(), d3.end(), std::front_inserter(d2));
	printc(d2, "d2 after front_inserter");

	auto it2{ d2.begin() + 2 };
	std::copy(d1.begin(), d1.end(), std::inserter(d2, it2));
	printc(d2, "d2 after minddle insert");

	print("ostream_iterator:");
	std::copy(d1.begin(), d1.end(), std::ostream_iterator<int>{std::cout," "});
	print("\n");
	
	/*std::vector<std::string>vs{};
	std::copy(std::istream_iterator<std::string>(std::cin), {}, std::back_inserter(vs));
	printc(vs, "vs2");*/

	/*std::vector<int> V(std::istream_iterator<int>(std::cin), {});
	printc(V, "V");*/

	for (auto it = std::istream_iterator<std::string>(std::cin);
		it != std::istream_iterator<std::string>{}; ++it) {
		print("{} ", *it);
	}
}
```

### [4.6创建一个迭代器生成器](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/4.6%E5%88%9B%E5%BB%BA%E4%B8%80%E4%B8%AA%E8%BF%AD%E4%BB%A3%E5%99%A8%E7%94%9F%E6%88%90%E5%99%A8.cpp)
```cpp
#include"print.h"

class fib_generator {
	using fib_t = unsigned long;
	fib_t stop_{};
	fib_t count_{ 0 };
	fib_t a_{ 0 };
	fib_t b_{ 1 };
	constexpr void do_fib() {
		const fib_t old_d = b_;
		b_ += a_;
		a_ = old_d;
	}
public:
	using iterator_concept = std::forward_iterator_tag;
	using iterator_category = std::forward_iterator_tag;
	using value_type = std::remove_cv_t<fib_t>;
	using difference_type = std::ptrdiff_t;
	using pointer = const fib_t*;
	using reference = const fib_t&;

	explicit fib_generator(fib_t stop = 0) :stop_{ stop } {}

	fib_t operator*()const { return b_; }
	constexpr fib_generator& operator++() {
		do_fib();
		++count_;
		return *this;
	}
	fib_generator operator++(int) {
		auto tmp{ *this };
		++* this;
		return tmp;
	}
	bool operator==(const fib_generator& o)const {
		return count_ == o.count_;
	}
	const fib_generator& begin()const { return *this; }
	const fib_generator end()const {
		auto sentinel = fib_generator();
		sentinel.count_ = stop_;
		return sentinel;
	}
	fib_t size() { return stop_; }
};

int main() {
	printc(fib_generator(10));
	fib_generator fib(10);
	auto x = std::ranges::views::transform(fib, [](auto x) {return x * x; });
	printc(x, "squared:");
}
```

### [4.7反向迭代器的反向适配器](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/4.7%E5%8F%8D%E5%90%91%E8%BF%AD%E4%BB%A3%E5%99%A8%E7%9A%84%E5%8F%8D%E5%90%91%E9%80%82%E9%85%8D%E5%99%A8.cpp)
```cpp
#include"print.h"
#include<string>
#include<vector>

void printr(const auto& r, std::string_view s = "") {
	auto rbegin = std::rbegin(r);
	auto rend = std::rend(r);
	for (auto it = rbegin; it != rend; ++it) {
		print("{} ", *it);
	}
	print("\n");
}

int main() {
	int array[]{ 1,2,3,4,5 };
	printc(array, "c-array");
	auto it = std::begin(array);
	auto it_end = std::end(array);
	while (it != it_end) {
		print("{} ", *it++);
	}
	print("\n");

	auto it2 = std::rbegin(array);
	auto it_end2 = std::rend(array);
	while (it2 != it_end2) {
		print("{} ", *it2++);
	}
	print("\n");

	printr(array, "rev c-array");

	std::vector<int>v{ 1,2,3,4,5 };
	printc(v, "vector");
	printr(v, "rev vector");
}
```

### [4.8用哨兵迭代未知长度的对象](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/4.8%E7%94%A8%E5%93%A8%E5%85%B5%E8%BF%AD%E4%BB%A3%E6%9C%AA%E7%9F%A5%E9%95%BF%E5%BA%A6%E7%9A%84%E5%AF%B9%E8%B1%A1.cpp)
```cpp
#include"print.h"
#include<string>

class cstr_it {
	const char* s{};
	static constexpr const char nullchar = '\0';
public:
	explicit cstr_it(const char* str) :s{ str } {}
	char operator*()const { return *s; }
	cstr_it& operator++() {
		++s;
		return *this;
	}
	bool operator!=(const char)const {
		return s != nullptr && *s != nullchar;
	}
	cstr_it begin()const { return *this; }
	const char end()const { return nullchar; }
};

void print_cstr(const char* s) {
	print("{}: ", s);
	for (char c : cstr_it(s)) {
		print("{:02x} ", c);
	}
	print("\n");
}

int main() {
	const char carray[]{ "array" };
	print_cstr(carray);
	const char* cstr{ "c-string" };
	print_cstr(cstr);
}
```

### [4.9构建zip迭代器适配器](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/4.9%E6%9E%84%E5%BB%BAzip%E8%BF%AD%E4%BB%A3%E5%99%A8%E9%80%82%E9%85%8D%E5%99%A8.cpp)
```cpp
#include"print.h"
#include<vector>
#include<string>

template<typename T>
class zip_iterator {
	using val_t = typename T::value_type;
	using ret_t = std::pair<val_t, val_t>;
	using it_t = typename T::iterator;

	it_t ita_{};
	it_t itb_{};
	it_t ita_begin_{};
	it_t itb_begin_{};
	it_t ita_end_{};
	it_t itb_end_{};
	zip_iterator(it_t ita, it_t itb) :ita_{ ita }, itb_{ itb } {}//用作begin和end返回的迭代器构造函数

public:
	using value_type = std::pair<val_t, val_t>;
	using difference_type = long int;
	using pointer = const val_t*;
	using reference = const val_t&;

	zip_iterator(T& a, T& b) :
		ita_{ a.begin() }, itb_{ b.begin() }, ita_begin_{ ita_ }, itb_begin_{ itb_ }, ita_end_{ a.end() }, itb_end_{ b.end() } {}
	zip_iterator& operator++() {
		++ita_;
		++itb_;
		return *this;
	}
	bool operator==(const zip_iterator& o)const { return ita_ == o.ita_ || itb_ == o.itb_; }
	bool operator!=(const zip_iterator& o)const { return !operator==(o); }
	ret_t operator*()const {return { *ita_,*itb_ };}
	zip_iterator begin()const { return { ita_begin_,itb_begin_ }; }
	zip_iterator end()const { return { ita_end_,itb_end_ }; }
};

int main() {
	std::vector<std::string>vec_a{ "Bob","John","Joni","?" };
	std::vector<std::string>vec_b{ "Dylan","Williams","Mitchell" };

	printc(vec_a, "vec_a: ");
	printc(vec_b, "vec_b: ");

	print("zipped: ");
	for (const auto& [a, b] : zip_iterator{ vec_a, vec_b }) {
		print("[{}, {}] ", a, b);
	}
	print("\n");

	std::map<std::string, std::string>name_map{};
	for (auto [a, b] : zip_iterator{ vec_a,vec_b }) {//插入到map中
		name_map.emplace(a, b);
	}
	print(name_map);//打印

}
```

### [4.10创建随机访问迭代器](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/4.10%E5%88%9B%E5%BB%BA%E9%9A%8F%E6%9C%BA%E8%AE%BF%E9%97%AE%E8%BF%AD%E4%BB%A3%E5%99%A8.cpp)
```cpp
#include"print.h"
#include<memory>
#include<vector>

template<typename T>
class Container {

	class iterator {

		T* ptr_;
	public:
		using iterator_concept = std::contiguous_iterator_tag;
		using iterator_category = std::contiguous_iterator_tag;
		using value_type = std::remove_cv_t<T>;
		using difference_type = std::ptrdiff_t;
		using pointer = const T*;
		using reference = const T&;

		iterator(T* ptr_ = nullptr) :ptr_{ ptr_ } {}

		const auto operator<=>(const iterator& o)const {
			ptr_ <=> o.ptr_;
		}

		iterator operator+(size_t size)const {
			return { ptr_ + size };
		}

		friend const iterator operator+(size_t size, const iterator& o) {
			return { o.ptr_ + size };
		}

		const iterator operator-(size_t size)const {
			return{ ptr_ - size };
		}

		const size_t operator-(const iterator& o)const {
			return  ptr_ - o.ptr_ ;
		}

		iterator& operator++() {
			++ptr_;
			return *this;
		}

		iterator operator++(int) {
			auto tmp{ *this };
			++* this;
			return tmp;
		}

		iterator& operator--() {
			--ptr_;
			return *this;
		}

		iterator operator--(int) {
			auto tmp{ *this };
			--* this;
			return tmp;
		}

		iterator& operator+=(size_t size)const {
			ptr_ += size;
			return *this;
		}

		iterator& operator-=(size_t size)const {
			ptr_ -= size;
			return *this;
		}

		const reference operator[](size_t size)const {
			return ptr_[size];
		}

		const bool operator==(const iterator& o)const {
			return ptr_ == o.ptr_;
		}

		bool operator!=(const iterator& o)const {
			return ptr_ != o.ptr_;
		}

		reference operator*()const {
			return *ptr_;
		}

		T* operator->()const {
			return ptr_;
		}

	};

	size_t n_elements_{};
	std::unique_ptr<T[]>c_{};
public:
	Container(std::initializer_list<T>l) :n_elements_{ l.size() }, c_{ std::make_unique<T[]>(n_elements_) }
	{
		for (size_t index{}; auto e : l) {
			c_[index++] = e;
		}
	}

	Container(size_t sz) :n_elements_{ sz }, c_{ std::make_unique<T[]>(n_elements_) } {}

	size_t size()const {
		return n_elements_;
	}

	const T& operator[](size_t index)const {
		return c_[index];
	}

	const T& at(size_t index)const {
		return index < n_elements_ ? c_[index] : throw std::out_of_range{ "Container::at(): index out of range" };
	}

	bool empty() const {
		return (n_elements_ == 0);
	}

	iterator begin()const { return { c_.get() }; }
	iterator end()const { return { c_.get() + n_elements_ }; }
};

template<typename T>
Container(std::initializer_list<T>l) -> Container<T>;

struct X {
	int x;
};

int main() {
	Container v{1,2,3,4,5};
	for (const auto& i : v) {
		print("{} ", i);
	}
	print("\n");

	Container<X>v3{ {1},{2} };
	auto ret2 = v3.begin();
	ret2->x;
	print("{}\n", std::ranges::forward_range<Container<int>>);
	print("{}\n", std::ranges::range<Container<int>>);
	print("{}\n", std::ranges::viewable_range<Container<int>>);
	print("{}\n", std::bidirectional_iterator<Container<int>>);

	for (const auto& i : v | std::views::reverse) {
		print("{} ", i);
	}
	
}
```
### 第四章总结
关于这个迭代器的内容，书上这些demo总的来说还是可以的，值得慢慢看，最好都是自己照着写一遍就行。

---

<br>

## 第五章 lambda表达式
### [5.3用于作用域可重用代码](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/5.3%E7%94%A8%E4%BA%8E%E4%BD%9C%E7%94%A8%E5%9F%9F%E5%8F%AF%E9%87%8D%E7%94%A8%E4%BB%A3%E7%A0%81.cpp)
```cpp
#include"print.h"

int main() {
	auto one = []() {return "one"; };
	auto two = [] {return "two"; };
	print("{} {}\n", one(), two());

	auto p = [](auto f) {//泛型lambda，C++20之前只能使用这种方式
		print("{}\n", f()); 
	};
	p([] {return "乐"; });

	auto p2 = []<typename T>(T&& f) { print("{}\n", f()); };
	p2(one);
	p2(std::move(one));

	[] <typename T>(T&& f) { print("{}\n", f()); }(two);

	int num{};
	auto p3 = [num]()mutable {num++; };
	for (auto i = 0; i < 5; i++)p3();

	print("{}\n", num);

	auto p4 = [&]()mutable {num++; };
	print("{}\n", sizeof(p4));

	constexpr int n = []()constexpr {return 10 * 10; }();

	auto p5 = []()->int {return 10; };
}
```

### [5.4算法库中作为谓词](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/5.4%E7%AE%97%E6%B3%95%E5%BA%93%E4%B8%AD%E4%BD%9C%E4%B8%BA%E8%B0%93%E8%AF%8D.cpp)
```cpp
#include"print.h"
#include<vector>

bool is_div4(int i) {
	return i % 4 == 0;
}

struct is_div4_ {
	bool operator()(int i) {
		return i % 4 == 0;
	}
};

auto is_div_by(int divisor) {
	return [=](int i) {return i % divisor == 0; };
}

int main() {
	std::vector v{ 1,2,3,4,44,8,10 };
	auto count1 =std::count_if(v.begin(), v.end(), is_div4);
	auto count2 = std::count_if(v.begin(), v.end(), is_div4_{});
	print("{} {}\n", count1, count2);
	auto count3 = std::count_if(v.begin(), v.end(), [](int i) {return i % 4 == 0; });
	print("{}\n", count3);

	for (int i : {3, 4, 5}) {
		auto count = std::ranges::count_if(v, is_div_by(i));
		print("{} ", count);
	}

	//不带捕获的lambda表达式可以有转换函数，隐式转换到对应的函数指针
	int(*p)(int) = [](int a) {return a; };
	print("{}\n", p(10));
}
```

### [5.5与`std::function`一起作为多态包装器](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/5.5%E4%B8%8Efunction%E4%B8%80%E8%B5%B7%E4%BD%9C%E4%B8%BA%E5%A4%9A%E6%80%81%E5%8C%85%E8%A3%85%E5%99%A8.cpp)
```cpp
#include"print.h"
#include<vector>
#include<functional>
#include<list>
#include<deque>

void hello() {
	print("hello\n");
}

struct Hello_ {
	void greeting() {
		print("hello\n");
	}
};

int main() {
	std::deque<int>d;
	std::list<int>l;
	std::vector<int>v;

	auto print_c = [](const auto& c) {
		for (const auto& i : c)print("{} ", i);
		print("\n");
	};
	auto push_c = [](auto& container) {
		return [&container](auto value) {
			container.push_back(value);
		};
	};
	const std::vector<std::function<void(int)>>consumers{ push_c(d),push_c(l),push_c(v) };
	//consumers[0](10);
	//print_c(d);
	for (auto& i : consumers) {
		for (size_t j = 0; j < 10; j++) {
			i(j);
		}
	}
	print_c(d);
	print_c(l);
	print_c(v);

	std::function f{ hello };
	f();
	Hello_ h;
	std::function<void(void)>ff{ std::bind(&Hello_::greeting,&h) };
	ff();
	std::bind(&Hello_::greeting, &h)();
}
```

### [5.6用递归连接lambda](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/5.6%E7%94%A8%E9%80%92%E5%BD%92%E8%BF%9E%E6%8E%A5lambda.cpp)
```cpp
#include"print.h"
template <typename F, typename ...Ts>
auto concat(F t, Ts ...ts){
	if constexpr (sizeof...(ts) > 0) {
		return [=](auto ...parameters) {
			return t(concat(ts...)(parameters...));
		};
	}
	else {
		return t;
	}
}
int main() {
	auto twice = [](auto i) {return i * 2; };
	auto thrice = [](auto i) {return i * 3; };
	auto combined = concat(thrice, twice, std::plus<int>{});
	print("{} \n", combined(2, 3));
}
```
`concat`较为复杂，所以先看传入一个参数时的情况。
```cpp
auto combined = concat(std::plus<int>{});
print("{} \n", combined(2, 3));
```
因为只有一个参数`t`，`sizeof...(ts) > 0`求值结果为`flase`，所以函数直接返回传入的参数，即`plus<int>`对象。
`concat`返回值初始化`combined`，`combined(2, 3)`的结果是5。<br>
再来看传入两个参数时的情况。
```cpp
auto combined = concat(twice, std::plus<int>{});
```
为了便于理解，不妨手动实例化`concat`模板：
```cpp
//针对 concat(twice, std::plus<int>{}) 实例化后的伪代码，假设 twice_type 是 twice 的类型
int concat(twice_type t, std::plus<int> ts){
	if constexpr (1 > 0) {
		return [=](auto ...parameters) {
			return t(concat(ts)(parameters...));
		};
	}
	else {
		return t;
	}
}
```
这时`sizeof...(ts) > 0`求值为`true`，`concat`返回一个 lambda 表达式。这个 lambda 又返回了`twice`
和`plus<int>`复合调用。这是因为`concat(std::plus<int>{})`正是一个参数时的情况，返回值是`std::plus<int>{}`。
`combined`被这个由`twice`和`plus<int>`复合调用的lambda表达式初始化，`combined(2, 3)`结果是10。<br>
结合以上两个例子，可以知道`concat`的作用就是将多个函数(可调用对象)进行连接调用，后一个函数的返回值作为前一个函数的参数，
最后一个函数的参数是由新生成函数的参数传入。这类似于数学函数的复合<br>
$$ (f \circ g \circ h)(x) = f(g(h(x))) $$<br>
最后来看书中三个参数的情况。`concat`递归的复合`thrice`、`twice`、`std::plus<int>{}`三个可调用对象，
并用lambda包装后返回并初始化`combined`，所以`combined(2, 3)`的结果是30。<br>

<br>

### [5.7将谓词与逻辑连接词连接起来](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/5.7%E5%B0%86%E8%B0%93%E8%AF%8D%E4%B8%8E%E9%80%BB%E8%BE%91%E8%BF%9E%E6%8E%A5%E8%AF%8D%E8%BF%9E%E6%8E%A5%E8%B5%B7%E6%9D%A5.cpp)
```cpp
#include"print.h"
#include <functional>

static bool begins_with_a(const std::string& s)
{
	return s.find("a") == 0;
}
static bool ends_with_b(const std::string& s)
{
	return s.rfind("b") == s.length() - 1;
}

template <typename A, typename B, typename F>
auto combine(F binary_func, A a, B b) {
	return [=](auto param) {
		return binary_func(a(param), b(param));
	};
}

int main() {
	auto a_xxx_b{ combine(std::logical_and<int>{},begins_with_a, ends_with_b) };

	std::copy_if(std::istream_iterator<std::string>{std::cin}, {},
		std::ostream_iterator<std::string>{std::cout, ", "}, a_xxx_b);
	std::cout << '\n';
}
```

### [5.8用相同的输入调用多个lambda](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/5.8%E7%94%A8%E7%9B%B8%E5%90%8C%E7%9A%84%E8%BE%93%E5%85%A5%E8%B0%83%E7%94%A8%E5%A4%9A%E4%B8%AAlambda.cpp)
```cpp
#include"print.h"

auto braces(const char a, const char b) {
	return [a, b](const auto v) {
		print("{}{}{} ", a, v, b);
	};
}

int main() {
	auto a = braces('(', ')');
	auto b = braces('[', ']');
	auto c = braces('{', '}');
	auto d = braces('|', '|');
	for (int i : {1, 2, 3, 4, 5}) {
		for (auto x : { a,b,c,d }) {
			x(i);
		}
		print("\n");
	}
}
```

### [5.9对跳转表使用映射lambda](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/5.9%E5%AF%B9%E8%B7%B3%E8%BD%AC%E8%A1%A8%E4%BD%BF%E7%94%A8%E6%98%A0%E5%B0%84lambda.cpp)
```cpp
#include"print.h"

const char prompt(const char* p) {
    std::string r;
    print("{} > ", p);
    std::getline(std::cin, r, '\n');

    if (r.size() < 1) return '\0';//如果走这个分支，就是直接下一个循环
    if (r.size() > 1) {
        print("响应时间过长\n");
        return '\0';
    }
    return toupper(r[0]);
}

int main() {
    using jumpfunc = void(*)();

    std::map<const char, jumpfunc> jumpmap{
        { 'A', [] { print("func A\n"); } },
        { 'B', [] { print("func B\n"); } },
        { 'C', [] { print("func C\n"); } },
        { 'D', [] { print("func D\n"); } },
        { 'X', [] { print("Bye!\n"); } }
    };

    char select{};
    while (select != 'X') {
        if ((select = prompt("select A/B/C/D/X"))) {
            auto it = jumpmap.find(select);
            if (it != jumpmap.end()) it->second();
            else print("没有对应的选项！\n");
        }
    }
}
```

### 第五章总结
关于lambda的一些概念，书上描述的是有问题的，不要被误导，视频也提到了。其他的一些demo什么的没问题，都写写。

---

<br>

## 第六章 STL算法
### [6.2基于迭代器的复制](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/6.2%E5%9F%BA%E4%BA%8E%E8%BF%AD%E4%BB%A3%E5%99%A8%E7%9A%84%E5%A4%8D%E5%88%B6.cpp)
```cpp
#include"print.h"
#include<vector>
namespace stdr = std::ranges;

int main() {
	std::vector<std::string>v1{ "alpha","beta","gamma","delta","epsilon" };
	printc(v1,"v1");
	std::vector<std::string>v2(v1.size());
	std::copy(v1.begin(), v1.end(), v2.begin());
	printc(v2, "v2");

	std::copy(v1.begin(), v1.end(), std::back_inserter(v2));
	printc(v2, "v2");

	std::vector<std::string>v3{};
	std::copy_n(v1.begin(), 3, std::back_inserter(v3));
	printc(v3, "v3");

	std::vector<std::string>v4{};
	/*std::copy_if(v1.begin(), v1.end(), std::back_inserter(v4), [](auto& s) {
		return s.size() > 4;
	});*/
	stdr::copy_if(v1,std::back_inserter(v4), [](auto& s) {
		return s.size() > 4;
		});
	printc(v4, "v4");

	stdr::copy(v1, std::ostream_iterator<std::string>{std::cout, " "});
	print("\n");

	stdr::move(v1, v2.begin());
	printc(v1, "after move: v1");
	printc(v2, "after move: v2");
}
```

### [6.3将容器元素连接到以供字符串当中](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/6.3%E5%B0%86%E5%AE%B9%E5%99%A8%E5%85%83%E7%B4%A0%E8%BF%9E%E6%8E%A5%E5%88%B0%E4%BB%A5%E4%BE%9B%E5%AD%97%E7%AC%A6%E4%B8%B2%E5%BD%93%E4%B8%AD.cpp)
```cpp
#include"print.h"
#include<vector>
#include<sstream>
#include<list>
#include<numbers>

namespace bw {
	template<typename T>
	std::ostream& join(T it, T end_it, std::ostream& o, std::string_view sep = "") {
		if (it != end_it)o << *it++;
		while (it != end_it)o << sep << *it++;
		return o;
	}

	template<typename I>
	std::string join(I it, I end_it, std::string_view sep = "") {
		std::ostringstream ostr;
		join(it, end_it, ostr, sep);
		return ostr.str();
	}

	std::string join(const auto& c, std::string_view sep = "") {
		return join(std::begin(c), std::end(c), sep);
	}
}

int main() {
	std::vector<std::string>greek{ "alpha","beta","gamma",
		"delta","epsilon" };
	for (const auto& c : greek) std::cout << c << ",";
	print("\n");
	auto greek_view = greek | std::views::join;
	for (const auto& c : greek_view) std::cout << c;
	print("\n");

	bw::join(greek.begin(), greek.end(), std::cout, ", ") << '\n';

	auto s = bw::join(greek.begin(), greek.end(), ", ");
	print("{}\n", s);

	auto s2 = bw::join(greek, ", ");
	print("{}\n", s2);

	std::list<double>list{ std::numbers::pi,std::numbers::e,std::numbers::sqrt2 };
	print("{}\n", bw::join(list, ": "));
}
```

### [6.4`std::sort`排序容器元素](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/6.4sort%E6%8E%92%E5%BA%8F%E5%AE%B9%E5%99%A8%E5%85%83%E7%B4%A0.cpp)
```cpp
#include"print.h"
#include<vector>
#include<random>

void check_sorted(auto& c) {
	if (!std::is_sorted(c.begin(), c.end()))print("un");
	print("sorted: ");
}

void printc_(const auto& c) {
	check_sorted(c);
	for (auto& e : c)print("{} ", e);
	print("\n");
}

void randomize(auto& c) {
	static std::random_device rd;
	static std::default_random_engine rng(rd());
	std::shuffle(c.begin(), c.end(), rng);
}

struct things {
	std::string s_;
	int i_;
	std::string str()const {
		return std::format("({}, {})", s_, i_);
	}
};

void print_things(const auto& c) {
	for (auto& v : c)print("{} ", v.str());
	print("\n");
}

int main() {
	std::vector<int>v{ 1,2,3,4,5,6,7,8,9,10 };
	printc_(v);

	for (int i{ 3 }; i; i--) {
		randomize(v);
		printc_(v);
	}
	std::sort(v.begin(), v.end());
	printc_(v);

	print("partial_sort:\n");
	randomize(v);
	auto middle{ v.begin() + (v.size() / 2) };
	std::partial_sort(v.begin(), middle, v.end());
	printc_(v);

	std::partition(v.begin(), v.end(), [](int i) {return i > 5; });
	printc_(v);

	std::vector<things>vthings{ {"?",1},{"?",2},{"?",3},{"?",4},{"?",5} };
	std::sort(vthings.begin(), vthings.end(),
		[](const things& lhs, const things& rhs) {
			return lhs.i_ > rhs.i_;
	});

	print_things(vthings);
}
```

### [6.5`std::transform`修改容器内容](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/6.5transform%E4%BF%AE%E6%94%B9%E5%AE%B9%E5%99%A8%E5%86%85%E5%AE%B9.cpp)
```cpp
#include"print.h"
#include<vector>

std::string str_lower(const std::string& s) {
	std::string outstr{};
	for (const char& c : s) {
		outstr += tolower(c);
	}
	return outstr;
}

int main() {
	std::vector<int>v1{ 1,2,3,4,5,6,7,8,9,10 };
	std::vector<int>v2;
	printc(v1, "v1");
	std::transform(v1.begin(), v1.end(), std::back_inserter(v2), [](int x) {return x * x; });
	printc(v2, "v2");

	std::vector<std::string>vstr1{ "Aaa","Bbb","Ccc","DDD" };
	std::vector<std::string>vstr2;
	printc(vstr1, "vstr1");
	print("str_lower:\n");
	std::transform(vstr1.begin(), vstr1.end(), std::back_inserter(vstr2),
		[](std::string& x) {return str_lower(x); });
	printc(vstr2, "vstr2");

	print("ranges sequares:\n");
	auto view1 = std::views::transform(v1, [](int x) {return x * x; });
	printc(view1, "view1");

	v2.clear();
	std::ranges::transform(v1, std::back_inserter(v2), [](int x) {return x * x; });
	printc(v2, "v2");
}
```

<br>

### [6.6查找特定项](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/6.6%E6%9F%A5%E6%89%BE%E7%89%B9%E5%AE%9A%E9%A1%B9.cpp)
```cpp
#include"print.h"
#include<vector>
#include<algorithm>

struct City {
	std::string name{};
	unsigned pop{};
	bool operator==(const City& o)const {
		return name == o.name;
	}
	std::string str()const {
		return std::format("[{}, {}]", name, pop);
	}
};

int main() {
	const std::vector<int>v{ 1,2,3,4,5,6,7,8,9,10 };

	auto it1 = std::find(v.begin(), v.end(), 7);
	if (it1 != v.end())print("found: {}\n", *it1);
	else print("not found:\n");

	const std::vector<City>c{
		{"London",8425622},
		{"Berlin",3566791},
		{"Tokyo",37435191},
		{"Cairo",20485965}
	};
	auto it2 = std::find(c.begin(), c.end(), City{ "Berlin" });
	if (it2 != c.end())print("found: {}\n", it2->str());
	else print("not found:\n");

	auto it3 = std::find_if(begin(c), end(c), [](const City& item) {
		return item.pop > 20000000;
	});
	if (it3 != c.end())print("found: {}\n", it3->str());
	else print("not found:\n");

	auto vwl = std::views::filter(c, [](const City& item) {
		return item.pop > 20000000;
	});
	for (const City& e : vwl)print("{}\n", e.str());
}
```

这个内容大概四个部分
1. 使用[**`std::find`**](https://zh.cppreference.com/w/cpp/algorithm/find)查找标量元素
2. 使用`std::find`查找自定义类型元素(需要重载`operator==`)
3. 使用 **`std::find_if`** 查找自定义类型符合谓词要求的元素
4. 使用 [**`std::views::filter`**](https://zh.cppreference.com/w/cpp/ranges/filter_view) 返回符合谓词要求的视图，可以像普通容器一样遍历

`std::find`或`std::find_if`的返回值是**迭代器**，如果没有查找到，则返回`end()`。

<br>

### [6.7将容器元素限制在`std::clamp`范围内](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/6.7%E5%B0%86%E5%AE%B9%E5%99%A8%E5%85%83%E7%B4%A0%E9%99%90%E5%88%B6%E5%9C%A8clamp%E8%8C%83%E5%9B%B4%E5%86%85.cpp)
```cpp
#include"print.h"
#include<vector>
#include<list>

constexpr int ilow{ 0 };
constexpr int ihigh{ 500 };

void printc_(const std::ranges::range auto& v, std::string_view s = "") {
	for (const auto& i : v)print("{:>5} ", i);
	print("\n");
}

int main() {
	auto il = { 0,-12,2001,4,5,-14,100,200 };
	std::vector<int>voi{ il };
	print("vector voi before:\n");
	printc_(voi);

	print("vector voi after:\n");
	for (auto& e : voi)e = std::clamp(e, ilow, ihigh);
	printc_(voi);

	print("list loi before:\n");
	std::list<int>loi{ il };
	printc_(loi);
	std::transform(loi.begin(), loi.end(), loi.begin(), [](auto e) {
		return std::clamp(e, ilow, ihigh);
	});
	print("list loi after:\n");
	printc_(loi);
}
```
运行结果:

	vector voi before:
	0   -12  2001     4     5   -14   100   200
	vector voi after:
	0     0   500     4     5     0   100   200
	list loi before:
	0   -12  2001     4     5   -14   100   200
	list loi after:
	0     0   500     4     5     0   100   200
[**`std::clamp`**](https://zh.cppreference.com/w/cpp/algorithm/clamp) 的作用非常单纯，就是限制元素的范围
```cpp
std::cout << std::clamp(-10, 0, 100) << '\n';//0
std::cout << std::clamp(200, 0, 100) << '\n';//100
```

<br>

### [6.8`std::sample`采集样本数据集](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/6.8sample%E9%87%87%E9%9B%86%E6%A0%B7%E6%9C%AC%E6%95%B0%E6%8D%AE%E9%9B%86.cpp)
```cpp
#include"print.h"
#include<vector>
#include<random>
#include<array>

int iround(const double& d) {
	return static_cast<int>(std::round(d));//四舍五入返回double，进行显式类型转换
}

int main() {
	constexpr size_t n_data{ 200000 };//数据大小
	constexpr size_t n_samples{ 500 };//样本容器大小
	constexpr int mean{ 0 };//均值
	constexpr size_t dev{ 3 };//方差
	
	std::random_device rd;//提供对硬件随机数生成器的访问
	std::mt19937 rng{ rd() };//随机数生成器
	std::normal_distribution<>dist{ mean,dev };//正态分布的两个参数 均值与方差
	std::array<int, n_data>data{};
	for (auto& e : data)e = iround(dist(rng));

	std::array<int, n_samples>samples{};
	std::sample(data.begin(), data.end(), samples.begin(), n_samples, rng);

	std::map<int, size_t> hist{};
	for (const int i : samples) ++hist[i];

	constexpr size_t scale{ 3 };
	print("{:>3} {:>5} {:<}/{}\n", "n", "count", "graph", "scale");
	for (const auto& [value, count] : hist) {
		print("{:>3} ({:>3}) {}\n", value, count, std::string(count , '*'));
	}
}
```

运行结果:

	  n count graph/scale
	-11 (  1) *
	-10 (  1) *
	 -9 (  1) *
	 -8 (  1) *
	 -7 (  5) *****
	 -6 (  9) *********
	 -5 ( 11) ***********
	 -4 ( 30) ******************************
	 -3 ( 35) ***********************************
	 -2 ( 62) **************************************************************
	 -1 ( 70) **********************************************************************
	  0 ( 56) ********************************************************
	  1 ( 60) ************************************************************
	  2 ( 59) ***********************************************************
	  3 ( 34) **********************************
	  4 ( 29) *****************************
	  5 ( 19) *******************
	  6 ( 10) **********
	  7 (  5) *****
	  8 (  2) **

这个内容我们慢慢看。首先看函数`iround`，它的作用十分简单，因为标准库没有给我们提供返回`int`的四舍五入的函数，所以自己稍微包装一下，在里面使用`static_cast<int>`转换后再返回，看着会舒服点。

进入`main`函数，我们定义了几个 **`constexpr`** 的常量，作用也都有注释:**数据大小、样本容器大小、均值、方差**。

下面两行代码就是之前在 **[6.4sort排序容器元素](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/6.4sort%E6%8E%92%E5%BA%8F%E5%AE%B9%E5%99%A8%E5%85%83%E7%B4%A0.cpp)** 提起过的内容，随机数。

[`std::normal_distribution<>dist{ mean,dev };`](https://zh.cppreference.com/w/cpp/numeric/random/normal_distribution)需要注意，这个正态分布算本节重点，接受两个参数**均值与方差**，你可以简单理解我我们这节使用的随机数将以正态分布的方式随机，并且我们会将它的值打印出来直方图。

后面自然而然的就是为`std::array`对象遍历元素赋值按照正态分布的随机值了。

然后我们就要开始抽取样本了: 此时就要再创造一个数组对象，然后使用[**`std::sample`**](https://zh.cppreference.com/w/cpp/algorithm/sample)算法，前面两个参数是被抽取样本的范围，第三个参数是写入样本的输出迭代器，第四个参数是要抽取样本数`n_samples`，第五个参数是用作随机源的随机数生成器`rng`

后面就是简简单单的遍历一下容器映射到`map`中，对其进行打印即可

<br>

### [6.9生成有序数据序列](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/6.9%E7%94%9F%E6%88%90%E6%9C%89%E5%BA%8F%E6%95%B0%E6%8D%AE%E5%BA%8F%E5%88%97.cpp)
```cpp
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
```

运行结果:

	[cat] [dog] [velociraptor]
	[cat] [velociraptor] [dog]
	[dog] [cat] [velociraptor]
	[dog] [velociraptor] [cat]
	[velociraptor] [cat] [dog]
	[velociraptor] [dog] [cat]
	[1] [2] [3]
	[1] [3] [2]
	[2] [1] [3]
	[2] [3] [1]
	[3] [1] [2]
	[3] [2] [1]
	从大到小:
	[3] [2] [1]

这个例子应该是非常的简短了，我们看到，先是创建了一个`std::vector<std::string>`的对象并且初始化，然后先对其进行默认的降序排序。

进入`do while`它的条件语句里是执行 [**`std::next_permutation`**](https://zh.cppreference.com/w/cpp/algorithm/next_permutation)，若新排列按字典序大于旧者则为 **true** 。若抵达最后重排并重置范围为首个排列则为 **false**。我们可以看数字的这个，它一开始是`1 2 3 `重新排列后是 `1 3 2 `按照字典序，肯定是大于旧的，所以返回`true`，循环会继续,直到`3 2 1`它是按照首个排列的，也就是从大到小，所以是 **false**，循环结束。

其实我们看最后一段代码可以发现，它直接打印了一次`[3] [2] [1]`就结束了，因为这三个元素的最后一个字符是字典顺序的最后一个

<br>

### [6.10合并已排序容器](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/6.9%E7%94%9F%E6%88%90%E6%9C%89%E5%BA%8F%E6%95%B0%E6%8D%AE%E5%BA%8F%E5%88%97.cpp)
```cpp
#include"print.h"
#include<vector>
#include<algorithm>

int main() {
	std::vector<std::string>vs1{ "dog","cat","veloiraptor" };
	std::vector<std::string>vs2{ "kirk","sulu","spock" };
	std::vector<std::string>dest{};
	printc(vs1, "vs1");
	printc(vs2, "vs2");

	std::ranges::sort(vs1);
	std::ranges::sort(vs2);
	printc(vs1, "vs1");
	printc(vs2, "vs2");

	std::merge(vs1.begin(), vs1.end(), vs2.begin(), vs2.end(), std::back_inserter(dest));
	printc(dest, "dest");
}

```

运行结果:

	vs1: [dog] [cat] [veloiraptor]
	vs2: [kirk] [sulu] [spock]
	vs1: [cat] [dog] [veloiraptor]
	vs2: [kirk] [spock] [sulu]
	dest: [cat] [dog] [kirk] [spock] [sulu] [veloiraptor]

[**`std::merge`**](https://zh.cppreference.com/w/cpp/algorithm/merge)算法接受两个已排序的序列，并创建第三个已合并并排序的序列

前面四个参数表示两个输入范围，第五个参数表示结果序列发送的输出迭代器

### 第六章总结
第六章的内容普遍比较简短简单，都是一些库的使用，如果你是第一次见，那最好还是自己写一下多用用

---

<br>

## 第七章 字符串、流和格式化

STL 字符串类是一个功能强大的全功能工具，用于存储、操作和显示基于字符的数据。在高级脚本语言中，可以找到的许多字符串相关的便利、快速和敏捷的功能。

**`std::string`** 类基于 [**`std::basic_string`**](https://zh.cppreference.com/w/cpp/string/basic_string)，这是一个连续的容器类，可以用字符类型实例化。其类签名是这样

```cpp
template <class _Elem, class _Traits = char_traits<_Elem>, class _Alloc = allocator<_Elem>>
class basic_string
```
`Trait` 和 `Allocator` 模板参数通常保留默认值。

`basic_string` 的底层存储是一个连续的 CharT 序列，可以通过 `data()` 成员函数访问:
```cpp
#include<string>
#include<iostream>

int main() {
	const std::basic_string<char>s{ "hello" };
	const char* sdata = s.data();
	for (size_t i = 0; i < s.size(); i++){
		std::cout << sdata[i] << ' ';
	}
	std::cout << '\n';
}
```

运行结果:

	h e l l o

`data()` 成员函数返回一个指向底层字符数组的 `CharT*`。从 C++11 起，data() 返回的数组以空结
束，使得 `data()` **等价于** `c_str()`。

`basic_string` 类包含许多在其他连续存储类中可以找到的方法，包括 `insert()`、`erase()`、`push_back()`、
`pop_back()` 等，这些方法可以操作底层的 CharT 数组。

`std::string` 是 `std::basic_string<char>` 类型的别名:

```cpp
using string  = basic_string<char, char_traits<char>, allocator<char>>;
```

<br>

- [轻量级字符串对象——`string_view`](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/7.3%E8%BD%BB%E9%87%8F%E5%AD%97%E7%AC%A6%E4%B8%B2%E5%AF%B9%E8%B1%A1string_view.cpp)
- [连接字符串]()
- [转换字符串]()
- [使用格式库格式化文本]()
- [删除字符串中的空白]()
- [从用户输入中读取字符串]()
- [统计文件中的单词数]()
- [使用文件输入初始化复杂结构体]()
- [使用 `char_traits` 定义一个字符串类]()
- [用正则表达式解析字符串]()

<br>

### [7.3轻量字符串对象`string_view`](https://github.com/Mq-b/Cpp20-STL-Cookbook-src/blob/master/src/7.3%E8%BD%BB%E9%87%8F%E5%AD%97%E7%AC%A6%E4%B8%B2%E5%AF%B9%E8%B1%A1string_view.cpp)
