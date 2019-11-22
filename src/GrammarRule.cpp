#pragma once

#include "Lexeme.hpp"
#include <memory>

namespace arrow {

    class GrammarRule
    {
      public:
        GrammarRule(Lexeme start)
          : m_start(std::move(start))
          , m_next{nullptr}
        {
        }

        GrammarRule & followedBy(std::unique_ptr<GrammarRule> next)
        {
            m_next = std::move(next);
            return *m_next;
        }

      protected:
        Lexeme m_start;
        std::unique_ptr<GrammarRule> m_next;
    };
}


GrammarRule genericStringRule(Lexeme::GENERIC_STRING);
