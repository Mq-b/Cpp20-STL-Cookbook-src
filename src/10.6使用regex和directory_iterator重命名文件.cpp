#include"print.h"
#include<regex>
using dit = fs::directory_iterator;
using pat_v = std::vector<std::pair<std::regex, std::string>>;

std::string replace_str(std::string s, const pat_v& replacements) {
	for (const auto& [pattern, repl] : replacements){
		s = std::regex_replace(s, pattern, repl);//第一个参数是传入字符串，第二个是正则对象用于匹配，第三个是替换匹配上的字符串
	}
	return s;
}

int main(const int argc,const char**argv) {
	pat_v patterns{};
	if (argc < 3 || argc % 2 != 1) {
		fs::path cmdname{ fs::path(argv[0]).filename() };
		std::cout << std::format("usage: {} [regex replacement] ...\n", cmdname);
		return 1;
	}//argc默认就有一个，我们的要求是一个字符串后面跟一个替换，那么算上那个1就是必须是奇数，所以如果不满足就退出
	
	for (int i{ 1 }; i < argc; i += 2) {
		patterns.emplace_back(argv[i], argv[i + 1]);
	}

	for (const auto& entry : dit{ fs::current_path() }) {
		fs::path fpath{ entry.path() };
		std::string rname{replace_str(fpath.filename().string(),patterns)};
		if (fpath.filename().string() != rname) {
			fs::path rpath{ fpath };
			rpath.replace_filename(rname);//更改rpath的路径文件名

			fs::rename(fpath, rpath);//这个函数调用才是真正的更改文件名，它不修改传入的两个path对象，而是真正的修改文件名
			std::cout << std::format("{} -> {}\n", fpath.filename(), rpath.filename());//fpath是原始，rpath是更改之后
		}
	}
}
