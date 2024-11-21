#include <iostream>
#include <fstream>
#include <coroutine>
#include <thread>
#include <filesystem>
#include "SyntaxAnalyzer.h"

namespace fs = std::filesystem;

int main() {
    fs::path filePath = fs::current_path() / "test" / "example.txt";
    std::ifstream in(filePath);
    std::ostringstream sstr;
    if (in.is_open())
    {
        sstr << in.rdbuf();
    }
    in.close();

    SyntaxAnalyzer analyzer(sstr.str());
    auto result = analyzer.getResult();
    for (auto& res: result) {
        if (res._type == Output::Type::error) {std::cerr << res._text << std::endl; return 1;}
        else std::cout << res._text << std::endl;
    }
    return 0;
}
