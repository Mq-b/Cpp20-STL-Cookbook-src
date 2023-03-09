#include"print.h"
#include<any>
#include<list>
using namespace std::string_literals;

void p_any(const std::any& a) {
	if (!a.has_value()) {
		print("None.\n");
	}
	else if (a.type() == typeid(int)) {
		print("int: {}\n", std::any_cast<int>(a));
	}
	else if (a.type() == typeid(std::string)) {
		print("string: \"{}\"\n", std::any_cast<const std::string&>(a));
	}
	else if (a.type() == typeid(std::list<int>)) {
		print("list<int>: ");
		for (auto& i : std::any_cast<const std::list<int>&>(a)) {
			print("{} ", i);
		}
		print("\n");
	}
	else {
		print("something else: {}\n", a.type().name());
	}
}

int main() {
	std::any x{};
	if (x.has_value())print("have value\n");
	else print("no value\n");

	x = 42;
	if (x.has_value()) {
		print("x has type :{}\n", x.type().name());
		print("x has value: {}\n", std::any_cast<int>(x));
	}
	else {
		print("no value\n");
	}

	x = "abc"s;
	print("x is type {} with value {}\n", x.type().name(), std::any_cast<std::string>(x));

	p_any( {} );
	p_any(47);
	p_any("abc"s);
	p_any(std::list{ 1,2,3 });
	p_any(std::vector{ 1,2,3 });
	x.reset();
	p_any(x);
}