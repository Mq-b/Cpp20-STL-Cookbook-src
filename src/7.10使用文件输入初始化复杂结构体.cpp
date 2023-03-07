#include"print.h"
#include<fstream>

struct City {
	std::string name;
	unsigned long population;
	double latitude;
	double longitude;
};

std::istream& operator>>(std::istream& in, City& c) {
	in >> std::ws;
	std::getline(in, c.name);
	in >> c.population >> c.latitude >> c.longitude;
	return in;
}
std::string make_commas(const unsigned long num) {//把数字串中间添加逗号，三位一个逗号分隔
	std::string s{ std::to_string(num) };
	for (int l = s.length() - 3; l > 0; l -= 3)
		s.insert(l, ",");
	return s;
}

int main() {
	constexpr const char* fn{ "E:/自制视频教程/《C++20 STL Cookbook》2023/src/src/cities.txt" };
	std::vector<City>cities;
	std::ifstream infile(fn, std::ios_base::in);
	if (!infile.is_open()) {
		print("failed to open file {}\n", fn);
		return 1;
	}
	for (City c{}; infile >> c;)cities.emplace_back(c);

	for (const auto& [name, pop, lat, lon] : cities) {
		print("{:.<15} pop {:<15} coords {}, {}\n", name, make_commas(pop), lat, lon);
	}
}