#ifndef DEQUE_CONTAINER_VARIABLE_HPP
#define DEQUE_CONTAINER_VARIABLE_HPP

#include <deque>

#include "IVariable.hpp"

namespace pepon
{

template <typename Type>
class DequeValue final : public IVariable
{
    public:

        // ctor
        DequeValue(std::string name, std::deque<Type> container);

        // Virtual functions override
        void print(const PeponQuotes& quotes, std::ostream& out = std::cout) const override;


        // Getters:
        auto get() noexcept -> std::deque<Type>&;
        auto get() const noexcept -> const std::deque<Type>&;

};


// Template functions implementation:
// ctor
template <typename Type>
DequeValue<Type>::DequeValue(std::string name, std::deque<Type> container)

    : IVariable(std::move(name),
                IVariable::e_varType::DEQUE,
                getTypeEnum<Type>(),
                std::move(container))
{
}


template <typename Type>
void DequeValue<Type>::print(const PeponQuotes& quotes, std::ostream& out) const
{
    out << m_name << " : deque<"
        << getTypeName<Type>()
        << "> = { ";

    const auto& container = std::any_cast<const std::deque<Type>&>(m_value);
    std::size_t shown = 0U;

    for (const auto& elem : container)
    {
        if (quotes.useQuotes)
            out << quotes.quotesChar;

        if (m_cppType == e_cppTypes::BOOL)
            out << std::boolalpha << elem;
        else
            out << elem;

        if (quotes.useQuotes)
            out << quotes.quotesChar;

        if (shown < container.size() - 1)
            out << ", ";

        shown++;
    }

    out << " }" << std::endl;
}


// Getters:
template <typename Type>
auto DequeValue<Type>::get() noexcept -> std::deque<Type>&
{
    return m_value;
}

template <typename Type>
auto DequeValue<Type>::get() const noexcept -> const std::deque<Type>&
{
    return m_value;
}

} // pepon


#endif // DEQUE_CONTAINER_VARIABLE_HPP
