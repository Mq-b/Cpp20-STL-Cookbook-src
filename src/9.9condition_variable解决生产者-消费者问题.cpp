#include"print.h"
#include<mutex>
#include<thread>
#include<chrono>
#include<deque>
using namespace std::chrono_literals;

constexpr size_t num_items{ 10 };
constexpr auto delay_time{ 200ms };
std::deque<size_t>q{};
std::mutex mtx{};//共用这一个锁
std::condition_variable cond{};
bool finished{};

void producer() {
	for (size_t i{}; i < num_items; ++i) {
		std::this_thread::sleep_for(delay_time);//如果不延时，生产者运行太快，有可能直接运行完，把标志位都设置成true了，那么消费者不会打印任何数据
		std::lock_guard x{ mtx };
		q.push_back(i);
		cond.notify_all();//唤醒
	}
	std::lock_guard x{ mtx };
	finished = true;
	cond.notify_all();
}
void consumer() {
	while (!finished) {
		std::unique_lock lck{ mtx };//lock()
		cond.wait(lck, [] {return !q.empty() || finished; });//unlock()，阻塞当前线程  唤醒之后会先上锁
		//while (!q.empty()) {
			std::cout << std::format("Got {} from the queue\n", q.front());
			q.pop_front();
		//}
	}
}

int main() {
	std::thread t1{ producer };
	std::thread t2{ consumer };
	t1.join();
	t2.join();
	std::cout << "finished!\n";
}