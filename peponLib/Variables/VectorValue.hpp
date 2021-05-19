#ifndef VECTOR_CONTAINER_VARIABLE_HPP
#define VECTOR_CONTAINER_VARIABLE_HPP

#include <vector>

#include "IVariable.hpp"

namespace pepon
{

template <typename Type>
class VectorValue final : public IVariable
{
    public:

        // ctor
        VectorValue(std::string name, std::vector<Type> container);

        // Virtual functions override
        void print(const PeponQuotes& quotes, std::ostream& out = std::cout) const override;


        // Getters:
        auto get() noexcept -> std::vector<Type>&;
        auto get() const noexcept -> const std::vector<Type>&;

};


// Template functions implementation:
// ctor
template <typename Type>
VectorValue<Type>::VectorValue(std::string name, std::vector<Type> container)

    : IVariable(std::move(name),
                IVariable::e_varType::VECTOR,
                getTypeEnum<Type>(),
                std::move(container))
{
}

template <typename Type>
void VectorValue<Type>::print(const PeponQuotes& quotes, std::ostream& out) const
{
    out << m_name << " : vector<"
        << getTypeName<Type>()
        << "> = { ";

    const auto& container = std::any_cast<const std::vector<Type>&>(m_value);
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
auto VectorValue<Type>::get() noexcept -> std::vector<Type>&
{
    return m_value;
}

template <typename Type>
auto VectorValue<Type>::get() const noexcept -> const std::vector<Type>&
{
    return m_value;
}

} // pepon


#endif // VECTOR_CONTAINER_VARIABLE_HPP
