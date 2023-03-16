#include"print.h"
#include<thread>
#include<chrono>

void sleepms(const unsigned ms) {
	using std::chrono::milliseconds;
	std::this_thread::sleep_for(milliseconds(ms));
}
void fthread(const int n) {
	print("this is t{}\n", n);
	for (size_t i{}; i < 5; ++i) {
		sleepms(100 * n);
		print("t{}: {}\n", n, i + 1);
	}
	print("Finishing t{}\n", n);
}

int main() {
	std::thread t1(fthread, 1);
	t1.join();

	std::thread t2(fthread, 2);
	t2.detach();
	//sleepms(2000);//确保线程分离后的线程有足够的运行时间,这里其实写不写都行，因为它不是最后一行代码，如果不写，就可以看到它和t3的竞争了

	std::jthread t3(fthread, 3);
	print("end of main()\n");
}