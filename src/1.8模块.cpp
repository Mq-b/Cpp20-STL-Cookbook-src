import test;

int main() {
	/*int n[]{
#include"t.txt"
	};
	for (auto i : n) {
		std::cout << i << ' ';
	}*/

	std::cout << mylib::add(1, 2) << '\n';
	//mylib::print("*");
	t();
}

//模块: https://zh.cppreference.com/w/cpp/language/modules
//编译设置:add_executable (Test1 "src/1.8模块.cpp" "src/test.ixx" "src/test2.ixx")