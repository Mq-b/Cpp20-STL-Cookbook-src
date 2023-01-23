#include"print.h"
#include<string>
#include<vector>
#include<list>
using Vstr = std::vector<std::string>;
namespace stdr = std::ranges;

void psorted(stdr::range auto&& v) {
	if  (stdr::is_sorted(v))
		print("sorted: ");
	else
		print("unsorted: ");
	print(v);
}

void insert_sorted(Vstr& v, const std::string& s) {
	//lower_bound() 算法查找不小于实参的第一个元素的迭代器
	const auto pos{ stdr::lower_bound(v,s) };
	v.insert(pos, s);//使用 lower_bound() 返回的迭代器在正确的位置插入一个元素
}

template<stdr::range C,typename E>
void insert_sorted(C& c, const E& e) {
	const auto pos{ stdr::lower_bound(c,e) };
	c.insert(pos, e);
}

int main() {
	std::vector<std::string> v{ "2","1","3"};
	psorted(v);//无序

	stdr::sort(v);
	psorted(v);//有序

	//v.emplace_back("0");
	//psorted(v);//无序

	::insert_sorted(v, "0");
	psorted(v);//有序，相比于普通插入的优势
	//用list测试改写泛型的版本
	std::list<int>list{ 1,2,3,4,5 };
	psorted(list);//有序
	::insert_sorted(list, 0);
	psorted(list);//有序
}