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