#include"print.h"
#include <string>

const char prompt(const char* p) {
    std::string r;
    print("{} > ", p);
    std::getline(std::cin, r, '\n');

    if (r.size() < 1) return '\0';//如果走这个分支，就是直接下一个循环
    if (r.size() > 1) {
        print("响应时间过长\n");
        return '\0';
    }
    return toupper(r[0]);
}

int main() {
    using jumpfunc = void(*)();

    std::map<const char, jumpfunc> jumpmap{
        { 'A', [] { print("func A\n"); } },
        { 'B', [] { print("func B\n"); } },
        { 'C', [] { print("func C\n"); } },
        { 'D', [] { print("func D\n"); } },
        { 'X', [] { print("Bye!\n"); } }
    };

    char select{};
    while (select != 'X') {
        if ((select = prompt("select A/B/C/D/X"))) {
            auto it = jumpmap.find(select);
            if (it != jumpmap.end()) it->second();
            else print("没有对应的选项！\n");
        }
    }
}