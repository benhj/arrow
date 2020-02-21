/// (c) Ben Jones 2019 - present

#pragma once

#include "ExpressionParser.hpp"
#include "ReceiverParser.hpp"
#include "TokenManager.hpp"
#include "lexer/Token.hpp"
#include "representation/Environment.hpp"
#include "representation/Type.hpp"
#include "receivers/Receiver.hpp"
#include "statements/Statement.hpp"
#include <map>
#include <memory>
#include <ostream>
#include <vector>

namespace arrow {

    class FunctionStatement;
    class Parser
    {
      public:
        Parser(std::vector<Token> tokens,
               Environment & environment,
               std::ostream & os);
        Parser() = delete;
        void parse();

        std::shared_ptr<Statement> getStartStatement() const;
        std::vector<std::shared_ptr<Statement>> getStatements() const;

        /// Parses the program arguments that were tokenized
        /// by the lexer.
        void parseProgramArguments();

      private:

        /// The class instance that iterates over the
        /// collection of tokens used to build the AST.
        TokenManager m_tm;

        /// The 'environment' of a program (program stack etc.)
        Environment & m_environment;

        /// For outputting program
        std::ostream & m_os;

        /// Parses expressions
        ExpressionParser m_ep;

        /// Parses receivers
        ReceiverParser m_rp;

        /// The start statement represents a program's entry point.
        /// (Similar to a main function in c in c++)
        std::shared_ptr<Statement> m_startStatement;

        /// Store non-function statements
        std::vector<std::shared_ptr<Statement>> m_statements;

        /// Constructs a program statement
        std::shared_ptr<Statement> buildStatement();

        /// Parses statements of the form
        /// keyword expression -> identifier;
        /// For example
        /// int 5 -> a;
        std::shared_ptr<Statement> parseArrowStatement();

        /// Parses statements of the form
        /// expression -> identifier;
        /// For example
        /// 5 -> a;
        std::shared_ptr<Statement> parseSimpleArrowStatement();

        /// Parses statements of the form
        /// keyword expression;
        /// For example
        /// prn "hello";
        std::shared_ptr<Statement> parseArrowlessStatement();

        /// Bespoke handling for a release statement
        /// For example
        /// release identifier;
        std::shared_ptr<Statement> parseReleaseStatement();

        /// Parses a repeat statement of the form
        /// repeat expression times { statement* }
        /// For example
        /// repeat 5 times { prn "hello"; }
        std::shared_ptr<Statement> parseRepeatStatement();

        /// Parses a while statement of the form
        /// while (expression) { statement* }
        std::shared_ptr<Statement> parseWhileStatement();

        /// Parses a for statement of the form
        /// for identifier in identifier { statement* }
        /// For example
        /// for i in N { prn i; }
        std::shared_ptr<Statement> parseForStatement();

        /// Parses an if statement of the form
        /// if (expression) { statement * } elseif (...) {} else {}
        /// Note the elseif and final else parts are optional
        std::shared_ptr<Statement> parseIfStatement();

        /// Parses a start statement (the program entry point):
        /// start { statement * }
        std::shared_ptr<Statement> parseStartStatement();

        /// Parses an async statement:
        /// start { statement * }
        std::shared_ptr<Statement> parseAsyncStatement();

        /// Parses a function statement of the form
        /// fn:identifier identifier(expression1,..,expressionN) -> identifier
        /// {
        /// }
        std::shared_ptr<Statement> parseFunctionStatement();

        /// Parses a statement of the form
        /// expression;
        /// For example:
        /// function(a, b, c);
        std::shared_ptr<Statement> parseSingleExpressionStatement();

        /// break;
        std::shared_ptr<Statement> parseBreakStatement();

        /// return;
        std::shared_ptr<Statement> parseReturnStatement();

        /// A block of code: E.g.:
        /// {
        ///     statement1;
        ///     statement2;
        ///     statementN; 
        /// }
        std::shared_ptr<Statement> parseScopedBlockStatement();
    };
}
