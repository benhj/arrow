/// (c) Ben Jones 2019 - present

#include "FileReceiverEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "receivers/evaluator/ReceiverEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include <utility>

#include <cstdint>
#include <fstream>
#include <stdexcept>
#include <string>

namespace arrow {

    FileReceiverEvaluator::FileReceiverEvaluator
    (FileReceiver receiver)
      : m_receiver(std::move(receiver))
    {
    }
    void FileReceiverEvaluator::evaluate(Type incoming, Environment & environment) const
    {
        // Pull out the name of the function
        auto const callLineNumber = m_receiver.getLineNumber();

        // Pull out the arguments being passed into the function
        auto const expression = m_receiver.getExpression();
        auto const fname = expression->getEvaluator()->evaluate(environment);
        if(fname.m_descriptor != TypeDescriptor::String) {
            throw LanguageException("Expects a string path", callLineNumber);
        }
        if(incoming.m_descriptor != TypeDescriptor::Bytes) {
            throw LanguageException("Expects a bytes array", callLineNumber);
        }

        try {
            auto const & bytes = std::get<std::vector<char>>(incoming.m_variantType);
            auto const filename = std::get<std::string>(fname.m_variantType);
            std::ofstream file(filename, std::ios::binary);
            if(!file.is_open()) {
                throw LanguageException("Problem opening file for writing", callLineNumber);
            }
            file.write(reinterpret_cast<char const*>(&bytes.front()), bytes.size());
            file.close();
        } catch (...) {
            throw LanguageException("Problem with filesystem command", callLineNumber);
        }
    }
}