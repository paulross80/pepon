#include "Pepon.hpp"
#include "PeponConfig.hpp"

#include <extLibs/Utils/StringUtils.hpp>

namespace pepon
{

// ----------------------------------------------------------------------
// ctor
SyntaxError::SyntaxError(std::string message)
    : std::runtime_error("SyntaxError"),
      m_str(PEPON_LIBRARY_NAME)
{
    m_str += ": " + std::move(message);
}

auto SyntaxError::what() const noexcept -> const char*
{
    return m_str.data();
}


// ----------------------------------------------------------------------
// Recognize types from string format to enum
auto stringTypeToEnum(std::string_view str) -> e_cppTypes
{
    // Integral
    if (str == "int")
        return e_cppTypes::INT;
    if (str == "uint" || str == "unsigned int")
        return e_cppTypes::UINT;

    // Boolean
    if (str == "bool")
        return e_cppTypes::BOOL;

    // Floating point
    if (str == "float")
        return e_cppTypes::FLOAT;
    if (str == "double")
        return e_cppTypes::DOUBLE;

    // String
    if (str == "string")
        return e_cppTypes::STRING_UTF8;

    // Unknown
    return e_cppTypes::UNKNOWN;
}

} // pepon
