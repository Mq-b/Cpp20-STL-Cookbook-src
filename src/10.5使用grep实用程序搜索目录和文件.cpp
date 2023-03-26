#include"print.h"
#include<regex>
#include<fstream>
using de = fs::directory_entry;
using rdit = fs::recursive_directory_iterator;
using rdit = fs::recursive_directory_iterator;
using match_v = std::vector<std::pair<size_t, std::string>>;

//从文件中获取正则表达式匹配
match_v matches(const fs::path& fpath, const std::regex& re) {
	match_v matches{};
	std::ifstream instrm{ fpath.string() ,std::ios_base::in};
	std::string s;
	for (size_t lineo{ 1 }; std::getline(instrm, s); ++lineo) {
		if (std::regex_search(s.begin(), s.end(), re)) {//要是正则和字符串匹配则返回true
			matches.emplace_back(lineo, s);//插入第几行和匹配的字符串
		}
	}
	return matches;
}
size_t pmatches(const std::regex& re, const fs::path epath, const fs::path& search_path) {
	fs::path target{ epath };
	auto regmatches{ matches(epath,re) };
	auto matchcount{ regmatches.size()};
	if (!matchcount)return 0;
	if (!(search_path == epath)) {
		target = epath.lexically_relative(search_path);//将target转换路径到相对路径
	}
	for (const auto& [lineno, line] : regmatches) {
		std::cout << std::format("{} {}: {}\n", target, lineno, line);
	}
	return regmatches.size();
}

int main(const int argc,const char**argv) {
	const char* arg_pat{};//命令行中的正则表达式模式
	std::regex re{};//正则表达式对象
	fs::path search_path{};//命令行搜索路径是参数
	size_t matchcount{};//计数匹配的行
	if (argc < 2) {
		auto cmdname{ fs::path(argv[0]).filename() };
		std::cout << cmdname << '\n';
		return 1;
	}

	arg_pat = argv[1];
	try {
		re = std::regex(arg_pat, std::regex_constants::icase);
	}catch (std::regex_error& e) {
		std::cout << std::format("{}: {}\n", e.what(), arg_pat);
		return 1;
	}

	if (argc == 2) {
		search_path = ".";
		for (const auto& entry : rdit{ search_path }) {
			const auto& epath{ entry.path() };
			matchcount += pmatches(re, epath, search_path);
		}
	}
	int count{ argc - 2 };
	while (count-- > 0) {
		fs::path p{ argv[count + 2] };
		if (!fs::exists(p)) {
			std::cout << std::format("not found: {}\n", p);
			continue;
		}
		if (fs::is_directory(p)) {
			for (const auto& entry : rdit{ p }) {
				const auto epath{ entry.path() };	
				matchcount += pmatches(re, epath, p);
			}
		}else {
			matchcount += pmatches(re, p, p);
		}
	}
	std::cout << std::format("found {} matches\n", matchcount);
}