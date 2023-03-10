#include"print.h"
#include<tuple>
#include<chrono>

template<typename...T,size_t N = sizeof...(T)>
constexpr void print_t(const std::tuple<T...>& tup) {
	std::cout << "[";
	[&] <size_t...I>(std::index_sequence<I...>) {
		(..., print((I != N - 1 ? "{}, " : "{}]"), std::get<I>(tup)));
	}(std::make_index_sequence<N>());
	endl(std::cout);
}

template<typename... T>
constexpr int sum_t(const std::tuple<T...>& tup)requires (std::integral<T>&&...){
	int accum{};
	[&] <size_t...I>(std::index_sequence<I...>) {
		(..., (accum += std::get<I>(tup)));
	}(std::make_index_sequence<sizeof...(T)>());
	return accum;
}

int main() {
	std::tuple t{ 123,1.234,"🥵" };
	print_t(t);
	std::tuple t2{ 1,2,3,4,'a' };
	print("sum: {}\n", sum_t(t2));
}