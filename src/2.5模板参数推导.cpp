#include"print.h"

using namespace std::string_literals;

template<class T>
struct X {
	T v{};
	template<class...Args>
	X(Args&&...args) :v{ (args + ...) } {}
};

template<class...Ts>
X(Ts...ts) -> X<std::common_type_t<Ts...>>;

int main() {
	X x("10","🤣"s);
	print("{}\n", x.v);
}