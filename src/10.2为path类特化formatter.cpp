#include"print.h"
#include<filesystem>
namespace fs = std::filesystem;

//template<>
//struct std::formatter<fs::path> {
//	template<typename ParseContext>
//	constexpr auto parse(ParseContext& ctx) {
//		return ctx.begin();
//	}
//	template<typename FormatContext>
//	auto format(const fs::path& p, FormatContext& ctx) {
//		return std::format_to(ctx.out(), "{}", p.string());
//	}
//};//我们将其放入print.h中，以便后面使用

int main(const int argc,const char**argv) {
	if (argc != 2) {
		fs::path fn{ argv[0] };
		print("usage: {} <path>\n", fn.filename());
		return 0;
	}
	fs::path dir{ argv[1] };
	if (!fs::exists(dir)) {
		print("path: {} does not exist\n", dir);
		return 1;
	}
	print("path: {}\n", dir);//普通的使用特化格式化打印
	print("filename: {}\n", dir.filename());//文件名
	print("cannonical: {}\n", fs::canonical(dir));//绝对路径
	
	fs::path p{ "~/include/bwprint.h" };
	print("{}\n", p);//普通格式化打印
	print("{}\n", p.stem());//返回通用格式路径所标识的文件名，剥去其扩展名
	print("{}\n", p.extension());//返回文件扩展名
	print("{}\n", p.filename());//返回文件名(包含后缀)
	print("{}\n", p.parent_path());//返回到亲目录的路径。
}