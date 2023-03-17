#include"print.h"
#include<mutex>
#include<thread>
#include<list>
#include<optional>
#include<future>

std::mutex animal_mutex;

class Animal {
	using friend_t = std::list<Animal>;
	std::string_view s_name{ "unk" };
	friend_t l_friends{};
public:
	Animal() = delete;
	Animal(const std::string_view n) :s_name{n}{}
	bool operator==(const Animal& o)const {
		return s_name.data() == o.s_name.data();
	}

	bool is_friend(const Animal& o)const {
		for (const auto& a : l_friends) {
			if (a == o)return true;
		}
		return false;
	}
	std::optional<friend_t::iterator>find_friend(const Animal& o)noexcept {
		for (auto it{ l_friends.begin() }; it != l_friends.end(); ++it) {
			if (*it == o)return it;
		}
		return {};
	}
	void print()const noexcept {
		std::lock_guard l{ animal_mutex };
		auto n_animals{ l_friends.size() };
		::print("Animal: {}, friends: ", s_name);
		if (!n_animals)::print("none");
		else {
			for (auto n : l_friends) {
				std::cout << n.s_name;
				if (--n_animals)std::cout << ", ";
			}
		}
		endl(std::cout);
	}
	bool add_friend(Animal& o)noexcept {
		std::lock_guard l(animal_mutex);
		::print("add_friend {} -> {}\n", s_name, o.s_name);
		if (*this == o)return false;
		if (!is_friend(o))l_friends.emplace_back(o);//无重复则插入
		if (!o.is_friend(*this))o.l_friends.emplace_back(*this);
		return true;
	}
	bool delete_friend(Animal& o)noexcept {
		std::lock_guard l{ animal_mutex };
		::print("delete_friend {} -> {}\n", s_name, o.s_name);
		if (*this == o)return false;
		if (auto it = find_friend(o))l_friends.erase(it.value());
		if (auto it = o.find_friend(*this))o.l_friends.erase(it.value());
		return true;
	}
};

int main() {
	auto cat1 = std::make_unique<Animal>("Felix");
	auto tiger1 = std::make_unique<Animal>("Hobbes");
	auto dog1 = std::make_unique<Animal>("Astro");
	auto rabbit1 = std::make_unique<Animal>("Bugs");
	
	auto a1 = std::async([&] {cat1->add_friend(*tiger1); });
	auto a2 = std::async([&] {tiger1->add_friend(*rabbit1); });
	auto a3 = std::async([&] {dog1->add_friend(*dog1); });
	auto a4 = std::async([&] {rabbit1->add_friend(*cat1); });
	a1.wait();
	a2.wait();
	a3.wait();
	a4.wait();

	auto p1 = std::async([&] {cat1->print(); });
	auto p2 = std::async([&] {tiger1->print(); });
	auto p3 = std::async([&] {dog1->print(); });
	auto p4 = std::async([&] {rabbit1->print(); });
	p1.wait();
	p2.wait();
	p3.wait();
	p4.wait();

	auto a5 = std::async([&] {cat1->delete_friend(*rabbit1); });
	a5.wait();
	auto p5 = std::async([&] {cat1->print(); });
	auto p6 = std::async([&] {rabbit1->print(); });
}