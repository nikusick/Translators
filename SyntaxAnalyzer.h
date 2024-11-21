#ifndef TRANSLATORS_SYNTAXANALYZER_H
#define TRANSLATORS_SYNTAXANALYZER_H
#include "LexicalAnalyzer.h"
#include "SemanticAnalyzer.h"
#include <format>

const std::string ERROR_MSG = "Expected {}, got {} instead!";

class SyntaxAnalyzer {
public:
    SyntaxAnalyzer(const std::string& str) {
        _lexicalAnalyzer = std::make_shared<LexicalAnalyzer>(str);
        _semanticAnalyzer = std::make_shared<SemanticAnalyzer>();
    }

    std::vector<Output>& getResult() {
        auto token = _lexicalAnalyzer->getToken();
        check(token, Token::Type::_try);
        sentence();
        return result;
    }
private:
    void check(std::optional<Token> token, Token::Type expected) {
        if (token == std::nullopt)
            throw std::invalid_argument(std::vformat(ERROR_MSG,
                                                     std::make_format_args(typeToStr(expected), "None")));
        if (token->_type != expected)
            throw std::invalid_argument(std::vformat(ERROR_MSG,
                                                     std::make_format_args(typeToStr(expected), typeToStr(token->_type))));
    }

    void throwBlock() {
        auto token = _lexicalAnalyzer->getToken();
        check(token, Token::Type::_new);
        auto className = _lexicalAnalyzer->getToken();
        check(className, Token::Type::_string);
        token = _lexicalAnalyzer->getToken();
        check(token, Token::Type::_roundOpenBracket);
        auto text = _lexicalAnalyzer->getToken();
        check(text, Token::Type::_text);
        token = _lexicalAnalyzer->getToken();
        check(token, Token::Type::_roundCloseBracket);
        token = _lexicalAnalyzer->getToken();
        check(token, Token::Type::_semiColon);
        _semanticAnalyzer->addThrow(className->_value, text->_value);
    }

    void state() {
        auto token = _lexicalAnalyzer->getToken();
        if (token->_type == Token::Type::_curlyCloseBracket) {
            token = _lexicalAnalyzer->getToken();
            if (token->_type == Token::Type::_catch) return; // without error
        }
        if (token->_type == Token::Type::_throw)
            throwBlock();
        else if (token->_type == Token::Type::_try) {
            sentence();
            token = _lexicalAnalyzer->getToken();
            if (token->_type == Token::Type::_catch) return; //without error
            _lexicalAnalyzer->returnToken(*token);
        }
        else return; // error
        state();
    }

    void tryBlock() {
        auto token = _lexicalAnalyzer->getToken();
        check(token, Token::Type::_curlyOpenBracket);
        state();
    }

    void code() {
        auto token = _lexicalAnalyzer->getToken();
        if (token->_type == Token::Type::_curlyCloseBracket) return; // without error
        check(token, Token::Type::_print);
        token = _lexicalAnalyzer->getToken();
        check(token, Token::Type::_roundOpenBracket);
        auto text = _lexicalAnalyzer->getToken();
        check(text, Token::Type::_text);
        token = _lexicalAnalyzer->getToken();
        check(token, Token::Type::_roundCloseBracket);
        token = _lexicalAnalyzer->getToken();
        check(token, Token::Type::_semiColon);
        _semanticAnalyzer->addPrint(text->_value);
        code();
    }

    void catchBlock() {
        auto token = _lexicalAnalyzer->getToken();
        check(token, Token::Type::_roundOpenBracket);
        auto className = _lexicalAnalyzer->getToken();
        check(className, Token::Type::_string);
        auto objName = _lexicalAnalyzer->getToken();
        check(objName, Token::Type::_string);
        token = _lexicalAnalyzer->getToken();
        check(token, Token::Type::_roundCloseBracket);
        token = _lexicalAnalyzer->getToken();
        check(token, Token::Type::_curlyOpenBracket);
        _semanticAnalyzer->addCatch(className->_value, objName->_value);
        code();
        token = _lexicalAnalyzer->getToken();
        if (token == std::nullopt || token->_type != Token::Type::_catch) return; // without error
        catchBlock();
    }

    void sentence() {
        _semanticAnalyzer->addBlock();
        tryBlock();
        catchBlock();
        _semanticAnalyzer->makeResults();
        result = _semanticAnalyzer->getResults();
        _semanticAnalyzer->levelUp();
    }

    std::shared_ptr<LexicalAnalyzer> _lexicalAnalyzer;
    std::shared_ptr<SemanticAnalyzer> _semanticAnalyzer;
    std::vector<Output> result;
};

#endif //TRANSLATORS_SYNTAXANALYZER_H
