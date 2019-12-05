/// (c) Ben Jones 2019 - present

#include "ListWordExpression.hpp"
#include "evaluator/ExpressionEvaluator.hpp"
#include <utility>

namespace arrow {

    ListWordExpression::ListWordExpression(long const lineNumber) 
      : Expression(lineNumber)
      , m_word()
    {
    }

    std::shared_ptr<ExpressionEvaluator> 
    ListWordExpression::getEvaluator() const
    {
        struct ListWordEvaluator : public ExpressionEvaluator {
            ListWordEvaluator(Token tok) : m_tok(std::move(tok))
            {
            }

            Type evaluate(Cache & cache) const override
            {
                return {TypeDescriptor::ListWord, m_tok.raw};
            }
          private:
            Token m_tok;
        };
        return std::make_shared<ListWordEvaluator>(m_word);
    }

    ListWordExpression & ListWordExpression::withWordToken(Token word)
    {
        m_word = std::move(word);
        return *this;
    }

    Token ListWordExpression::getWordToken() const
    {
        return  m_word;
    }

    std::string ListWordExpression::toString() const
    {
        std::string str("\nList word expression: ");
        return str.append(m_word.raw);
    }
}