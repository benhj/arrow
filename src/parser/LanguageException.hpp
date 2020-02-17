/// (c) Ben Jones 2019 - present

#pragma once

#include <exception>
#include <string>
#include <utility>

namespace arrow {
    class LanguageException : public std::exception
    {
      public:
        LanguageException(std::string errorString,
                          long const lineNumber)
          : m_errorString(std::move(errorString))
          , m_lineNumber(lineNumber)
        {
            m_errorStringWithLineNum = m_errorString + " on line number " + std::to_string(m_lineNumber);
        }
        ~LanguageException() throw() {}

        LanguageException & withErrorString(std::string errorString)
        {
            m_errorString = std::move(errorString);
            m_errorStringWithLineNum = m_errorString + " on line number " + std::to_string(m_lineNumber);
            return *this;
        }
        LanguageException & withLineNumber(long const lineNumber)
        {
            m_lineNumber = lineNumber;
            m_errorStringWithLineNum = m_errorString + " on line number " + std::to_string(m_lineNumber);
            return *this;
        }
        char const * report(bool const withLineNumber) const{
            return withLineNumber ? m_errorStringWithLineNum.c_str() : m_errorString.c_str();
        }

        char const * what() const throw (){
            return report(true);
        }

      private:
        std::string m_errorString, m_errorStringWithLineNum;
        long m_lineNumber;
    };
}