#include"print.h"
using dit = fs::directory_iterator;
using de = fs::directory_entry;

std::string make_commas(const unsigned long num) {//把数字串中间添加逗号，三位一个逗号分隔
	std::string s{ std::to_string(num) };
	for (int l = s.length() - 3; l > 0; l -= 3)
		s.insert(l, ",");
	return s;
}
std::string strlower(std::string s) {//全部小写
	auto char_lower = [](const char& c)->char {
		if (c >= 'A' && c <= 'Z')return c + 32;
		else return c;
	};
	std::transform(s.begin(), s.end(), s.begin(), char_lower);
	return s;
}
bool dircmp_lc(const de& lhs, const de& rhs) {
	const auto lhstr{ lhs.path().string() };
	const auto rhstr{ rhs.path().string() };
	return strlower(lhstr) < strlower(rhstr);
}
std::string size_string(const uintmax_t fsize) {
	constexpr const uintmax_t kilo{ 1024 };
	constexpr const uintmax_t mega{ kilo * 1024 };
	constexpr const uintmax_t giga{ mega * kilo };
	std::string s;
	if (fsize >= giga)return std::format("{}{}", fsize / giga, 'G');
	else if (fsize >= mega)return std::format("{}{}", fsize / mega, 'M');
	else if (fsize >= kilo)return std::format("{}{}", fsize / kilo, 'K');
	else return std::format("{}B", fsize);
}
uintmax_t entry_size(const fs::path& p) {
	if (fs::is_regular_file(p))return fs::file_size(p);
	uintmax_t accum{};
	if (fs::is_directory(p) && !fs::is_symlink(p)) {
		for (auto& e : dit{ p }) {
			accum += entry_size(e.path());
		}
	}
	return accum;
}

int main(const int argc,const char**argv) {
	auto dir{ argc > 1 ? fs::path(argv[1]) : fs::current_path() };
	std::vector<de>entries{};
	uintmax_t accum{};//目录文件总字节大小
	if (!fs::exists(dir)) {
		std::cout << std::format("path {} does not exist\n", dir);
		return 1;
	}
	if (!fs::is_directory(dir)) {
		std::cout << std::format("{} is not a directory\n", dir);
		return 1;
	}
	std::cout << std::format("{}:\n", fs::absolute(dir));

	for (const auto& e : dit{ dir }) {
		entries.emplace_back(e.path());
	}
	std::sort(entries.begin(), entries.end(), dircmp_lc);

	for (const auto& e : entries) {
		fs::path p{ e };
		uintmax_t esize{ entry_size(p) };
		std::string dir_flag{};
		accum += esize;
		if (fs::is_directory(p) && !fs::is_symlink(p))dir_flag = " *";
		std::cout << std::format("{:>5} {} {}\n", size_string(esize), p.filename(), dir_flag);
	}
	std::cout << std::format("{:->25}\n", "");
	std::cout << std::format("total bytes: {} ({})\n", make_commas(accum), size_string(accum));
}