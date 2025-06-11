/// (c) Ben Jones 2019 - present

#pragma once

#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>
#include <vector>

namespace arrow {
    class PodStatement final : public Statement
    {
      public:
        PodStatement(long const lineNumber);

        PodStatement & withToken(Token token);
        PodStatement & withNameToken(Token token);
        PodStatement & 
        withExpressionCollection(std::shared_ptr<Expression> expression);
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
        Token const & getName() const;
        std::shared_ptr<Expression> const & getExpressionCollection() const;
      private:
        Token m_keywordToken; // pod keyword
        Token m_nameToken;
        std::shared_ptr<Expression> m_expressionCollection;
    };

}