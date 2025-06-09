/// (c) Ben Jones 2025 - present

#include "GraphicsCanvas.hpp"
#include "FillRectFunctionExpressionEvaluator.hpp"
#include "expressions/evaluator/ExpressionEvaluator.hpp"
#include "parser/LanguageException.hpp"
#include "parser/Parser.hpp"
#include <utility>

namespace arrow {

    FillRectFunctionExpressionEvaluator::FillRectFunctionExpressionEvaluator(FillRectFunctionExpression expression)
      : m_expression(std::move(expression))
    {
    }
    Type FillRectFunctionExpressionEvaluator::evaluate(Environment & environment) const
    {
        // Pull out the name of the function
        auto const callLineNumber = m_expression.getLineNumber();

        // Pull out the arguments being passed into the function
        auto const expression = m_expression.getExpression();
        auto const col = expression->getEvaluator()->evaluate(environment);
        auto & expressionCollEval = std::get<std::vector<Type>>(col.m_variantType);
        if(expressionCollEval.size() < 6) {
            throw LanguageException("Too few arguments", callLineNumber);
        }

        auto const x = expressionCollEval[0];
        auto const y = expressionCollEval[1];
        auto const w = expressionCollEval[2];
        auto const h = expressionCollEval[3];
        auto const r = expressionCollEval[4];
        auto const g = expressionCollEval[5];
        auto const b = expressionCollEval[6];

        if(x.m_descriptor != TypeDescriptor::Int ||
           y.m_descriptor != TypeDescriptor::Int ||
           w.m_descriptor != TypeDescriptor::Int ||
           h.m_descriptor != TypeDescriptor::Int ||
           r.m_descriptor != TypeDescriptor::Int ||
           g.m_descriptor != TypeDescriptor::Int ||
           b.m_descriptor != TypeDescriptor::Int) {
            throw LanguageException("Wrong argument type", callLineNumber);
        }

        // Proceed
        auto castedX = std::get<int64_t>(x.m_variantType);
        auto castedY = std::get<int64_t>(y.m_variantType);
        auto castedW = std::get<int64_t>(w.m_variantType);
        auto castedH = std::get<int64_t>(h.m_variantType);
        auto castedR = std::get<int64_t>(r.m_variantType);
        auto castedG = std::get<int64_t>(g.m_variantType);
        auto castedB = std::get<int64_t>(b.m_variantType);

        GraphicsCanvas::fillRect(castedX, castedY, castedW, castedH, castedR, castedG, castedB);
        return {TypeDescriptor::Bool, true};
    }
}