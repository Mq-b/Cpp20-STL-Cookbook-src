#include"print.h"
#include<map>

int main() {
	std::multimap<int, std::string>todo{
		{1,"🤣"},
		{2,"🥵"},
		{3,"🐴"},
		{4,"😘"}
	};
	rprint(todo);
}