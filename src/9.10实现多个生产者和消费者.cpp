#include"print.h"
#include<memory>
#include<mutex>
#include<chrono>
#include<queue>
using namespace std::chrono_literals;

constexpr auto delay_time{ 100ms };
constexpr auto consumer_wait{ 100ms };
constexpr size_t queue_limit{ 5 };
constexpr size_t num_items{ 15 };
constexpr size_t num_producers{ 3 };
constexpr size_t num_consumers{ 5 };
std::queue<std::string>qs{};
std::mutex q_mutex{};
std::condition_variable cv_producer{};
std::condition_variable cv_consumer{};
bool production_complete{};

void producer(const size_t id) {
	for (size_t i{}; i < num_items; ++i) {
		std::this_thread::sleep_for(delay_time * id);
		std::unique_lock lock{ q_mutex };
	}
}

int main() {

}