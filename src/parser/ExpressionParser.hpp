/// (c) Ben Jones 2019 - present

#pragma once

#include "TokenManager.hpp"
#include "expressions/Expression.hpp"
#include "lexer/Token.hpp"

namespace arrow {

    class ExpressionParser
    {
      public:
        ExpressionParser(TokenManager & tm);
        std::shared_ptr<Expression> parseExpression(bool checkOperator = true);

        std::shared_ptr<Expression> parseLiteralIntExpression();
        std::shared_ptr<Expression> parseIndexExpression();
        std::shared_ptr<Expression> parseIdentifierExpression();
        std::shared_ptr<Expression> parseLiteralRealExpression();
        std::shared_ptr<Expression> parseLiteralStringExpression();
        std::shared_ptr<Expression> parseLiteralCharExpression();
        std::shared_ptr<Expression> parseHatStringExpression();
        std::shared_ptr<Expression> parseHatHatStringExpression();
        std::shared_ptr<Expression> parseQStringExpression();
        std::shared_ptr<Expression> parseQQStringExpression();
        std::shared_ptr<Expression> parseOperatorExpression();
        std::shared_ptr<Expression> parseBooleanExpression(std::shared_ptr<Expression> left);
        std::shared_ptr<Expression> parseMathExpression(std::shared_ptr<Expression> left);
        std::shared_ptr<Expression> parseGroupedExpression();
        std::shared_ptr<Expression> parseListExpression();
        std::shared_ptr<Expression> parseListWordExpression();
        std::shared_ptr<Expression> parseSingleEqualExpression();
        std::shared_ptr<Expression> parseDoubleEqualExpression();
        std::shared_ptr<Expression> parseFunctionExpression();
        std::shared_ptr<Expression> parseMatchesExpression();

        /// Parse a part of ListExpression. Note, we need to distinguish
        /// between 'plain' words and identifiers, both of which
        /// are generic strings.
        std::shared_ptr<Expression> parseListExpressionType();

        std::shared_ptr<Expression> 
        parseExpressionCollectionExpression(bool const identifierOnly = false);

        /// Expresions of the type {a, b, c}
        std::shared_ptr<Expression> parseBracedExpressionCollectionExpression();

      private:

        TokenManager & m_tm;
    };
}
