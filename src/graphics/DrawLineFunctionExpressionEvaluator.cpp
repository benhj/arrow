/// (c) Ben Jones 2025 - present

#include "GraphicsCanvas.hpp"
#include "DrawLineFunctionExpressionEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include "parser/Parser.hpp"
#include <utility>

namespace arrow {

    DrawLineFunctionExpressionEvaluator::DrawLineFunctionExpressionEvaluator(DrawLineFunctionExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type DrawLineFunctionExpressionEvaluator::evaluate(Environment & environment) const
    {
        // Pull out the name of the function
        auto const callLineNumber = m_expression.getLineNumber();

        // Pull out the arguments being passed into the function
        auto const expression = m_expression.getExpression();
        auto const col = expression->getEvaluator()->evaluate(environment);
        auto & expressionCollEval = std::get<std::vector<Type>>(col.m_variantType);
        if(expressionCollEval.size() < 7) {
            throw LanguageException("Too few arguments", callLineNumber);
        }

        auto const x1 = expressionCollEval[0];
        auto const y1 = expressionCollEval[1];
        auto const x2 = expressionCollEval[2];
        auto const y2 = expressionCollEval[3];
        auto const r = expressionCollEval[4];
        auto const g = expressionCollEval[5];
        auto const b = expressionCollEval[6];

        if(x1.m_descriptor != TypeDescriptor::Int ||
           y1.m_descriptor != TypeDescriptor::Int ||
           x2.m_descriptor != TypeDescriptor::Int ||
           y2.m_descriptor != TypeDescriptor::Int ||
           r.m_descriptor != TypeDescriptor::Int ||
           g.m_descriptor != TypeDescriptor::Int ||
           b.m_descriptor != TypeDescriptor::Int) {
            throw LanguageException("Wrong argument type", callLineNumber);
        }

        // Proceed
        auto castedX1 = std::get<int64_t>(x1.m_variantType);
        auto castedY1 = std::get<int64_t>(y1.m_variantType);
        auto castedX2 = std::get<int64_t>(x2.m_variantType);
        auto castedY2 = std::get<int64_t>(y2.m_variantType);
        auto castedR = std::get<int64_t>(r.m_variantType);
        auto castedG = std::get<int64_t>(g.m_variantType);
        auto castedB = std::get<int64_t>(b.m_variantType);

        GraphicsCanvas::drawLine(castedX1, castedY1, castedX2, castedY2,
            castedR, castedG, castedB);
        return {TypeDescriptor::Bool, true};
    }
}