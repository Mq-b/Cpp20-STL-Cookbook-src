#include"print.h"
#include<memory>

struct Thing {
	std::string thname{ "unk" };
	Thing() { print("default ctor:{}\n", thname); }
	Thing(std::string n):thname(n) { print("param ctor:{}\n", thname); }
	~Thing() { print("dtor :{}\n", thname); }
};

void process_thing(const std::unique_ptr<Thing>&p) {
	if (p)print("processing: {}\n", p->thname);
	else print("invalid pointer\n");
}

struct Delete {
	void operator()(Thing* p) {
		print("自定义删除函数被调用\n");
		delete p;
	}
};

int main() {
	//std::unique_ptr<Thing>pl{new Thing};

	auto pl1 = std::make_unique<Thing>("Thing 1");
	process_thing(pl1);
	process_thing(std::make_unique<Thing>("Thing 2"));

	//auto p2 = std::move(pl1);
	process_thing(pl1);
	pl1.reset();
	process_thing(pl1);
	pl1.reset(new Thing("Thing 3"));
	process_thing(pl1);

	std::unique_ptr<Thing, Delete>p2{ new Thing("🤣🤣"),Delete{} };

	print("end of main()\n");
}