#include"print.h"
#include<mutex>
#include<thread>
#include<list>

constexpr size_t max_threads{ 25 };
std::once_flag init_flag;
void do_init(size_t id) {//只会被调用一次
	print("do_init ({}):", id);
}
void do_print(size_t id) {
	std::call_once(init_flag, do_init, id);
	print("{} ", id);
}

int main() {
	std::list<std::thread>spawn;
	for (size_t id = 0; id < max_threads; id++){
		spawn.emplace_back(do_print, id);
	}
	for (auto& t : spawn)t.join();
	std::cout << '\n';
}