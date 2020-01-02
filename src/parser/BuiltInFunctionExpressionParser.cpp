/// (c) Ben Jones 2019 - present

#include "BuiltInFunctionExpressionParser.hpp"
#include "builtin/BuiltInFunctionExpressionBuilder.hpp"
#include "builtin/RandomFunctionExpression.hpp"
#include "builtin/SqrtFunctionExpression.hpp"
#include "builtin/StringInputExpression.hpp"
#include "builtin/SystemCommandExpression.hpp"
#include "builtin/filesystem/FileReadLinesFunctionExpression.hpp"
#include "builtin/filesystem/FileReadBytesFunctionExpression.hpp"
#include "builtin/filesystem/FileWriteBytesFunctionExpression.hpp"
#include "builtin/filesystem/FolderListFunctionExpression.hpp"
#include "builtin/filesystem/FolderListRecursiveFunctionExpression.hpp"
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
