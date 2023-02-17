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

### 1.4
### 1.5
### 1.6
### 1.7
### 1.8
### 1.9