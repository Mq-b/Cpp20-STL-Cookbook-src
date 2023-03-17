#include"print.h"
#include<algorithm>
#include<execution>
#include<vector>
#include<chrono>
#include <random>

using dur_t = std::chrono::duration<double, std::milli>;
int main() {
	std::vector<unsigned>v(10 * 1000 * 1000);
	std::random_device rng;
	for (auto& i : v)i = rng() % 0xFFFF;
	auto mul2 = [](int n) {return n * 2; };

	auto t1 = std::chrono::steady_clock::now();
	std::transform(v.begin(), v.end(), v.begin(), mul2);
	dur_t dur1 = std::chrono::steady_clock::now() - t1;
	print("no policy: {:.3}ms\n", dur1.count());

	auto t2 = std::chrono::steady_clock::now();
	std::transform(std::execution::seq, v.begin(), v.end(), v.begin(), mul2);
	dur_t dur2 = std::chrono::steady_clock::now() - t2;
	print("std::execution::seq: {:.3}ms\n", dur2.count());

	auto t3 = std::chrono::steady_clock::now();
	std::transform(std::execution::par, v.begin(), v.end(), v.begin(), mul2);
	dur_t dur3 = std::chrono::steady_clock::now() - t3;
	print("std::execution::par: {:.3}ms\n", dur3.count());

	auto t4 = std::chrono::steady_clock::now();
	std::transform(std::execution::par_unseq, v.begin(), v.end(), v.begin(), mul2);
	dur_t dur4 = std::chrono::steady_clock::now() - t4;
	print("std::execution::par_unseq: {:.3}ms\n", dur4.count());

	auto t5 = std::chrono::steady_clock::now();
	std::sort(v.begin(), v.end());
	dur_t dur5 = std::chrono::steady_clock::now() - t5;
	print("sort: {:.3}ms\n", dur5.count());

	auto t6 = std::chrono::steady_clock::now();
	std::sort(std::execution::par,v.begin(), v.end());
	dur_t dur6 = std::chrono::steady_clock::now() - t6;
	print("sort: {:.3}ms\n", dur6.count());

	auto t7 = std::chrono::steady_clock::now();
	std::sort(std::execution::par_unseq, v.begin(), v.end());
	dur_t dur7 = std::chrono::steady_clock::now() - t7;
	print("sort: {:.3}ms\n", dur7.count());
}