#include"print.h"
#include<vector>
#include<random>
#include<array>

int iround(const double& d) {
	return static_cast<int>(std::round(d));//四舍五入返回double，进行显式类型转换
}

int main() {
	constexpr size_t n_data{ 200000 };//数据大小
	constexpr size_t n_samples{ 500 };//样本容器大小
	constexpr int mean{ 0 };//均值
	constexpr size_t dev{ 3 };//方差
	
	std::random_device rd;//提供对硬件随机数生成器的访问
	std::mt19937 rng{ rd() };//随机数生成器
	std::normal_distribution<>dist{ mean,dev };//正态分布的两个参数 均值与方差
	std::array<int, n_data>data{};
	for (auto& e : data)e = iround(dist(rng));

	std::array<int, n_samples>samples{};
	std::sample(data.begin(), data.end(), samples.begin(), n_samples, rng);

	std::map<int, size_t> hist{};
	for (const int i : samples) ++hist[i];

	constexpr size_t scale{ 3 };
	print("{:>3} {:>5} {:<}/{}\n", "n", "count", "graph", "scale");
	for (const auto& [value, count] : hist) {
		print("{:>3} ({:>3}) {}\n", value, count, std::string(count , '*'));
	}
}