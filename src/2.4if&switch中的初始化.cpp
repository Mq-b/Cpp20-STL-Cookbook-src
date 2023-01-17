#include"print.h"
#include<mutex>
#include<thread>

std::mutex m;
bool flag = true;

void f(int n) {
	if (std::lock_guard lg{ m }; flag) {
		print("乐\t");
		print("🤣🤣🤣\n");
	}
}

void t() {
	if (auto flag = [](int n) {return n * n; }(10); flag != 0) {
		print("🐴🐴🐴\n");
	}
}

void t2() {
	switch (char c = getchar();c)
	{
	case 'a':
		print("a\n"); 
		break;
	case 'b':
		print("b\n");
		break;
	case 'c':
		print("c\n");
		break;
	case 'd':
		print("d\n");
		break;
	default:
		print("error\n");
		break;
	}
}

int main() {
	for (int i = 0; i < 10; i++) {
		std::jthread t{ f,0 };
		std::jthread t2{ f,0 };
	}
	t();
	t2();
}