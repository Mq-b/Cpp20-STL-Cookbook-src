#include"print.h"

int main() {
	auto one = []() {return "one"; };
	auto two = [] {return "two"; };
	print("{} {}\n", one(), two());

	auto p = [](auto f) {//泛型lambda，C++20之前只能使用这种方式
		print("{}\n", f()); 
	};
	p([] {return "乐 "; });

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