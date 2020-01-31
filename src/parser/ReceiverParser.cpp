/// (c) 2020 - present

/// This
#include "ReceiverParser.hpp"

/// Receivers
#include "receivers/DollarIdentifierReceiver.hpp"
#include "receivers/IdentifierReceiver.hpp"
#include "receivers/ArrayAccessorReceiver.hpp"
#include "builtin/filesystem/FileReceiver.hpp"

/// Other
#include "LanguageException.hpp"
#include "lexer/Lexeme.hpp"
#include <algorithm>
#include <functional>
#include <utility>
#include <vector>

namespace arrow {

    ReceiverParser::ReceiverParser(TokenManager & tm)
      : m_tm(tm)
      , m_ep(m_tm)
    {
    }

    std::shared_ptr<Receiver> ReceiverParser::parseReceiver()
    {
        static std::vector<std::function<std::shared_ptr<Receiver>(void)>> pvec;
        if(pvec.empty()) {
            pvec.emplace_back([this]{return parseFileReceiver();});
            pvec.emplace_back([this]{return parseArrayAccessorReceiver();});
            pvec.emplace_back([this]{return parseIdentifierReceiver();});
            pvec.emplace_back([this]{return parseDollarIdentifierReceiver();});
        }            
        for(auto const & p : pvec) {
            auto store = m_tm.retrieveIt();
            auto rec = p();
            if(rec) {
                return rec;
            }
            m_tm.revert(store);
        }
        return nullptr;
    }
    std::shared_ptr<Receiver> ReceiverParser::parseIdentifierReceiver()
    {
        if(m_tm.currentToken().lexeme != Lexeme::GENERIC_STRING) {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<IdentifierReceiver>(ln);
        exp->withIdentifierToken(m_tm.currentToken());
        return exp;
    }
    std::shared_ptr<Receiver> ReceiverParser::parseDollarIdentifierReceiver()
    {
        if(m_tm.currentToken().lexeme != Lexeme::DOLLAR_STRING) {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto exp = std::make_shared<DollarIdentifierReceiver>(ln);
        exp->withIdentifierToken(m_tm.currentToken());
        return exp;
    }
    std::shared_ptr<Receiver> ReceiverParser::parseArrayAccessorReceiver()
    {
        if(m_tm.currentToken().lexeme != Lexeme::GENERIC_STRING) {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto rec = std::make_shared<ArrayAccessorReceiver>(ln);
        rec->withIdentifierToken(m_tm.currentToken());
        m_tm.advanceTokenIterator();
        if(m_tm.currentToken().lexeme != Lexeme::COLON) {
            return nullptr;
        }
        m_tm.advanceTokenIterator();
        auto expr = m_ep.parseExpression();
        if(!expr) {
            return nullptr;
        }
        rec->withExpression(std::move(expr));
        return rec;
    }

    std::shared_ptr<Receiver> ReceiverParser::parseFileReceiver()
    {
        if(m_tm.currentToken().lexeme != Lexeme::GENERIC_STRING &&
           m_tm.currentToken().raw != "file") {
            return nullptr;
        }
        auto const ln = m_tm.currentToken().lineNumber;
        auto rec = std::make_shared<FileReceiver>(ln);
        m_tm.advanceTokenIterator();
        if(m_tm.currentToken().lexeme != Lexeme::OPEN_PAREN) {
            return nullptr;
        }
        auto expression = m_ep.parseExpression();
        if(m_tm.currentToken().lexeme != Lexeme::CLOSE_PAREN) {
            return nullptr;
        }
        if(expression) {
            rec->setExpression(std::move(expression));
            return rec;
        }
        return nullptr;
    }
}