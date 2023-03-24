#include<filesystem>
#include"print.h"
#include<algorithm>
using de = fs::directory_entry;

std::string strlower(std::string s) {
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
char type_char(const fs::file_status& fstat) {
	if (fs::is_symlink(fstat))return 'l';
	else if (fs::is_directory(fstat))return 'd';
	else if (fs::is_character_file(fstat))return 'c';
	else if (fs::is_block_file(fstat))return 'b';
	else if (fs::is_fifo(fstat))return 'p';
	else if (fs::is_socket(fstat))return 's';
	else if (fs::is_other(fstat))return 'o';
	else if (fs::is_regular_file(fstat))return '-';
	return '?';
}
std::string rwx(const fs::perms& p) {
	using fs::perms;
	auto bit2char = [&](perms bit, char c) {
		return (p & bit) == perms::none ? '-' : c;
	};
	return { bit2char(perms::owner_read,'r'),
	bit2char(perms::owner_write,'w'),
	bit2char(perms::owner_exec,'x'),
	bit2char(perms::group_read,'r'),
	bit2char(perms::group_write,'w'),
	bit2char(perms::group_exec,'x'),
	bit2char(perms::others_read,'r'),
	bit2char(perms::others_write,'w'),
	bit2char(perms::others_exec,'x'),
	};
}
std::string size_string(const uintmax_t fsize) {
	constexpr const uintmax_t kilo{ 1024 };
	constexpr const uintmax_t mega{ kilo * 1024 };
	constexpr const uintmax_t giga{ mega * kilo };
	std::string s;
	if (fsize >= giga)return std::format("{}{}", fsize / giga, 'G');
	else if (fsize >= mega)return std::format("{}{}",  fsize  / mega, 'M');
	else if (fsize >= kilo)return std::format("{}{}", fsize / kilo, 'K');
	else return std::format("{}B", fsize);
}
std::string time_string(const fs::directory_entry& dir) {
	auto file_time{ dir.last_write_time() };
	auto t =std::chrono::current_zone()->to_local(std::chrono::clock_cast<std::chrono::system_clock>(file_time));
	auto str =std::format("{}", t);
	auto index = str.find('.');
	str.replace(index, str.size(), "");
	return str;
}
void print_dir(const de& dir) {
	using fs::perms;
	const auto fpath{ dir.path() };//获取路径
	const auto fstat{ dir.symlink_status() };//获取文件状态
	const auto fperm{ fstat.permissions() };//获取文件权限
	const uintmax_t fsize{ fs::is_regular_file(fstat) ? fs::file_size(fpath) : 0 };
	const auto fn{ fpath.filename() };
	const auto permstr{ type_char(fstat) + rwx(fperm) };
	const std::string timestr{ time_string(dir) };

	std::string suffix{};
	if (fs::is_symlink(fstat)) {
		suffix = "-> ";
		suffix += fs::read_symlink(fpath).string();
	}
	else if (fs::is_directory(fstat))suffix = "/";//如果是目录就加杠
	else if ((fperm & perms::owner_exec) != perms::none) {
		suffix = "*";//如果是可执行可查找文件就加*，权限参见owner_exec
	}
	std::cout<<std::format("{} {:>6} {} {}{}\n", permstr, size_string(fsize),timestr,fn, suffix);
}

int main() {
	//fs::create_directories("sandbox/subdir");
	//fs::create_directory_symlink("sandbox/subdir", "include");
	const fs::path fp{ "." };
	std::vector<de>entries{};
	if (fs::is_directory(fp)) {
		for (const auto& de : fs::directory_iterator{ fp }) {
			entries.emplace_back(de);
		}

		std::sort(entries.begin(), entries.end(),dircmp_lc);
		for (const auto& e : entries) {
			print_dir(e);
		}
	}
	else {
		std::cout<<std::format("{}\n", fp.filename());
	}
}