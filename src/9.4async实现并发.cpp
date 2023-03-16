#include"print.h"
#include<chrono>
#include<thread>
#include<future>
#include<list>

using secs = std::chrono::duration<double>;

struct prime_time {
	secs dur{};//计算花的时间
	uint64_t count{};//多少个质数
};
prime_time count_prime(const uint64_t& max) {
	prime_time ret{};
	constexpr auto isprime = [](const uint64_t& n) {
		for (uint64_t i{ 2 }; i < n / 2; ++i) {
			if (n % i == 0)return false;
		}
		return true;
	};
	uint64_t start{ 2 };
	uint64_t end{ max };
	auto t1 = std::chrono::steady_clock::now();
	for (uint64_t i{ start }; i <= end; ++i) {
		if (isprime(i))++ret.count;
	}
	ret.dur = std::chrono::steady_clock::now() - t1;
	return ret;
}

void f(std::promise<int>value) {
	print("this is f()\n");
	value.set_value(47);
}

int main() {
	constexpr uint64_t MAX_PRIME{ 0x1FFFF };
	auto primes1 = std::async(count_prime, MAX_PRIME);
	auto pt = primes1.get();
	print("primes: {} {:.3}\n", pt.count, pt.dur);

	std::list<std::future<prime_time>>swarm;
	print("start parallel primes\n");
	auto t1{ std::chrono::steady_clock::now() };
	for (size_t i{}; i < 15; ++i) {
		swarm.emplace_back(std::async(std::launch::async, count_prime, MAX_PRIME));
	}
	for (size_t i{}; auto& f : swarm) {
		auto pt = f.get();
		print("primes({:02}): {} {:.5}\n", ++i, pt.count, pt.dur);
	}

	secs dur_total{ std::chrono::steady_clock::now() - t1 };
	print("total duration: {:.5}s\n", dur_total.count());

	std::promise<int>value_promise;
	std::future<int>value_future = value_promise.get_future();
	std::thread t2(f, std::move(value_promise));//不可以复制，只能移动，其实改一下函数的参数为引用，然后std::ref也行
	t2.detach();
	print("value is {}\n", value_future.get());

	print("end of main()\n");
}