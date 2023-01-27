#include"print.h"
#include<deque>
#include<string>

class RPN {
	std::deque<double>deq_{};
	constexpr static double zero_{ 0.0 };
	constexpr static double inf_{ std::numeric_limits<double>::infinity() };

	bool is_numeric(const std::string str) {
		for (const auto& i:str) {
			if (i != '.' && !std::isdigit(i))
				return false;
		}
		return true;
	}

	std::pair<double, double>pop_get2() {
		if (deq_.size() < 2)return { zero_,zero_ };
		double v1{ deq_.front() };
		deq_.pop_front();
		double v2{ deq_.front() };
		deq_.pop_front();
		return { v2,v1 };
	}

	double optor(const std::string op) {
		std::map<std::string, double(*)(double, double)>opmap{
			{"+",[](double l,double r) {return l + r; }},
			{"-",[](double l,double r) {return l - r; }},
			{"*",[](double l,double r) {return l * r; }},
			{"/",[](double l,double r) {return l / r; }},
			{"^",[](double l,double r) {return std::pow(l,r); }},
			{"%",[](double l,double r) {return std::fmod(l,r); }}
		};
		if (opmap.find(op) == opmap.end())return zero_;
		auto [l, r] = pop_get2();
		if (op == "/" && r == zero_)deq_.push_front(inf_);
		else deq_.push_front(opmap.at(op)(l, r));
		return deq_.front();
	}

public:
	double op(const std::string& s) {
		if (is_numeric(s)) {
			double v{ std::stod(s) };
			deq_.push_front(v);
			return v;
		}
		else return optor(s);
	}

	void clear() {
		deq_.clear();
	}

	std::string get_stack_string()const {
		std::string s{};
		for (const auto& v : deq_) {
			s += std::format("{} ", v);
		}
		return s;
	}
};

int main() {
	RPN rpn;
	for (std::string o{}; std::cin >> o;) {
		rpn.op(o);
		auto stack_str{ rpn.get_stack_string() };
		print("{}: {}\n", o, stack_str);
	}
}