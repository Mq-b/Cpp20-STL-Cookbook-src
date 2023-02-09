#include"print.h"
#include<memory>
#include<vector>

template<typename T>
class Container {

	class iterator {

		T* ptr_;
	public:
		using iterator_concept = std::contiguous_iterator_tag;
		using iterator_category = std::contiguous_iterator_tag;
		using value_type = std::remove_cv_t<T>;
		using difference_type = std::ptrdiff_t;
		using pointer = const T*;
		using reference = const T&;

		iterator(T* ptr_ = nullptr) :ptr_{ ptr_ } {}

		const auto operator<=>(const iterator& o)const {
			ptr_ <=> o.ptr_;
		}

		iterator operator+(size_t size)const {
			return { ptr_ + size };
		}

		friend const iterator operator+(size_t size, const iterator& o) {
			return { o.ptr_ + size };
		}

		const iterator operator-(size_t size)const {
			return{ ptr_ - size };
		}

		const size_t operator-(const iterator& o)const {
			return  ptr_ - o.ptr_ ;
		}

		iterator& operator++() {
			++ptr_;
			return *this;
		}

		iterator operator++(int) {
			auto tmp{ *this };
			++* this;
			return tmp;
		}

		iterator& operator--() {
			--ptr_;
			return *this;
		}

		iterator operator--(int) {
			auto tmp{ *this };
			--* this;
			return tmp;
		}

		iterator& operator+=(size_t size)const {
			ptr_ += size;
			return *this;
		}

		iterator& operator-=(size_t size)const {
			ptr_ -= size;
			return *this;
		}

		const reference operator[](size_t size)const {
			return ptr_[size];
		}

		const bool operator==(const iterator& o)const {
			return ptr_ == o.ptr_;
		}

		bool operator!=(const iterator& o)const {
			return ptr_ != o.ptr_;
		}

		reference operator*()const {
			return *ptr_;
		}

		T* operator->()const {
			return ptr_;
		}

	};

	size_t n_elements_{};
	std::unique_ptr<T[]>c_{};
public:
	Container(std::initializer_list<T>l) :n_elements_{ l.size() }, c_{ std::make_unique<T[]>(n_elements_) }
	{
		for (size_t index{}; auto e : l) {
			c_[index++] = e;
		}
	}

	Container(size_t sz) :n_elements_{ sz }, c_{ std::make_unique<T[]>(n_elements_) } {}

	size_t size()const {
		return n_elements_;
	}

	const T& operator[](size_t index)const {
		return c_[index];
	}

	const T& at(size_t index)const {
		return index < n_elements_ ? c_[index] : throw std::out_of_range{ "Container::at(): index out of range" };
	}

	bool empty() const {
		return (n_elements_ == 0);
	}

	iterator begin()const { return { c_.get() }; }
	iterator end()const { return { c_.get() + n_elements_ }; }
};

template<typename T>
Container(std::initializer_list<T>l) -> Container<T>;

struct X {
	int x;
};

int main() {
	Container v{1,2,3,4,5};
	for (const auto& i : v) {
		print("{} ", i);
	}
	print("\n");

	Container<X>v3{ {1},{2} };
	auto ret2 = v3.begin();
	ret2->x;
	print("{}\n", std::ranges::forward_range<Container<int>>);
	print("{}\n", std::ranges::range<Container<int>>);
	print("{}\n", std::ranges::viewable_range<Container<int>>);
	print("{}\n", std::bidirectional_iterator<Container<int>>);

	for (const auto& i : v | std::views::reverse) {
		print("{} ", i);
	}
	
}