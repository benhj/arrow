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
        char const * what() const throw (){
           return std::string(m_errorString)
                  .append(" on line number ")
                  .append(std::to_string(m_lineNumber))
                  .c_str();
        }

      private:
        std::string m_errorString;
        long m_lineNumber;
    };
}