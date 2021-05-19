#include <charconv>     // std::from_chars

#include <myoga-utils/Utils/StringUtils.hpp>
#include <myoga-utils/Utils/TemplateUtils.hpp>

#include "Group.hpp"

#include "SingleValue.hpp"
#include "VectorValue.hpp"
#include "DequeValue.hpp"
#include "ListValue.hpp"

namespace pepon
{

// ctor
Group::Group(std::string_view name)
    : m_name(name)
{
}


// Get a view of the name
auto Group::getName() const noexcept -> std::string_view
{
    return m_name;
}

auto Group::getVars() noexcept -> t_variablesContainer&
{
    return m_vars;
}

auto Group::getVars() const noexcept -> const t_variablesContainer&
{
    return m_vars;
}

auto Group::getComments() const noexcept -> const t_commentsContainer&
{
    return m_comments;
}


void Group::print(const PeponQuotes& quotes, std::ostream& out, e_commentStyle style) const
{

    // Write comments
    for (auto& comment : m_comments)
    {
        switch(style)
        {
            case e_commentStyle::CPP:
                out << "// ";
                break;
            case e_commentStyle::LUA:
                out << "-- ";
                break;
            case e_commentStyle::PYTHON:
                out << "# ";
                break;
        };

        out << comment << std::endl;
    }


    // Write group name
    out << '[' << m_name << ']' << std::endl;

    // Write every value
    for (auto& varPtr : m_vars)
        varPtr->print(quotes, out);

}


// Adds a single value
void Group::addVariable(std::string_view name, std::string_view value, e_cppTypes typeAsEnum)
{

    // Unlike other parsing functions in C++ and C libraries, std::from_chars is locale-independent, non-allocating, and non-throwing
    // https://en.cppreference.com/w/cpp/utility/from_chars
    switch(typeAsEnum)
    {
        case e_cppTypes::INT:
        {
            int result = 0;
            std::from_chars(value.data(), value.data() + value.size(), result);
            m_vars.emplace_back(std::make_unique<SingleValue<int>>(std::string(name), result));
        }
            break;

        case e_cppTypes::UINT:
        {
            uint result = 0U;
            std::from_chars(value.data(), value.data() + value.size(), result);
            m_vars.emplace_back(std::make_unique<SingleValue<uint>>(std::string(name), result));
        }
            break;

        case e_cppTypes::BOOL:
            m_vars.emplace_back(std::make_unique<SingleValue<bool>>(std::string(name), myoga::boolFromStr(value)));
            break;

        case e_cppTypes::FLOAT:
        {
            //float result = 0.0f;
            // DOESN'T WORK IN GCC!
            //std::from_chars(varValue.data(), varValue.data() + varValue.size(), result);
            //std::istringstream(varValue.data()) >> result;
            // Why stoX only works with string ???
            float result = std::stof(std::string(value));
            m_vars.emplace_back(std::make_unique<SingleValue<float>>(std::string(name), result));
        }
            break;

        case e_cppTypes::DOUBLE:
        {
            double result = std::stod(std::string(value));
            m_vars.emplace_back(std::make_unique<SingleValue<double>>(std::string(name), result));
        }
            break;

        case e_cppTypes::STRING_UTF8:
            m_vars.emplace_back(std::make_unique<SingleValue<std::string>>(std::string(name), std::string(value)));
            break;

        case e_cppTypes::UNKNOWN:
            throw SyntaxError("Unknown variable type for \'" + std::string(name) + '\'');
            break;
    }

}


// Adds a container of values
void Group::addContainer(std::string_view name, std::string_view containerType, e_cppTypes typeAsEnum, std::string_view values)
{

    // ---------------------------------
    // VECTOR:
    // ---------------------------------
    if (containerType == "vector")
    {

        // Vector construction
        switch(typeAsEnum)
        {
            case e_cppTypes::INT:
                m_vars.emplace_back(std::make_unique<VectorValue<int>>(std::string(name), myoga::parseCSV<std::vector<int>>(values)));
                break;

            case e_cppTypes::UINT:
                m_vars.emplace_back(std::make_unique<VectorValue<uint>>(std::string(name), myoga::parseCSV<std::vector<uint>>(values)));
                break;

            case e_cppTypes::BOOL:
                throw SyntaxError("vector<bool> is not supported/not implemented");
                break;

            case e_cppTypes::FLOAT:
                m_vars.emplace_back(std::make_unique<VectorValue<float>>(std::string(name), myoga::parseCSV<std::vector<float>>(values)));
                break;

            case e_cppTypes::DOUBLE:
                m_vars.emplace_back(std::make_unique<VectorValue<double>>(std::string(name), myoga::parseCSV<std::vector<double>>(values)));
                break;

            case e_cppTypes::STRING_UTF8:
                m_vars.emplace_back(std::make_unique<VectorValue<std::string>>(std::string(name), myoga::parseCSStrings<std::vector<std::string>>(std::string(values))));
                break;

            case e_cppTypes::UNKNOWN:
                throw SyntaxError("Unknown variable type for " + std::string(name));
                break;
        }

    }


    // ---------------------------------
    // DEQUE:
    // ---------------------------------
    else if (containerType == "deque")
    {

        // Deque construction
        switch(typeAsEnum)
        {
            case e_cppTypes::INT:
                m_vars.emplace_back(std::make_unique<DequeValue<int>>(std::string(name), myoga::parseCSV<std::deque<int>>(values)));
                break;

            case e_cppTypes::UINT:
                m_vars.emplace_back(std::make_unique<DequeValue<uint>>(std::string(name), myoga::parseCSV<std::deque<uint>>(values)));
                break;

            case e_cppTypes::BOOL:

                m_vars.emplace_back(std::make_unique<DequeValue<bool>>(std::string(name), myoga::parseCSV<std::deque<bool>>(values)));
                break;

            case e_cppTypes::FLOAT:
                m_vars.emplace_back(std::make_unique<DequeValue<float>>(std::string(name), myoga::parseCSV<std::deque<float>>(values)));
                break;

            case e_cppTypes::DOUBLE:
                m_vars.emplace_back(std::make_unique<DequeValue<double>>(std::string(name), myoga::parseCSV<std::deque<double>>(values)));
                break;

            case e_cppTypes::STRING_UTF8:
                m_vars.emplace_back(std::make_unique<DequeValue<std::string>>(std::string(name), myoga::parseCSStrings<std::deque<std::string>>(std::string(values))));
                break;

            case e_cppTypes::UNKNOWN:
                throw SyntaxError("Unknown variable type for " + std::string(name));
                break;
        }

    }


    // ---------------------------------
    // LIST:
    // ---------------------------------
    else if (containerType == "list")
    {

        // List construction
        switch(typeAsEnum)
        {
            case e_cppTypes::INT:
                m_vars.emplace_back(std::make_unique<ListValue<int>>(std::string(name), myoga::parseCSV<std::list<int>>(values)));
                break;

            case e_cppTypes::UINT:
                m_vars.emplace_back(std::make_unique<ListValue<uint>>(std::string(name), myoga::parseCSV<std::list<uint>>(values)));
                break;

            case e_cppTypes::BOOL:

                m_vars.emplace_back(std::make_unique<ListValue<bool>>(std::string(name), myoga::parseCSV<std::list<bool>>(values)));
                break;

            case e_cppTypes::FLOAT:
                m_vars.emplace_back(std::make_unique<ListValue<float>>(std::string(name), myoga::parseCSV<std::list<float>>(values)));
                break;

            case e_cppTypes::DOUBLE:
                m_vars.emplace_back(std::make_unique<ListValue<double>>(std::string(name), myoga::parseCSV<std::list<double>>(values)));
                break;

            case e_cppTypes::STRING_UTF8:
                m_vars.emplace_back(std::make_unique<ListValue<std::string>>(std::string(name), myoga::parseCSStrings<std::list<std::string>>(std::string(values))));
                break;

            case e_cppTypes::UNKNOWN:
                throw SyntaxError("Unknown variable type for " + std::string(name));
                break;
        }

    }

}


// Comments are mine
void Group::takeOwnershipOfComments(t_commentsContainer comments)
{
    m_comments = std::move(comments);
}

} // pepon
