#include"print.h"

template<class T>
auto f(const T& v) {
	if constexpr (std::is_pointer_v<T>)
		print("pointer\n");
	else
		print("no pointer\n");
}

template<class T,class...Args>
void show(T t, Args&&...args) {
	print("{}\t",t);
	if constexpr (sizeof...(args)) {
		show(args...);
	}
}

int main() {
	int* p{};
	f(p);
	f(1);
	show(5,314, "🤣", '*');
	print("\n");
}