#include"print.h"
#include<memory>

struct Thing {
	std::string thname{ "unk" };
	Thing() { print("default ctor:{}\n", thname); }
	Thing(std::string n) :thname(n) { print("param ctor:{}\n", thname); }
	~Thing() { print("dtor :{}\n", thname); }
};

void check_thing_ptr(const std::shared_ptr<Thing>& p) {
	if (p)print("{} use count: {}\n", p->thname, p.use_count());
	else print("invalid pointer\n");
}

int main() {
	std::shared_ptr<Thing>p1{ new Thing("Thing 1") };
	auto p2 = std::make_shared<Thing>("Thing 2");
	check_thing_ptr(p1);
	check_thing_ptr(p2);
	{
		auto pa = p1;
		auto pb = p1;
		auto pc = p1;
		auto pd = p1;
		check_thing_ptr(p1);
		check_thing_ptr(pa);
		check_thing_ptr(pb);
		check_thing_ptr(pc);
		check_thing_ptr(pd);
	}
	check_thing_ptr(p1);

	auto p3 = p1;
	check_thing_ptr(p1);
	p3.reset();
	check_thing_ptr(p1);
	p1.reset(new Thing{ "🥵" }, [](Thing* p) {
		puts("自定义删除器被调用"); 
		delete p; 
	});

	std::shared_ptr<Thing>p4{ new Thing("Thing 4"),[](Thing* p) {
		puts("自定义删除器被调用!!!🤡🤡");
		delete p;
	} };
}