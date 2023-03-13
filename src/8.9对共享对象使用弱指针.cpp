#include"print.h"
#include<memory>

struct Thing {
	std::string thname{ "unk" };
	Thing() { print("default ctor:{}\n", thname); }
	Thing(std::string n) :thname(n) { print("param ctor:{}\n", thname); }
	~Thing() { print("dtor :{}\n", thname); }
};
void get_weak_thing(const std::weak_ptr<Thing>& p) {
	if (auto sp = p.lock())print("{} use count: {}\n", sp->thname, sp.use_count());
	else print("no shared object\n");
}

struct circB;
struct circA {
	std::shared_ptr<circB>p;
	~circA() { print("dtor A\n"); }
};
struct circB {
	std::weak_ptr<circA>p;//如果这里不使用weak_ptr而是shared_ptr，那么就会循环引用
	~circB() { print("dtor B\n"); }
};

std::shared_ptr<Thing>p;

int main() {
	auto thing1 = std::make_shared<Thing>("Thing 1");
	std::weak_ptr<Thing>wp1;
	print("expried: {}\n", wp1.expired());//等价于use_cout()==0
	get_weak_thing(wp1);

	wp1 = thing1;
	get_weak_thing(wp1);

	std::weak_ptr<Thing>wp2(thing1);
	get_weak_thing(wp2);

	thing1.reset();
	get_weak_thing(wp1);
	get_weak_thing(wp2);

	auto a{ std::make_shared<circA>() };
	auto b{ std::make_shared<circB>() };
	a->p = b;
	b->p = a;
	//我们需要强调一点：weak_ptr是没有所有权的，指向shared_ptr不会增加use_count()的值，所以可以解决一些循环引用导致use_count()始终不为0无法析构的问题
	//即A内部有指向B，B内部有指向A，这样对于A，B必定是在A析构后B才析构，对于B，A必定是B析构后才析构A，这就是循环引用的问题，违反常规，导致内存泄露
}//