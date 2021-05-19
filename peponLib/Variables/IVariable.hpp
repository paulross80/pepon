#ifndef VARIABLES_INTERFACE_HPP
#define VARIABLES_INTERFACE_HPP

#include <any>
#include <string>
#include <memory>
#include <iostream>

#include "../Pepon.hpp"

namespace pepon
{

// Define variables using std::any
class IVariable
{

    public:

        enum class e_varType
        {
            SINGLE_VALUE,

            // https://en.cppreference.com/w/cpp/container
            // Sequence containers
            VECTOR,
            DEQUE,
            LIST,
            //FORWARD_LIST      // seriously, who designed that thing?
        };

    public:

        // ctor
        IVariable(std::string name, e_varType varType, e_cppTypes cppType, std::any&& value);

        // dtor
        virtual ~IVariable() = default;


        // Virtual functions:
        // Print value
        virtual void print(const PeponQuotes& quotes, std::ostream& out = std::cout) const = 0;

        // Getters:
        auto getName() const noexcept -> std::string_view;
        auto getVarType() const noexcept -> e_varType;
        auto getCppType() const noexcept -> e_cppTypes;

        auto getAny() -> std::any&;
        auto getAny() const -> const std::any&;


    protected:


        // Variable name
        std::string m_name;
        // Variable type:
        // single value or container?
        e_varType m_varType;
        // C++ type (enum)
        e_cppTypes m_cppType;
        // Value
        std::any m_value;

};


// Define smart pointer to it
using t_varSmartPtr = std::unique_ptr<IVariable>;

} // pepon


#endif // VARIABLES_INTERFACE_HPP
