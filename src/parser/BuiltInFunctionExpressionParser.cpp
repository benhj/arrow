/// (c) Ben Jones 2019 - present

#include "BuiltInFunctionExpressionParser.hpp"
#include "builtin/BuiltInFunctionExpressionBuilder.hpp"
#include "builtin/ArgExpression.hpp"
#include "builtin/CosFunctionExpression.hpp"
#include "builtin/GetKeyPressExpression.hpp"
#include "builtin/InitKeyHandlerFunctionExpression.hpp"
#include "builtin/LengthExpression.hpp"
#include "builtin/MapContainsFunctionExpression.hpp"
#include "builtin/MapKeysFunctionExpression.hpp"
#include "builtin/RandomFunctionExpression.hpp"
#include "builtin/RegexFunctionExpression.hpp"
#include "builtin/SinFunctionExpression.hpp"
#include "builtin/SqrtFunctionExpression.hpp"
#include "builtin/StringInputExpression.hpp"
#include "builtin/SystemCommandExpression.hpp"
#include "builtin/TrimStringFunctionExpression.hpp"
#include "builtin/ToIntFunctionExpression.hpp"
#include "builtin/TokenizeFunctionExpression.hpp"
#include "builtin/WildcardEqExpression.hpp"
#include "builtin/TypeExpression.hpp"
#include "builtin/filesystem/FileReadLinesFunctionExpression.hpp"
#include "builtin/filesystem/FileReadBytesFunctionExpression.hpp"
#include "builtin/filesystem/FolderListFunctionExpression.hpp"
#include "builtin/filesystem/FolderListRecursiveFunctionExpression.hpp"

#ifdef WITH_HEAVY
#include "heavy/HTTPReadExpression.hpp"
#endif

#ifdef WITH_GRAPHICS
#include "graphics/InitGraphicsCanvasFunctionExpression.hpp"
#include "graphics/DrawFunctionExpression.hpp"
#include "graphics/DrawLineFunctionExpression.hpp"
#include "graphics/DrawCircleFunctionExpression.hpp"
#include "graphics/FillCircleFunctionExpression.hpp"
#include "graphics/DrawRectFunctionExpression.hpp"
#include "graphics/FillRectFunctionExpression.hpp"
#include "graphics/GraphicsProcessFunctionExpression.hpp"
#endif

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
            m_builtins.emplace_back(std::make_shared<ArgExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<CosFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<GetKeyPressExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<InitKeyHandlerFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<LengthExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<MapContainsFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<MapKeysFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<RandomFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<RegexFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<SinFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<SqrtFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<StringInputExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<SystemCommandExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<TokenizeFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<ToIntFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<TrimStringFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<TypeExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<WildcardEqExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<FileReadLinesFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<FolderListFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<FolderListRecursiveFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<FileReadBytesFunctionExpressionBuilder>());
#ifdef WITH_HEAVY
            m_builtins.emplace_back(std::make_shared<HTTPReadExpressionBuilder>());
#endif
#ifdef WITH_GRAPHICS
            m_builtins.emplace_back(std::make_shared<InitGraphicsCanvasFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<DrawFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<DrawLineFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<DrawCircleFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<DrawRectFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<FillCircleFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<FillRectFunctionExpressionBuilder>());
            m_builtins.emplace_back(std::make_shared<GraphicsProcessFunctionExpressionBuilder>());
#endif
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
