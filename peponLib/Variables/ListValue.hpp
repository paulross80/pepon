#ifndef LIST_CONTAINER_VARIABLE_HPP
#define LIST_CONTAINER_VARIABLE_HPP

#include <list>

#include "IVariable.hpp"

namespace pepon
{

template <typename Type>
class ListValue final : public IVariable
{
    public:

        // ctor
        ListValue(std::string name, std::list<Type> container);

        // Virtual functions override
        void print(const PeponQuotes& quotes, std::ostream& out = std::cout) const override;


        // Getters:
        auto get() noexcept -> std::list<Type>&;
        auto get() const noexcept -> const std::list<Type>&;

};


// Template functions implementation:
// ctor
template <typename Type>
ListValue<Type>::ListValue(std::string name, std::list<Type> container)

    : IVariable(std::move(name),
                IVariable::e_varType::LIST,
                getTypeEnum<Type>(),
                std::move(container))
{
}

template <typename Type>
void ListValue<Type>::print(const PeponQuotes& quotes, std::ostream& out) const
{
    out << m_name << " : list<"
        << getTypeName<Type>()
        << "> = { ";

    const auto& container = std::any_cast<const std::list<Type>&>(m_value);
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
auto ListValue<Type>::get() noexcept -> std::list<Type>&
{
    return m_value;
}

template <typename Type>
auto ListValue<Type>::get() const noexcept -> const std::list<Type>&
{
    return m_value;
}

} // pepon


#endif // LIST_CONTAINER_VARIABLE_HPP
