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