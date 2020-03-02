/// (c) Ben Jones 2019 - present

#include "HTTPReadExpressionEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"

#include <curl/curl.h>

#include <cstdint>
#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

    inline
    size_t write_data(const char * data, size_t size,
        size_t count, std::vector<uint8_t> * bytes) {
        for(int c = 0; c < count; ++c) {
            bytes->push_back(data[c]);
        }
        //stream->write(data, count);
        return size * count;
    }

    inline
    int pull_one_url(std::string const & url,
                     std::vector<char> & bytes)
    {
        CURL *curl;
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &bytes);
            curl_easy_perform(curl);
            /* always cleanup */
            curl_easy_cleanup(curl);
            return 0;
        }
        return 1;
    }
}


namespace arrow {

    HTTPReadExpressionEvaluator::HTTPReadExpressionEvaluator
    (HTTPReadExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type HTTPReadExpressionEvaluator::evaluate(Environment & environment) const
    {
        // Pull out the name of the function
        auto const callLineNumber = m_expression.getLineNumber();

        // Pull out the arguments being passed into the function
        auto const expression = m_expression.getExpression();
        auto const col = expression->getEvaluator()->evaluate(environment);
        auto const expressionCollEval = std::get<std::vector<Type>>(col.m_variantType);
        if(expressionCollEval.empty()) {
            throw LanguageException("Expected argument", m_expression.getLineNumber());
        }
        auto const t = expressionCollEval[0];
        if(t.m_descriptor != TypeDescriptor::String) {
            throw LanguageException("Expects a string expression", callLineNumber);
        }
        try {
            auto const url = std::get<std::string>(t.m_variantType);
            std::vector<char> bytes;
            if(pull_one_url(url, bytes) == 0) {
                return {TypeDescriptor::Bytes, bytes};
            }
        } catch (...) {
        }
        throw LanguageException("Problem with http_read command", callLineNumber);
    }
}