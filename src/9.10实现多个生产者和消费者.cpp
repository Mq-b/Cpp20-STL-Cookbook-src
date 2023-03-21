#include"print.h"
#include<memory>
#include<mutex>
#include<chrono>
#include<queue>
#include<list>
#include<future>
using namespace std::chrono_literals;

constexpr auto delay_time{ 100ms };
constexpr auto consumer_wait{ 100ms };//持续时间，和条件变量一起使用
constexpr size_t queue_limit{ 5 };//缓冲区限制，deque中最大容量值
constexpr size_t num_items{ 15 };//生产者生产的最大产品数量
constexpr size_t num_producers{ 3 };//生产者的数量
constexpr size_t num_consumers{ 5 };//消费者的数量
std::deque<std::string>qs{};//保存生成对象
std::mutex q_mutex{};//锁
std::condition_variable cv_producer{};//生产者条件变量
std::condition_variable cv_consumer{};//消费者条件变量
bool production_complete{};//标志位，所有生产者线程完毕的时候设置true

void producer(const size_t id) {
	for (size_t i{}; i < num_items; ++i) {
		std::this_thread::sleep_for(delay_time * id);
		std::unique_lock lock{ q_mutex };
		cv_producer.wait(lock, [&] {return qs.size() < queue_limit; });
		qs.push_back(std::format("pid {}, qs {}, item {:02}\n", id, qs.size(), i + 1));
		cv_consumer.notify_all();//生产完毕，唤醒消费者
	}
}
void consumer(const size_t id) {
	while (!production_complete) {
		std::unique_lock lock{ q_mutex };
		cv_consumer.wait_for(lock,consumer_wait, [&] {return !qs.empty(); });//如果不使用wait_for，而是wait，那么根据main函数，生产者会先生产完，然后结束，设置标志位，那么消费者的wait的lambda永远不满足要求，且阻塞，无唤醒
		if (!qs.empty()) {
			std::cout << std::format("cid {}: {}", id,qs.front());
			qs.pop_front();
		}
		cv_producer.notify_all();//消费完毕，唤醒生产者
	}
}

int main() {
	std::list<std::future<void>>producers;//生产者
	std::list<std::future<void>>consumers;//消费者
	for (size_t i{}; i < num_producers; ++i) {
		producers.emplace_back(std::async(std::launch::async, producer, i));
	}
	for (size_t i{}; i < num_consumers; i++) {
		consumers.emplace_back(std::async(std::launch::async, consumer, i));
	}

	for (auto& f : producers)f.wait();
	production_complete = true;
	/*qs.push_back("1");
	qs.push_back("2");
	qs.push_back("3");
	qs.push_back("4");
	qs.push_back("5");
	cv_consumer.notify_all();*///如果消费者不使用wait_for，那么生产者结束后后，消费者的wait可能导致阻塞，往共享资源插入足够的元素，也可让它成功退出
	//我们的消费者线程数是5，所以插入5个，最为保险，确保每个线程都能运行退出，而不会卡在wait
	
	std::cout << "producers done.\n";
	for (auto& f : consumers)f.wait();
	std::cout << "consumbers done.\n";
}