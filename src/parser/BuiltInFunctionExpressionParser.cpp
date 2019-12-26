/// (c) Ben Jones 2019 - present

#include "BuiltInFunctionExpressionParser.hpp"
#include "expressions/builtin/BuiltInFunctionExpressionBuilder.hpp"
#include "expressions/builtin/RandomFunctionExpression.hpp"
#include "expressions/builtin/SqrtFunctionExpression.hpp"
#include "expressions/builtin/StringInputExpression.hpp"
#include "expressions/builtin/SystemCommandExpression.hpp"
#include "expressions/builtin/filesystem/FileReadLinesFunctionExpression.hpp"
#include "expressions/builtin/filesystem/FileReadBytesFunctionExpression.hpp"
#include "expressions/builtin/filesystem/FileWriteBytesFunctionExpression.hpp"
#include "expressions/builtin/filesystem/FolderListFunctionExpression.hpp"
#include "expressions/builtin/filesystem/FolderListRecursiveFunctionExpression.hpp"
#include <algorithm>
#include <utility>

namespace arrow {

    std::vector<std::shared_ptr<BuiltInFunctionExpressionBuilder>> 
    BuiltInFunctionExpressionParser::m_builtins;
    BuiltInFunctionExpressionParser::BuiltInFunctionExpressionParser(TokenManager & tm)
    : m_tm(tm)
    {
    }
    std::shared_ptr<Expression> BuiltInFunctionExpressionParser::parseExpression(std::string name)
    {
        auto const ln = m_tm.currentToken().lineNumber;
        if(m_builtins.empty()) {
            m_builtins.emplace_back(std::make_shared<RandomFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<SqrtFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<StringInputExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<SystemCommandExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<FileReadLinesFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<FolderListFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<FolderListRecursiveFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<FileReadBytesFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<FileWriteBytesFunctionExpressionBuilder>());
        }
        auto found = std::find_if(std::begin(m_builtins),
                                  std::end(m_builtins),
                                  [name{std::move(name)}]
                                  (auto const & p) {
            return p->getName() == name;
        });
        if(found != std::end(m_builtins)) {
            return (*found)->build(ln);
        }
        return nullptr;
    }
}
