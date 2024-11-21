#ifndef TRANSLATORS_SEMANTICANALYZER_H
#define TRANSLATORS_SEMANTICANALYZER_H

#include <memory>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <variant>
#include <algorithm>


class SemanticAnalyzer {
public:
    SemanticAnalyzer() = default;
    void addBlock() {
        curBlock = std::make_shared<TryCatchBlock>(curBlock);
        if (curBlock->parentBlock != nullptr)
            curBlock->parentBlock->_tryBlock._blocks.emplace_back(curBlock);
    }
    void addThrow(const std::string& className, const std::string& text) {
        curBlock->_tryBlock._blocks.emplace_back(ThrowBlock(className, text.substr(1, text.size() - 2)));
    }
    void addCatch(const std::string& className, const std::string& objName) {
        curBlock->_catchBlocks.emplace_back(className, objName);
    }
    void addPrint(const std::string& text) {
        curBlock->_catchBlocks.back()._prints.push_back(text.substr(1, text.size() - 2));
    }
    void makeResults() {
        for (auto&& block: curBlock->_tryBlock._blocks) {
            if (block.index() == 1) {
                curBlock->results.insert(curBlock->results.end(),
                                         std::get<std::shared_ptr<TryCatchBlock>>(block)->results.begin(),
                                         std::get<std::shared_ptr<TryCatchBlock>>(block)->results.end());
            }
            else {
                auto throwBlock = std::get<ThrowBlock>(block);
                auto it = std::find_if(curBlock->_catchBlocks.begin(), curBlock->_catchBlocks.end(),
                                       [&](const CatchBlock& x) { return x._className == throwBlock._className;});
                if (it == curBlock->_catchBlocks.end()) curBlock->results.emplace_back(Output::Type::error, throwBlock._text);
                else {
                    for (auto&& print: it->_prints)
                        curBlock->results.emplace_back(Output::Type::common, print);
                }
            }
            if (curBlock->results.back()._type == Output::Type::error) return;
        }
    }
    auto getResults() {
        return curBlock->results;
    }
    void levelUp() {
        curBlock = curBlock->parentBlock;
    }
private:
    std::shared_ptr<TryCatchBlock> curBlock = nullptr;
};

#endif //TRANSLATORS_SEMANTICANALYZER_H
