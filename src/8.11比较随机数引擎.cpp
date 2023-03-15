#include"print.h"
#include<random>
#include<vector>

constexpr size_t n_samples{ 1000 };//样本数量
constexpr size_t n_partitions{ 10 };//分区数量
constexpr size_t n_max{ 50 };//直方图图形条最大值

template<typename RNG>
void histogram(const std::string_view& rng_name) {
	constexpr auto p_ratio =(double)RNG::max() / n_partitions;
	RNG rng{};

	//收集样品
	std::vector<size_t>v(n_partitions);
	for (size_t i{}; i < n_samples; ++i) {
		++v[(size_t)(rng() / p_ratio)];
	}

	//显示直方图
	auto max_e1 = std::max_element(v.begin(), v.end());
	auto v_ratio = *max_e1 / n_max;//不能超过最大，所以除一下
	if (v_ratio < 1)v_ratio = 1;
	std::cout << std::format("engine: {}\n", rng_name);
	for (size_t i{}; i < n_partitions; ++i) {
		std::cout << std::format("{:02}:{:*<{}}\n", i + 1, ' ', v[i]/v_ratio);//除是为了保证一个比例
	}
	endl(std::cout);
}

int main() {

	histogram<std::random_device>("random_device");
	histogram<std::default_random_engine>("default_random_engine");

	histogram<std::minstd_rand0>("minstd_rand0");
	histogram<std::minstd_rand>("minstd_rand");

	histogram<std::mt19937>("mt19937");
	histogram<std::mt19937_64>("mt19937_64");

	histogram<std::ranlux24_base>("ranlux24_base");
	histogram<std::ranlux48_base>("ranlux48_base");

	histogram<std::ranlux24>("ranlux24");
	histogram<std::ranlux48>("ranlux48");

	histogram<std::knuth_b>("knuth_b");
}