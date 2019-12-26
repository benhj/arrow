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
        }
        ~LanguageException() throw(){}

        LanguageException & withErrorString(std::string errorString)
        {
            m_errorString = std::move(errorString);
            return *this;
        }
        LanguageException & withLineNumber(std::string errorString)
        {
            m_errorString = std::move(errorString);
            return *this;
        }
        char const * report(bool const withLineNumber) const{
            std::string errorStr(m_errorString);
            if(withLineNumber) {
                errorStr.append(" on line number ")
                        .append(std::to_string(m_lineNumber));
            }
            return errorStr.c_str();
        }

        char const * what() const throw (){
            return report(true);
        }

      private:
        std::string m_errorString;
        long m_lineNumber;
    };
}