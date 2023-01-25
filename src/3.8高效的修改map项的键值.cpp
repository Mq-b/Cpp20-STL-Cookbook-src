#include"print.h"
#include<string>

template<typename M,typename K>
bool node_swap(M& m, K k1, K k2) {
	//extract 是更换 map 的键而不重分配的唯一方式
	auto node1{ m.extract(k1) };
	auto node2{ m.extract(k2) };
	if (node1.empty() || node2.empty())
		return false;
	std::swap(node1.key(), node2.key());
	m.insert(std::move(node1));
	m.insert(std::move(node2));
	return true;
}

int main() {
	std::map<uint32_t, std::string>maps{
		{1,"🐴"},{2,"🥵"},{3,"🤣"},{4,"🐭"},{5,"😘"}
	};
	print(maps);
	::node_swap(maps, 3, 5);
	print(maps);

	auto node = maps.extract(maps.begin());
	node.key() = 5;
	auto t =maps.insert(std::move(node));
	print(maps);
	if (!t.inserted) {
		print("插入失败 {}\n",t.position->second);
	}
}