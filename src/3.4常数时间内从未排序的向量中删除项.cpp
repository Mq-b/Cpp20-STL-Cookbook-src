#include"print.h"
#include<vector>
#include<ranges>
namespace stdr = std::ranges;

//使用下标的版本
template<typename T>
void quick_delete(T& v, size_t idx) {
	if (idx < v.size()) {
		v[idx] = std::move(v.back());
		v.pop_back();
	}
}
//使用迭代器的版本
template<typename T>
void quick_delete(T& v, typename T::iterator it) {
	if (it < v.end()) {
		*it = std::move(v.back());
		v.pop_back();
	}
}
//若 vector 中项目的顺序不重要，就可以优化这个过程，使其花费 O(1)(常数) 时间
//做法很简单，将传入的要删除的迭代器或索引赋值为末尾元素的值，然后将末尾元素删除，就完成了，但是没有顺序

int main() {
	std::vector v{ 1,2,3,4,5 };
	print(v);
	auto it = stdr::find(v, 3);
	quick_delete(v, it);
	print(v);//顺序不对，正常现象

	quick_delete(v, 2);
	print(v);
}