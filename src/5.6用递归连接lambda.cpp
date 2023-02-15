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