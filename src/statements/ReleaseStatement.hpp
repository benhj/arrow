/// (c) Ben Jones 2019 - present

#pragma once

#include "ArrowlessStatement.hpp"
#include <memory>

namespace arrow {
    
    class ReleaseStatement : public Statement
    {
      public:
        ReleaseStatement(long const lineNumber);
        ReleaseStatement & withToken(Token token);
        ReleaseStatement & withIdentifier(Token token);
        Token getToken() const;
        Token getIdentifier() const;
        std::string toString() const override;
        std::shared_ptr<StatementEvaluator> getEvaluator() const override;
      private:
        Token m_token;
        Token m_identifier;
    };

}