#include"print.h"
#include<string>
#include<unordered_map>

struct Coord {
	int x{};
	int y{};
};
auto operator==(const Coord& a, const Coord& b) {
	return a.x == b.x && a.y == b.y;
}
namespace std {
	template<>
	struct hash<Coord> {
		size_t operator()(const Coord&a)const {
			return static_cast<size_t>(a.x) + static_cast<size_t>(a.y);
		}
	};
}
template<class T, class T2>
inline void print(const std::unordered_map<T, T2>& map) {
	print("size: {} ", map.size());
	for (auto& [k, v] : map)print("{{{} {}}}:{} ", k.x, k.y, v);
	print("\n");
}
int main() {
	std::unordered_map<Coord, std::string>map{ {{1,1},"😘"},{{0,0},"🤣"} };
	print(map);
}