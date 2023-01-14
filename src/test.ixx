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