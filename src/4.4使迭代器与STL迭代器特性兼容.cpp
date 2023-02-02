#include"print.h"

template<class T>
class Seq {
	T _star{};
	T _end{};
public:
	Seq(T star, T end) :_star(star), _end(end) {}
	struct iterator {
		T value{};

		using value_type        = std::remove_cv_t<T>;
		using difference_type   = std::ptrdiff_t;
		using pointer           = const T*;
		using reference         = const T&;

		explicit iterator(T v=0) :value(v) {}
		iterator& operator++() {
			value++;
			return *this;
		}
		iterator operator++(int) {
			auto t{ *this };
			++ *this;
			return t;
		}
		T operator*()const {
			return value;
		}
		bool operator!=(const iterator& l)const noexcept{
			return this->value != l.value;
		}
		bool operator==(const iterator& l)const noexcept {
			return this->value == l.value;
		}
	};
	iterator begin()const {
		return iterator{ _star };
	}
	iterator end()const {
		return iterator{ _end };
	}
};

template<class T>
requires std::forward_iterator<typename T::iterator>
void printc(const T& r) {
	for (const auto& i : r) {
		print("{} ", i);
	}
	print("\n");
}

int main() {
	Seq<int>r{ 100,110 };
	//auto [min_it, max_it] = std::minmax_element(r.begin(), r.end());
	auto [min_it, max_it] = std::ranges::minmax_element(r);
	print("max:{} min:{}\n", max_it.value, min_it.value);
	printc(r);
	static_assert(std::ranges::forward_range<Seq<int>>);//静态断言，forward_range是向前迭代器concept
}