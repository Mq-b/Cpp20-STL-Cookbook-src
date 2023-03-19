#include"print.h"
#include<thread>
#include<atomic>

std::atomic<bool>ready{};
std::atomic<uint64_t>g_count{};
std::atomic_flag winner{};
constexpr int max_count{ 1000 * 1000 };
constexpr int max_threads{ 100 };

struct Trivial {
	int a;
	int b;
};
std::atomic<Trivial>trival;

void countem(int id) {
	while (!ready)std::this_thread::yield();//让当前线程放弃CPU时间片，给其他线程
	for (int i{}; i < max_count; ++i)++g_count;
	if (!winner.test_and_set()) {
		std::cout << std::format("thread {:02} won!\n", id);
	}
}
std::string make_commas(const uint64_t& num) {
	std::string s{ std::to_string(num) };
	for (long l = s.length() - 3; l > 0; l -= 3) {
		s.insert(l, ",");
	}
	return s;
}

int main() {
	std::vector<std::thread>swarm;
	std::cout << std::format("spawn {} threads\n", max_threads);
	for (int i{}; i < max_threads; i++) {
		swarm.emplace_back(countem, i);
	}
	ready = true;//启动线程开始运行
	for (auto& t : swarm)t.join();
	std::cout << std::format("global count: {}\n", make_commas(g_count));

	std::cout << std::format("is g_count lock-free? {}\n", g_count.is_lock_free());
}