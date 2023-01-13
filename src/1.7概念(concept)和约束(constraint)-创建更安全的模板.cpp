#include<iostream>

template<std::integral T>
void f(T t){}

template<class T>
	requires std::integral<T> || std::is_pointer_v<T>
struct X {

};

template<class T>
	requires std::is_integral_v<T>
T n{};

template<class T>
concept love = std::is_integral_v<T> && (std::is_same_v<int, T> || std::is_same_v<uint32_t, T>);

void f2(love auto){}

int main() {
	f(1);
	f('*');
	//f(1.2);
	X<int>x;
	//X<double>x2;
	X<double*>x3;
	n<int> = 3;
	//n<double>;
	std::cout << n<int> << '\n';
	f2(1);
	f2(1u);
	//f2(1l);
}

//Requires表达式 https://zh.cppreference.com/w/cpp/language/requires
//约束与概念 https://zh.cppreference.com/w/cpp/language/constraints