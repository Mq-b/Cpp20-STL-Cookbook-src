#include"print.h"
#include<optional>

#if 0
factor_t factor(long n) {
	struct factor_t {
		bool is_prime;
		long factor;
	};
	factor_t r{};
	for (long i = 2; i <= n / 2; ++i) {
		if (n % i == 0) {
			r.is_prime = false;
			r.factor = i;
			return r;
		}
	}
	r.is_prime = true;
}
#endif

std::optional<long>factor(long n) {
	for (long i = 2; i <= n / 2; ++i) {
		if (n % i == 0)return { i };
	}
	return {};
}

std::optional<int> operator+(const std::optional<int>& a, const std::optional<int>& b) {
	if (a && b)return *a + *b;
	else return {};
}

std::optional<int> operator+(const std::optional<int>& a, const int b) {
	if (a)return *a + b;
	else return {};
}

int main() {
	long a{ 42 };
	long b{ 73 };
	auto x = factor(a);
	auto y = factor(b);
	if (x)print("lowest factor of {} is {} \n", a, *x);
	else print("{} is prime\n", a);
	if (y)print("lowest factor of {} is {} \n", a, *y);
	else print("{} is prime\n", b);

	std::optional<int>a2{ 42 };
	print("{}\n", *a2);
	if (a2)print("{}\n", *a2);
	else print("no value\n");

	{
		std::optional<int> a{ 42 };
		std::optional<int> b{ 73 };
		auto sum{ a + b };
		if (sum)print("{} + {} = {}\n", *a, *b, *sum);
		else print("NAN\n");
	}

	(void)a2.has_value();//判断是否有值
	(void)a2.value();//和*作用一样，取值
	a2.reset();//销毁值，重置可选对象状态

}