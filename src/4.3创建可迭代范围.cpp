#include"print.h"

template<class T>
class Seq {
	T _star{};
	T _end{};
public:
	Seq(T star, T end) :_star(star), _end(end) {}
	struct iterator {
		T value{};
		explicit iterator(T v) :value(v) {}
		iterator& operator++() {
			value++;
			return *this;
		}
		T operator*() {
			return value;
		}
		bool operator!=(const iterator& l) {
			return this->value != l.value;
		}
	};
	iterator begin() {
		return iterator{ _star };
	}
	iterator end() {
		return iterator{ _end };
	}
};

template<class T,size_t size>
struct X {
	T array[size]{};
	T* begin() {
		return array;
	}
	T* end() {
		return array + size;
	}
};

int main() {
	Seq<int>v{ 1,10 };
	for (auto i : v) {
		print("{} ", i);
	}
	print("\n");

	X<int, 10>x{ 1,2,3,4,5,6,7,8,9,10 };
	for (auto i : x) {
		print("{} ", i);
	}
}