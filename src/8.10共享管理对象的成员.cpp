#include"print.h"
#include<memory>
#include<string>

struct animal {
	std::string name{};
	std::string sound{};
	animal(const std::string& n, const std::string& a) :name{ n }, sound{ a } {
		print("ctor: {}\n", name);
	}
	~animal() { print("dtor: {}\n", name); }
};
auto make_animal(const std::string& n, const std::string& s) {
	auto ap = std::make_shared<animal>(n, s);
	auto np = std::shared_ptr<std::string>(ap, &ap->name);
	auto sp = std::shared_ptr<std::string>(ap, &ap->sound);
	print("Use count: name {}, sound {}\n", np.use_count(), sp.use_count());//3 3
	return std::tuple(np, sp);//因为别名指针 可以防止使用计数达到零，所以不会删除
}
int main() {
	auto [name,sound] = make_animal("Velociraptor", "Grrrr!");
	print("The {} says {}\n", *name, *sound);
	print("Use count: name {}, sound {}\n", name.use_count(), sound.use_count());//2 2，那个函数里面的shared_ptr肯定还是销毁了的
}