/// (c) Ben Jones 2019 - present

#include "FileReceiver.hpp"
#include "evaluator/FileReceiverEvaluator.hpp"

namespace arrow {

    FileReceiver::FileReceiver(long const lineNumber) 
      : Receiver(lineNumber)
    {
    }

    std::shared_ptr<ReceiverEvaluator> FileReceiver::getEvaluator() const
    {
        return std::make_shared<FileReceiverEvaluator>(*this);
    }

    void FileReceiver::setExpression(std::shared_ptr<Expression> expression)
    {
        m_expression = std::move(expression);
    }
    std::shared_ptr<Expression>
    FileReceiver::getExpression() const
    {
        return m_expression;
    }

    std::string FileReceiver::toString() const
    {
        return ""; // todo
    }
}