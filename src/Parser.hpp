#pragma once

#include "Expression.hpp"
#include "Statement.hpp"
#include "Token.hpp"
#include <vector>
#include <memory>

namespace jasl {
    
    class Parser
    {
      public:
        Parser(std::vector<Token> tokens);

        void parse();

        std::vector<std::shared_ptr<Statement>> getStatements() const;

      private:
        std::vector<Token> m_tokens;
        std::vector<Token>::iterator m_current;
        std::vector<std::shared_ptr<Statement>> m_statements;

        std::shared_ptr<Statement> buildStatement();
        bool notAtEnd() const;
        void advanceTokenIterator();
        Token currentToken() const;
        Token nextToken() const;

        std::shared_ptr<Expression> parseIdentifierExpression();
        std::shared_ptr<Expression> parseLiteralIntExpression();
        std::shared_ptr<Expression> parseLiteralRealExpression();
        std::shared_ptr<Expression> parseLiteralStringExpression();
        std::shared_ptr<Expression> parseHatStringExpression();
        std::shared_ptr<Expression> parseHatHatStringExpression();
        std::shared_ptr<Expression> parseQStringExpression();
        std::shared_ptr<Expression> parseQQStringExpression();
        std::shared_ptr<Expression> parseOperatorExpression();
        std::shared_ptr<Expression> parseGroupedExpression();
        std::shared_ptr<Expression> parseListExpression();

        std::shared_ptr<Expression> parseExpression(bool checkOperator = true);

        /// Parses statements of the form
        /// keyword expression -> identifier;
        /// For example
        /// int 5 -> a;
        std::shared_ptr<Statement> parseArrowStatement();

        /// Parses statements of the form
        /// keyword expression;
        /// For example
        /// prn "hello";
        std::shared_ptr<Statement> parseArrowlessStatement();

        /// Parses a repeat statement of the form
        /// repeat expression times { statement* }
        /// For example
        /// repeat 5 times { prn "hello"; }
        std::shared_ptr<Statement> parseRepeatStatement();
    };
}
