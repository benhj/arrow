/// (c) Ben Jones 2019 - present

#include "TokenManager.hpp"
#include <utility>

namespace arrow {

    TokenManager::TokenManager(std::vector<Token> tokens)
      : m_tokens(std::move(tokens))
      , m_current(std::begin(m_tokens))
      , m_end(std::end(m_tokens))
    {
    }

    bool TokenManager::notAtEnd() const
    {
        return m_current != m_end;
    }
    bool TokenManager::tokenPlusOneNotAtEnd() const
    {
        return (m_current + 1) != m_end;
    }
    void TokenManager::advanceTokenIterator()
    {
        if(m_current == m_end) {
            throw std::runtime_error("Parse error");
        }
        ++m_current;
        // skip over comments
        while(m_current != m_end && m_current->lexeme == Lexeme::COMMENT) {
            ++m_current;
        }
    }
    Token TokenManager::currentToken() const
    {
        if(m_current == m_end) {
            throw std::runtime_error("Parse error");
        }
        return *m_current;
    }
    Token TokenManager::nextToken() const
    {
        if((m_current + 1) == m_end) {
            throw std::runtime_error("Parse error");
        }
        return *(m_current + 1);
    }

    std::vector<Token>::iterator
    TokenManager::retrieveIt() const
    {
        return m_current;
    }
    void TokenManager::revert(std::vector<Token>::iterator orig)
    {
        m_current = orig;
    }
}