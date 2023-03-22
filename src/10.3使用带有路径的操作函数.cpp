#include<filesystem>
#include"print.h"
#include<chrono>

int main() {
	fs::path p = "E:cl.exe";
	print("current_path: {}\n", fs::current_path());
	print("absolute(p): {}\n", fs::absolute(p));
	print("append: {}\n", fs::path{ "tetdir" } /= "foo.txt");
	print("canoical: {}\n", fs::canonical(fs::path{ "." }/="1..txt"));
	print("equivalent: {}\n", fs::equivalent("1..txt", "E:/自制视频教程/《C++20 STL Cookbook》2023/src/bin/Debug/1..txt"));

	try {
		fs::path p{ "1..txt" };
		print("p: {}\n", p);
		(void)fs::equivalent("1..txt", "debug/1.txt");
	}
	catch (const fs::filesystem_error& e) {
		print("{}\n", e.what());
		print("parth1: {}\n", e.path1());
		print("parth2: {}\n", e.path2());
	}

	fs::path p2{ "1..txt" };
	std::error_code e;
	print("canonical: {}\n", fs::canonical(p2 / "foo", e));
	print("error: {}\n", e.message());//打印错误
}