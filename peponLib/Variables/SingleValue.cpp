#include "SingleValue.hpp"

namespace pepon
{

template <>
void SingleValue<bool>::print(const PeponQuotes& quotes, std::ostream& out) const
{
    out << m_name << " : bool = ";

    if (!m_value.has_value())
    {
        out << "UNINITIALIZED!" << '\n';
        return;
    }

    if (quotes.useQuotes)
        out << quotes.quotesChar;

    out << std::boolalpha << std::any_cast<bool>(m_value);

    if (quotes.useQuotes)
        out << quotes.quotesChar;

    out << std::endl;

}


template <>
void SingleValue<std::string>::print([[maybe_unused]] const PeponQuotes& quotes, std::ostream& out) const
{
    out << m_name << " : string = ";

    if (!m_value.has_value())
    {
        out << "UNINITIALIZED!" << '\n';
        return;
    }

    out << "\"";

    out << std::any_cast<const std::string&>(m_value);

    out << "\"" << std::endl;

}

} // pepon
