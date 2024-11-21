#ifndef TRANSLATORS_STRUCTS_H
#define TRANSLATORS_STRUCTS_H

#include <string>
#include <variant>

struct Token {
    enum class Type {
        _try, _catch, _throw, _new, _print, _string, _text,
        _roundOpenBracket, _roundCloseBracket, _curlyOpenBracket, _curlyCloseBracket, _semiColon,
    };
    Type _type;
    std::string _value;

    Token(Type t, const std::string& v) : _type(t), _value(v) {}
    Token() = default;
};

std::string typeToStr(Token::Type& t) {
    switch(t) {
        case(Token::Type::_try): return "try";
        case(Token::Type::_catch): return "catch";
        case(Token::Type::_throw): return "throw";
        case(Token::Type::_new): return "new";
        case(Token::Type::_print): return "print";
        case(Token::Type::_string): return "string";
        case(Token::Type::_text): return "text";
        case(Token::Type::_roundOpenBracket): return "(";
        case(Token::Type::_roundCloseBracket): return ")";
        case(Token::Type::_curlyOpenBracket): return "{";
        case(Token::Type::_curlyCloseBracket): return "}";
        case(Token::Type::_semiColon): return ";";
    }
    return "";
}

struct TryCatchBlock;

struct ThrowBlock {
    std::string _className;
    std::string _text;
    ThrowBlock(const std::string& className, const std::string& text): _className(className), _text(text) {}
};

struct TryBlock {
    std::vector<std::variant<ThrowBlock, std::shared_ptr<TryCatchBlock>>> _blocks;
    TryBlock() = default;
};

struct CatchBlock {
    std::string _className;
    std::string _objName;
    std::vector<std::string> _prints;

    CatchBlock() = default;
    CatchBlock(const std::string& className, const std::string& objName): _className(className), _objName(objName) {}
};

struct Output {
    enum class Type {
        common, error
    };
    Type _type;
    std::string _text;

    Output(Type type, const std::string& text): _type(type), _text(text) {}
};

struct TryCatchBlock {
    TryBlock _tryBlock;
    std::vector<CatchBlock> _catchBlocks;
    std::shared_ptr<TryCatchBlock> parentBlock = nullptr;
    std::vector<Output> results;

    TryCatchBlock(std::shared_ptr<TryCatchBlock> parentBlock): parentBlock(parentBlock) {}
};


#endif //TRANSLATORS_STRUCTS_H
