#ifndef SINGLE_VALUE_VARIABLE_HPP
#define SINGLE_VALUE_VARIABLE_HPP

#include "IVariable.hpp"

namespace pepon
{

// Non-container value
template <typename Type>
class SingleValue final : public IVariable
{
    public:

        // ctor
        SingleValue(std::string name, Type value);

        // Virtual functions override
        void print(const PeponQuotes& quotes, std::ostream& out = std::cout) const override;


        // Get reference
        auto get() -> std::any&;
        auto get() const -> const std::any&;

        // Get value
        auto getValue() -> Type;
        auto getValue() const -> const Type&;

};


// Template functions implementation:
// ctor
template <typename Type>
SingleValue<Type>::SingleValue(std::string name, Type value)

    : IVariable(std::move(name),
                IVariable::e_varType::SINGLE_VALUE,
                getTypeEnum<Type>(),
                std::make_any<Type>(value))
{
}


template <typename Type>
void SingleValue<Type>::print(const PeponQuotes& quotes, std::ostream& out) const
{
    out << m_name << " : ";
    out << getTypeName<Type>() << " = ";

    if (!m_value.has_value())
    {
        out << "UNINITIALIZED!" << '\n';
        return;
    }

    if (quotes.useQuotes)
        out << quotes.quotesChar;

    out << std::any_cast<Type>(m_value);

    if (quotes.useQuotes)
        out << quotes.quotesChar;

    out << std::endl;

}


template <>
void SingleValue<bool>::print(const PeponQuotes& quotes, std::ostream& out) const;

template <>
void SingleValue<std::string>::print(const PeponQuotes& quotes, std::ostream& out) const;


template <typename Type>
auto SingleValue<Type>::getValue() -> Type
{
    return std::any_cast<Type>(m_value);
}

template <typename Type>
auto SingleValue<Type>::getValue() const -> const Type&
{
    return std::any_cast<const Type&>(m_value);
}

} // pepon


#endif // SINGLE_VALUE_VARIABLE_HPP
