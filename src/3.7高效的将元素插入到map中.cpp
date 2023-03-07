#include"print.h"

struct X {
	std::string s;
	X() { print("default construct\n"); }
	X(const char* s) :s{ s } { print("construct\n"); }
	X(const X&) { print("copy construct\n"); }
};
void printm(const std::map<int, X>& map) {
	for (const auto& [k, v] : map) {
		print("[ {}:{} ]", k, v.s);
	}
	print("\n");
}

int main() {
	std::map<int, X>map{};
	map[1] = "🐴";//两个构造的开销，有参和默认
	print("\n");
	//直接转发，只有一个有参构造的开销,这里使用try_emplace和emplace效果完全一样
	map.emplace(2,"🥵");
	map.emplace(3, "🤣");
	printm(map);
	print("\n");

	map.emplace(1, "乐 ");//添加一个具有重复键的元素
	map.try_emplace(1, "乐 ");
	printm(map);
}
//重复键元素的问题参见 https://gcc.gnu.org/bugzilla/show_bug.cgi?id=92300