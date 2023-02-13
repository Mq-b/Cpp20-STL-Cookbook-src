#include"print.h"
#include<vector>
#include<functional>
#include<list>
#include<deque>

void hello() {
	print("hello\n");
}

struct Hello_ {
	void greeting() {
		print("hello\n");
	}
};

int main() {
	std::deque<int>d;
	std::list<int>l;
	std::vector<int>v;

	auto print_c = [](const auto& c) {
		for (const auto& i : c)print("{} ", i);
		print("\n");
	};
	auto push_c = [](auto& container) {
		return [&container](auto value) {
			container.push_back(value);
		};
	};
	const std::vector<std::function<void(int)>>consumers{ push_c(d),push_c(l),push_c(v) };
	//consumers[0](10);
	//print_c(d);
	for (auto& i : consumers) {
		for (size_t j = 0; j < 10; j++) {
			i(j);
		}
	}
	print_c(d);
	print_c(l);
	print_c(v);

	std::function f{ hello };
	f();
	Hello_ h;
	std::function<void(void)>ff{ std::bind(&Hello_::greeting,&h) };
	ff();
	std::bind(&Hello_::greeting, &h)();
}