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
	
	auto nums_ = stdv::iota(1, 10);
	print(nums_);

	auto rnums = stdv::iota(1) | stdv::take(200);
	print(rnums);
}