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