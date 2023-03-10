#include"print.h"
#include<chrono>
#include<iomanip>

using std::chrono::system_clock;
using std::chrono::steady_clock;
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using seconds = duration<double>;
using milliseconds = duration<double, std::milli>;
using microseconds = duration<double, std::micro>;
using fps24 = duration<unsigned long, std::ratio<1, 24>>;

constexpr uint64_t MAX_PRIME{ 0x1FFFF };
uint64_t count_primes() {
	constexpr auto is_prime = [](const uint64_t n) {
		for (uint64_t i{ 2 }; i < n / 2; ++i) {
			if (n % i == 0)return false;
		}
		return true;
	};
	uint64_t count{ 0 };
	uint64_t start{ 2 };
	uint64_t end{ MAX_PRIME };
	for (auto i{ start }; i < end; ++i) {
		if (is_prime(i))++count;
	}
	return count;
}

seconds timer(uint64_t(f)()) {
	auto t1{ steady_clock::now() };
	uint64_t count{ f() };
	auto t2{ steady_clock::now() };
	seconds secs{ t2 - t1 };
	print("there are {} primes in range\n", count);
	return secs;
}

int main() {
	auto t{ system_clock::now() };
	print("system_clock::now is {:%F %T}\n", t);
	std::time_t now_t = system_clock::to_time_t(t);
	std::cout << "system_lock::now is " << std::put_time(localtime(&now_t), "%F %H:%M:%S") << '\n';

	auto secs{ timer(count_primes) };
	print("time elapsed: {:.3f} seconds\n", secs.count());

	using fps24 = duration<unsigned long, std::ratio<1, 24>>;
	print("time elapsed: {:.3f} sec\n", secs.count());
	print("time elapsed: {:.3f} ms\n", milliseconds(secs).count());
	print("time elapsed: {:.3e} us\n", microseconds(secs).count());
	print("time elapsed: {} frames at 24 fps\n", floor<fps24>(secs).count());//其实差不多相当于前面sec的1/24，去除小数的
}