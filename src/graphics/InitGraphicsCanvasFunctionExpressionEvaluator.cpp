/// (c) Ben Jones 2025 - present

#include "GraphicsCanvas.hpp"
#include "InitGraphicsCanvasFunctionExpressionEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include "parser/Parser.hpp"
#include <utility>

namespace arrow {

    InitGraphicsCanvasFunctionExpressionEvaluator::InitGraphicsCanvasFunctionExpressionEvaluator(InitGraphicsCanvasFunctionExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type InitGraphicsCanvasFunctionExpressionEvaluator::evaluate(Environment & environment) const
    {
        // Pull out the name of the function
        auto const callLineNumber = m_expression.getLineNumber();

        // Pull out the arguments being passed into the function
        auto const expression = m_expression.getExpression();
        auto const col = expression->getEvaluator()->evaluate(environment);
        auto & expressionCollEval = std::get<std::vector<Type>>(col.m_variantType);
        if(expressionCollEval.size() < 4) {
            throw LanguageException("Too few arguments", callLineNumber);
        }

        auto const width = expressionCollEval[0];
        auto const height = expressionCollEval[1];
        auto const pixelWidth = expressionCollEval[2];
        auto const pixelHeight = expressionCollEval[3];

        if(width.m_descriptor != TypeDescriptor::Int ||
           height.m_descriptor != TypeDescriptor::Int ||
           pixelWidth.m_descriptor != TypeDescriptor::Int ||
           pixelHeight.m_descriptor != TypeDescriptor::Int) {
            throw LanguageException("Wrong argument type", callLineNumber);
        }

        // Proceed
        auto castedWidth = std::get<int64_t>(width.m_variantType);
        auto castedHeight = std::get<int64_t>(height.m_variantType);
        auto castedPixelWidth = std::get<int64_t>(pixelWidth.m_variantType);
        auto castedPixelHeight = std::get<int64_t>(pixelHeight.m_variantType);

        GraphicsCanvas::construct(castedWidth, castedHeight,
            castedPixelWidth, castedPixelHeight);

        return {TypeDescriptor::Bool, true};
    }
}