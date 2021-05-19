#ifndef PEPON_LIBRARY_HPP
#define PEPON_LIBRARY_HPP

#include <tuple>
#include <string>
#include <string_view>
#include <stdexcept>


// Library configuration
#ifdef PEPON_USE_STL_PATH
#include <filesystem>
#endif


namespace pepon
{

// ----------------------------------------------------------------------
// Enable/disable quotations
// ----------------------------------------------------------------------
struct PeponQuotes
{
    bool useQuotes  = true;
    char quotesChar = '\'';
};


// ----------------------------------------------------------------------
// Exception class
// ----------------------------------------------------------------------
class SyntaxError final : public std::runtime_error
{
    public:

        // ctor
        SyntaxError(std::string message);

        // Explain what happened
        auto what() const noexcept -> const char* override;

    private:

        std::string m_str;

};


// ----------------------------------------------------------------------
// C++ Fundamental types
// https://en.cppreference.com/w/cpp/language/types
// ----------------------------------------------------------------------
enum class e_cppTypes
{
    // Integral types
    INT,    // Signed
    UINT,   // Unsigned

    // Boolean
    BOOL,

    // Floating point
    FLOAT,
    DOUBLE,

    // String types
    STRING_UTF8,

    // Error
    UNKNOWN = -1
};


// Short version
using uint = unsigned int;


// ----------------------------------------------------------------------
// typeid(T)::name() can give some info,
// but the standard does not mandate this string to be human-readable
// ----------------------------------------------------------------------
template <typename Type>
inline auto getTypeName() -> std::string_view { return "Unknown"; }


// Specializations:
// Integral types
template <>
inline auto getTypeName<int>() -> std::string_view         { return "int";    }

template <>
inline auto getTypeName<uint>() -> std::string_view        { return "uint";   }

// Boolean type
template <>
inline auto getTypeName<bool>() -> std::string_view        { return "bool";   }

// Floating point types
template <>
inline auto getTypeName<float>() -> std::string_view       { return "float";  }

template <>
inline auto getTypeName<double>() -> std::string_view      { return "double"; }

// String type
template <>
inline auto getTypeName<std::string>() -> std::string_view { return "string"; }


// ----------------------------------------------------------------------
template <typename Type>
inline auto getTypeEnum() -> e_cppTypes { return e_cppTypes::UNKNOWN; }


// Specializations:
// Integral types
template <>
inline auto getTypeEnum<int>() -> e_cppTypes         { return e_cppTypes::INT;    }

template <>
inline auto getTypeEnum<uint>() -> e_cppTypes        { return e_cppTypes::UINT;   }

// Boolean type
template <>
inline auto getTypeEnum<bool>() -> e_cppTypes        { return e_cppTypes::BOOL;   }

// Floating point types
template <>
inline auto getTypeEnum<float>() -> e_cppTypes       { return e_cppTypes::FLOAT;  }

template <>
inline auto getTypeEnum<double>() -> e_cppTypes      { return e_cppTypes::DOUBLE; }

// String type
template <>
inline auto getTypeEnum<std::string>() -> e_cppTypes { return e_cppTypes::STRING_UTF8; }


// ----------------------------------------------------------------------
// Free functions
// ----------------------------------------------------------------------
// Recognize types from string format to enum
auto stringTypeToEnum(std::string_view str) -> e_cppTypes;

} // pepon


#endif // PEPON_LIBRARY_HPP
