#include"print.h"
#include<random> 
#include<map>

constexpr size_t n_samples{ 10000 };
constexpr size_t n_max{ 50 };

void dist_histogram(auto distor, const std::string_view& dist_name) {
	std::random_device seeds;
	std::default_random_engine rng(seeds());//随机数引擎
	std::map<long, size_t>m;

	//创建直方图
	for (size_t i{}; i < n_samples; ++i) {
		++m[(long)distor(rng)];
	}
	//打印直方图
	auto max_elm_it = std::max_element(m.begin(), m.end(), [](const auto& a, const auto& b) {
		return a.second < b.second;
	});
	size_t max_elm = max_elm_it->second;
	size_t max_div = std::max(max_elm / n_max, size_t(1));//设置比例50
	std::cout << std::format("{}:\n", dist_name);
	for (const auto [randval, count] : m) {
		if (count < max_div)continue;
		std::cout << std::format("{:3}:{:*<{}}\n", randval, ' ', count / max_div);
	}
}

int main() {
    dist_histogram(std::uniform_int_distribution<int>{0, 9}, "uniform_int_distribution");
    dist_histogram(std::normal_distribution<double>{0.0, 2.0}, "normal_distribution");

    std::initializer_list<double> intervals{ 0, 5, 10, 30 };
    std::initializer_list<double> weights{ 0.2, 0.3, 0.5 };
    dist_histogram(std::piecewise_constant_distribution<double>{begin(intervals), end(intervals), begin(weights)}, "piecewise_constant_distribution");
    std::initializer_list<double> weights2{ 0, 1, 1, 0 };
    dist_histogram(std::piecewise_linear_distribution<double>{begin(intervals), end(intervals), begin(weights2)}, "piecewise_linear_distribution");

    dist_histogram(std::bernoulli_distribution{ 0.75 }, "bernoulli_distribution");
    dist_histogram(std::discrete_distribution<int>{ {1, 2, 4, 8}}, "discrete_distribution");
    dist_histogram(std::binomial_distribution<int>{10, 0.3}, "binomial_distribution");
    dist_histogram(std::negative_binomial_distribution<int>{10, 0.8}, "negative_binomial_distribution");
    dist_histogram(std::geometric_distribution<int>{0.4}, "geometric_distribution");
    dist_histogram(std::exponential_distribution<double>{0.4}, "exponential_distribution");
    dist_histogram(std::gamma_distribution<double>{1.5, 1.0}, "gamma_distribution");
    dist_histogram(std::weibull_distribution<double>{1.5, 1.0}, "weibull_distribution");
    dist_histogram(std::extreme_value_distribution<double>{0.0, 1.0}, "extreme_value_distribution");
    dist_histogram(std::lognormal_distribution<double>{0.5, 0.5}, "lognormal_distribution");
    dist_histogram(std::chi_squared_distribution<double>{1.0}, "chi_squared_distribution");
    dist_histogram(std::cauchy_distribution<double>{0.0, 0.1}, "cauchy_distribution");
    dist_histogram(std::fisher_f_distribution<double>{1.0, 1.0}, "fisher_f_distribution");
    dist_histogram(std::student_t_distribution<double>{1.0}, "student_t_distribution");
}