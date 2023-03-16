#include"print.h"
#include<chrono>
using namespace std::this_thread;
using namespace std::chrono_literals;

int main() {
	auto t1 = std::chrono::steady_clock::now();
	//print("sleep for 1.3 seconds\n");
	sleep_for(1s + 300ms);
	//print("sleep for 2 seconds\n");
	sleep_until(std::chrono::steady_clock::now() + 2s);//阻塞当前线程，直至抵达指定的 sleep_time，和前面的sleep_for不一样
	std::chrono::duration<double>dur1 = std::chrono::steady_clock::now() - t1;
	print("total durtaion: {:.5}s\n", dur1.count());
}