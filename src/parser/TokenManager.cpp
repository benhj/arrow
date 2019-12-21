/// (c) Ben Jones 2019 - present

#include "TokenManager.hpp"
#include <utility>

namespace arrow {

    TokenManager::TokenManager(std::vector<Token> tokens)
      : m_tokens(std::move(tokens))
      , m_current(std::begin(m_tokens))
    {}

    bool TokenManager::notAtEnd() const
    {
        return m_current != std::end(m_tokens);
    }
    bool TokenManager::tokenPlusOneNotAtEnd() const
    {
        return m_current + 1 != std::end(m_tokens);
    }
    void TokenManager::advanceTokenIterator()
    {
        ++m_current;
        // skip over comments
        while(m_current->lexeme == Lexeme::COMMENT) {
            ++m_current;
        }
    }
    Token TokenManager::currentToken() const
    {
        if(m_current == std::end(m_tokens)) {
            throw std::runtime_error("Parse error");
        }
        return *m_current;
    }
    Token TokenManager::nextToken() const
    {
        if(m_current + 1 == std::end(m_tokens)) {
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