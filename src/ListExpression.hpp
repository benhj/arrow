#pragma once

#include "Expression.hpp"
#include "Token.hpp"
#include <memory>
#include <vector>

namespace jasl {
    
    class ListExpression : public Expression
    {
      public:
        ListExpression() 
        : Expression()
        , m_parts{}
        {
        }

        void addPart(std::shared_ptr<Expression> expression)
        {
            m_parts.emplace_back(std::move(expression));
        }

        int getPartsCount() const
        {
            return m_parts.size();
        }
        
        std::string toString() const override 
        {
            std::string str("Begin list expression");
            for(auto const & part : m_parts) {
                str.append("\n");
                str.append(part->toString());
            }
            str.append("\nEnd list expression");
            return str;
        }

      private:
        std::vector<std::shared_ptr<Expression>> m_parts;
    };

}