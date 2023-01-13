#include<iostream>

template<class T,class U>
constexpr bool cmp_less(T t, U u)noexcept {
	using UT = std::make_unsigned_t<T>;//有符号类型到无符号类型的安全转换。
	using UU = std::make_unsigned_t<U>;
	if constexpr (std::is_signed_v <T> == std::is_signed_v<U>)
		return t < u;
	else if constexpr (std::is_signed_v<T>)
		return t < 0 ? true : UT(t) < u;
	else
		return u < 0 ? false : t < UU(u);
}
int main() {
	std::cout << std::boolalpha << (5u < -1) << '\n';//true
	std::cout << std::boolalpha << ::cmp_less(5u, 1) << '\n';//false
	std::cout << std::boolalpha << ::cmp_less(5u, 2u) << '\n';//false
}

//整数比较函数: https://zh.cppreference.com/w/cpp/utility/intcmp