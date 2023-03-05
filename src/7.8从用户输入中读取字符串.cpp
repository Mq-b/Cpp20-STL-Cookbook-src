#include"print.h"
#include<sstream>

bool prompt(std::string_view s, std::string_view s2 = "") {
	if (s2.size())std::cout<<format("{} ({}): ", s, s2);
	else std::cout<<format("{}: ", s);
	std::cout.flush();
	return true;
}
void clearistream() {
	std::string s{};
	std::cin.clear();
	std::getline(std::cin, s);
}
std::string trimstr(const std::string& s) {
	constexpr const char* whitespace{ " \t\r\n\v\f" };
	if (s.empty())return s;
	const auto first{ s.find_first_not_of(whitespace) };
	if (first == std::string::npos)return{};
	const auto last{ s.find_last_not_of(whitespace) };
	return s.substr(first, (last - first + 1));
}

int main() {
	constexpr size_t MAXLINE{ 1024 };
	char s[MAXLINE]{};
	std::string line{};
	std::string word{};
	const char* pl{ "Words here" };
	const char* pla{ "More words here" };
	const char* p2{ "Please enter two numbers" };
	const char* p3{ "Comma=separated words" };

	prompt(pl);
	std::cin.getline(s, MAXLINE, '\n');
	std::cout << s << '\n';

	prompt(pla, "pla");
	std::getline(std::cin, line, '\n');
	std::cout << line << '\n';

	double a{};
	double b{};
	for (prompt(p2); !(std::cin >> a >> b); prompt(p2)) {
		std::cout << "not numeric\n";
		clearistream();
	}
	std::cout << std::format("You netered {} and {}\n", a, b);
	std::cin.get();

	line.clear();
	prompt(p3);
	while (line.empty())std::getline(std::cin, line);
	std::stringstream ss(line);
	while (std::getline(ss, word, ',')) {
		if (word.empty())continue;
		std::cout << std::format("word: [{}]\n", trimstr(word));
	}
}