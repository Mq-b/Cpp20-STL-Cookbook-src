#include"print.h"
#include<vector>
#include<string>

template<typename T>
class zip_iterator {
	using val_t = typename T::value_type;
	using ret_t = std::pair<val_t, val_t>;
	using it_t = typename T::iterator;

	it_t ita_{};
	it_t itb_{};
	it_t ita_begin_{};
	it_t itb_begin_{};
	it_t ita_end_{};
	it_t itb_end_{};
	zip_iterator(it_t ita, it_t itb) :ita_{ ita }, itb_{ itb } {}//用作begin和end返回的迭代器构造函数

public:
	using value_type = std::pair<val_t, val_t>;
	using difference_type = long int;
	using pointer = const val_t*;
	using reference = const val_t&;

	zip_iterator(T& a, T& b) :
		ita_{ a.begin() }, itb_{ b.begin() }, ita_begin_{ ita_ }, itb_begin_{ itb_ }, ita_end_{ a.end() }, itb_end_{ b.end() } {}
	zip_iterator& operator++() {
		++ita_;
		++itb_;
		return *this;
	}
	bool operator==(const zip_iterator& o)const { return ita_ == o.ita_ || itb_ == o.itb_; }
	bool operator!=(const zip_iterator& o)const { return !operator==(o); }
	ret_t operator*()const {return { *ita_,*itb_ };}
	zip_iterator begin()const { return { ita_begin_,itb_begin_ }; }
	zip_iterator end()const { return { ita_end_,itb_end_ }; }
};

int main() {
	std::vector<std::string>vec_a{ "Bob","John","Joni","🤣" };
	std::vector<std::string>vec_b{ "Dylan","Williams","Mitchell" };

	printc(vec_a, "vec_a: ");
	printc(vec_b, "vec_b: ");

	print("zipped: ");
	for (const auto& [a, b] : zip_iterator{ vec_a, vec_b }) {
		print("[{}, {}] ", a, b);
	}
	print("\n");

	std::map<std::string, std::string>name_map{};
	for (auto [a, b] : zip_iterator{ vec_a,vec_b }) {//插入到map中
		name_map.emplace(a, b);
	}
	print(name_map);//打印

}