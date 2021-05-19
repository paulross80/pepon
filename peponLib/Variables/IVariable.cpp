#include "IVariable.hpp"

namespace pepon
{

// ctor
IVariable::IVariable(std::string name, e_varType varType, e_cppTypes cppType, std::any&& value)
    : m_name(std::move(name)),
      m_varType(varType),
      m_cppType(cppType),
      m_value(std::move(value))
{
}


// Getters:
auto IVariable::getName() const noexcept -> std::string_view
{
    return m_name;
}

auto IVariable::getVarType() const noexcept -> e_varType
{
    return m_varType;
}

auto IVariable::getCppType() const noexcept -> e_cppTypes
{
    return m_cppType;
}

auto IVariable::getAny() -> std::any&
{
    return m_value;
}

auto IVariable::getAny() const -> const std::any&
{
    return m_value;
}

} // pepon
