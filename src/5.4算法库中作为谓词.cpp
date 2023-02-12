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