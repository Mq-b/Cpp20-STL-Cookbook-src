#include"print.h"
#include<string_view>
#include<variant>
#include<list>

class Animal {
	std::string_view _name{};
	std::string_view _sound{};
public:
	Animal(std::string_view n, std::string_view s) :_name{ n }, _sound{ s } {}
	void speak()const {
		print("{} says {}\n", _name, _sound);
	}
	void sound(std::string_view s) {
		_sound = s;
	}
};

class Cat :public Animal {
public:
	Cat(std::string_view n):Animal(n,"meow"){}
};
class Dog :public Animal {
public:
	Dog(std::string_view n) :Animal(n, "arf!"){}
};
class Wookie :public Animal {
public:
	Wookie(std::string_view n) :Animal(n, "grrraarrgghh!"){}
};

struct animal_speaks {
	void operator()(const Dog& d)const { d.speak(); }
	void operator()(const Cat& c)const { c.speak(); }
	void operator()(const Wookie& w)const { w.speak(); }
};

using v_animal = std::variant<Cat, Dog, Wookie>;
int main() {
	std::list<v_animal>pets{ Cat{"Hobbes"},Dog{"Fido"},Cat{"Max"},Wookie{"Chewie"} };
	for (const v_animal& a : pets) {
		std::visit(animal_speaks{}, a);
	}

	for (const v_animal& a : pets) {
		auto idx{ a.index() };
		if (idx == 0)std::get<Cat>(a).speak();
		if (idx == 1)std::get<Dog>(a).speak();
		if (idx == 2)std::get<Wookie>(a).speak();
	}

	for (const v_animal& a : pets) {
		if (const auto c{ std::get_if<Cat>(&a) }; c)
			c->speak();
		else if (const auto d{ std::get_if<Dog>(&a) }; d)
			d->speak();
		else if (const auto w{ std::get_if<Wookie>(&a) }; w)
			w->speak();
	}

	size_t n_cats{}, n_dogs{}, n_wookies{};
	for (const v_animal& a : pets) {
		if (std::holds_alternative<Cat>(a))++n_cats;
		if (std::holds_alternative<Dog>(a))++n_dogs;
		if (std::holds_alternative<Wookie>(a))++n_wookies;
	}
	print("there are {} cat(s)，{} dog(s)，and {} wookie(s)\n", n_cats, n_dogs, n_wookies);
}