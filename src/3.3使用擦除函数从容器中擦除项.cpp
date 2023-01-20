#include"print.h"
#include<vector>
#include<list>

template<typename Tc,typename Tv>
void remove_value(Tc& c, const Tv& v) {//C++20之前的做法
	//std::remove将和传入元素相同的元素移动放到末尾，并返回迭代器位置，还有一个std::remove_if的版本
	auto remove_it = std::remove(c.begin(), c.end(), v);//remove_it是首个需要被删除元素的位置
	c.erase(remove_it, c.end());//删除remove_it到end()这个范围的元素
}

int main() {
	std::vector v{ 1,2,3,4,5 };
	print(v);
	::remove_value(v, 1);
	print(v);
	std::erase(v,5);//C++20起，功能和remove_value()相同
	print(v);
	std::erase_if(v, [](int i) {return i % 2 != 0; });//第二个版本
	print(v);

	std::list list{ 1,2,3,4,5,6,7,8,9,10 };
	std::erase(list, 5);
	std::erase_if(list, [](int i) {return i % 2 == 0; });
	print(list);

	std::map<int, std::string> map{ {1,"🤣"},{2,"🥵"},{3,"🐴"},{4,"🐭"} };
	print(map);
	std::erase_if(map, [](auto& i) {
		const auto& [k, v] = i;
		return v == "🥵";
	});
	print(map);
}