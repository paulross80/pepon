#ifndef GROUP_OF_VARIABLES_HPP
#define GROUP_OF_VARIABLES_HPP

#include <vector>

#include "IVariable.hpp"

namespace pepon
{

// Represents a [Group] of variables
class Group
{

    public:

        using t_variablesContainer = std::vector<t_varSmartPtr>;
        using t_commentsContainer  = std::vector<std::string>;

        enum class e_commentStyle { CPP, LUA, PYTHON };

    public:

        // ctor
        Group(std::string_view name);


        // Get a view of the name
        auto getName() const noexcept -> std::string_view;

        // Users can read and modify values
        auto getVars() noexcept -> t_variablesContainer&;
        auto getVars() const noexcept -> const t_variablesContainer&;

        // Comments can only be read
        auto getComments() const noexcept -> const t_commentsContainer&;

        // Utility
        void print(const PeponQuotes& quotes, std::ostream& out = std::cout, e_commentStyle style = e_commentStyle::CPP) const;

        // Adds a single value
        void addVariable(std::string_view name, std::string_view value, e_cppTypes typeAsEnum);
        // Adds a container of values
        void addContainer(std::string_view name, std::string_view containerType, e_cppTypes typeAsEnum, std::string_view values);
        // Comments are mine
        void takeOwnershipOfComments(t_commentsContainer comments);


    private:

        // Group name
        std::string m_name;

        // Variables
        t_variablesContainer m_vars;

        // Comments
        t_commentsContainer m_comments;

};

} // pepon


#endif // GROUP_OF_VARIABLES_HPP
