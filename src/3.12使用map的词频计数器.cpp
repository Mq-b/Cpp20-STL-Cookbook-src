#include"print.h"
#include<ranges>
#include<regex>
#include<vector>

namespace stdr = std::ranges;
namespace regex_constants = std::regex_constants;

namespace bw { constexpr const char* re{ "(\\w+)" }; }

int main() {
	std::map<std::string, int>wordmap{};
	std::vector<std::pair<std::string, int>>wordvec{};
	std::regex word_re(bw::re);
	size_t total_words{};

	for (std::string s{}; std::cin >> s;) {
		auto words_begin{ std::sregex_iterator(s.begin(),s.end(),word_re) };
		auto words_end{ std::sregex_iterator() };
		for (auto r_it{ words_begin }; r_it != words_end; ++r_it) {
			std::smatch match{ *r_it };//字符串匹配类
			auto word_str{ match.str() };//得到输入的单词
			stdr::transform(word_str, word_str.begin(), [](uint8_t c) {return tolower(c); });//将字母全部大写
			auto [map_it, result] = wordmap.try_emplace(word_str, 0);//插入到map中，map的键不会有重复，自动去重
			auto& [w, count] = *map_it;
			++total_words;
			++count;//增加单词计数
		}
		auto unique_words = wordmap.size();
		wordvec.reserve(unique_words);
		stdr::move(wordmap, std::back_inserter(wordvec));
		stdr::sort(wordvec, [](const auto& a, const auto& b) {
			return (a.second != b.second) ? (a.second > b.second) : (a.first < b.first);
			});
		print("unique word count: {}\n", total_words);//总共的单词个数
		print("unqiue word count: {}\n", unique_words);//去除重复之后的

		for (int limit{ 20 }; auto & [w, count]:wordvec) {
			print("{}: {}\n", count, w);
			//if (--limit == 0)break;
		}
	}
}