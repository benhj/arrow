#pragma once

#include "Statement.hpp"
#include "lexer/Token.hpp"
#include "expressions/Expression.hpp"
#include <memory>

namespace arrow {
    
    // Describes a statement of the form
    // expression matches expression -> identifier;
    // Expression should be a list, e.g.:
    // [a b c] matches [a b c] -> result;
    class MatchesStatement : public Statement
    {
      public:
        MatchesStatement(long const lineNumber);
        MatchesStatement & withLeftExpression(std::shared_ptr<Expression> expression);
        MatchesStatement & withRightExpression(std::shared_ptr<Expression> expression);
        MatchesStatement & withIdentifier(Token identifier);

        Token getIdentifier() const;
        std::shared_ptr<Expression> getLeftExpression() const;
        std::shared_ptr<Expression> getRightExpression() const;

        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;

      private:
        Token m_identifier;
        std::shared_ptr<Expression> m_left;
        std::shared_ptr<Expression> m_right;
    };

}