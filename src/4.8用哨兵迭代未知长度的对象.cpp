#include"print.h"
#include<string>

class cstr_it {
	const char* s{};
	static constexpr const char nullchar = '\0';
public:
	explicit cstr_it(const char* str) :s{ str } {}
	char operator*()const { return *s; }
	cstr_it& operator++() {
		++s;
		return *this;
	}
	bool operator!=(const char)const {
		return s != nullptr && *s != nullchar;
	}
	cstr_it begin()const { return *this; }
	const char end()const { return nullchar; }
};

void print_cstr(const char* s) {
	print("{}: ", s);
	for (char c : cstr_it(s)) {
		print("{:02x} ", c);
	}
	print("\n");
}

int main() {
	const char carray[]{ "array" };
	print_cstr(carray);
	const char* cstr{ "c-string" };
	print_cstr(cstr);
}