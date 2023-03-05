#include"print.h"
#include<sstream>
#include<ostream>
#include<chrono>
using std::chrono::high_resolution_clock;
using std::chrono::duration;

void timer(auto(f)()->std::string) {
	auto t1 = high_resolution_clock::now();
	std::string s{ f() };
	auto t2 = high_resolution_clock::now();
	duration<double, std::milli>ms = t2 - t1;
	print("{}", s);
	print("duration: {} ms\n", ms.count());
}

std::string concat_string() {
	print("concat_string\n");
	std::string a{ "a" };
	std::string b{ "b" };
	long n{};
	while (++n) {
		std::string x{};
		x += a + ", " + b + "\n";
		if (n >= 10000000)return x;
	}
	return "error\n";
}

std::string append_string() {
	print("append_string\n");
	std::string a{ "a" };
	std::string b{ "b" };
	long n{};
	while (++n) {
		std::string x{};
		x.append(a);
		x.append(", ");
		x.append(b);
		x.append("\n");
		if (n >= 10000000)return x;
	}
	return "error\n";
}

std::string concat_ostringstream() {
	print("ostringstream\n");
	std::string a{ "a" };
	std::string b{ "b" };
	long n{};
	while (++n) {
		std::stringstream x{};
		x << a << ", " << b << "\n";
		if (n >= 10000000)return x.str();
	}
	return "error\n";
}

std::string concat_format() {
	print("append_format\n");
	std::string a{ "a" };
	std::string b{ "b" };
	long n{};
	while (++n) {
		std::string x{};
		x += std::format("{}, {}\n", a, b);
		if (n >= 10000000)return x;
	}
	return "error\n";
}

int main() {
	timer(append_string);
	timer(concat_string);
	timer(concat_ostringstream);
	timer(concat_format);
}