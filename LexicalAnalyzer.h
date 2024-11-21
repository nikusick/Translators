#ifndef TRANSLATORS_LEXICALANALYZER_H
#define TRANSLATORS_LEXICALANALYZER_H

#include <regex>
#include <iterator>
#include <queue>
#include <optional>

#include "structs.h"

class LexicalAnalyzer {
private:
    auto strip(const std::string& str) {
        std::regex regex(R"(^\s+|\s+$)");
        return std::regex_replace(str, regex, "");
    }

    auto getType(const std::string& str) {
        if (str == "try")         return Token::Type::_try;
        else if (str == "catch")  return Token::Type::_catch;
        else if (str == "throw")  return Token::Type::_throw;
        else if (str == "new")    return Token::Type::_new;
        else if (str == "print")  return Token::Type::_print;
        else if (str == "(")    return Token::Type::_roundOpenBracket;
        else if (str == ")")    return Token::Type::_roundCloseBracket;
        else if (str == "{")    return Token::Type::_curlyOpenBracket;
        else if (str == "}")    return Token::Type::_curlyCloseBracket;
        else if (str == ";")    return Token::Type::_semiColon;
        else if (str.front() == '\'' and str.back() == '\'') return Token::Type::_text;
        else return Token::Type::_string;
    }

    std::queue<Token> tokens;
    std::regex reg{R"(\s*(try|catch|throw|new|print|\w+|'.*'|\(|\)|\{|\}|;))"};
public:
    LexicalAnalyzer(const std::string& str) {
        size_t ind = 0;
        auto tokenItBegin = std::sregex_iterator{str.begin(), str.end(), reg};
        for (auto it = tokenItBegin; it != std::sregex_iterator(); ++it) {
            if (it->str().empty()) continue;
            if (it->position() != ind) { throw std::invalid_argument("Invalid symbol at position" + std::to_string(ind + 1)); }
            ind += it->str().size();
            auto string = strip(it->str());
            tokens.emplace(getType(string), string);
        }
    }
    auto getToken() {
        auto last = Token();
        bool f = !tokens.empty();
        if (!tokens.empty()) {
            last = tokens.front();
            tokens.pop();
        }
        return f ? std::optional<Token>{last} : std::nullopt;
    }
    void returnToken(Token token) {
        tokens.push(token);
        for (int i = 0; i < tokens.size() - 1; i++) {
            tokens.push(tokens.front());
            tokens.pop();
        }
    }
};

#endif //TRANSLATORS_LEXICALANALYZER_H
