#ifndef PEPON_FILE_HPP
#define PEPON_FILE_HPP

#include <vector>
#include <unordered_map>

#include "Pepon.hpp"

#include "../Variables/Group.hpp"

namespace pepon
{

// Pepon file reader/writer class
class PeponFile
{

    public:

    #ifdef PEPON_USE_STL_PATH
        using t_filePath = std::filesystem::path;
    #else
        using t_filePath = std::string_view;
    #endif

    public:

        // Interface:
        // File load and save functions:
        // Load and parse an entire Pepon file from disk
        void loadFile(t_filePath fileName);
        // Save Pepon data to a file on disk
        void saveFile(t_filePath fileName);

        // Load and parse Pepon data from strings
        void loadFromMemory(const std::vector<std::string>& lines);

        // Getters:
        // Get group by reference/const reference
        auto getGroup(std::string_view name) -> Group&;
        auto getGroup(std::string_view name) const -> const Group&;

        // Setters:
        // Specify quotations use and style
        void useQuotation(PeponQuotes quotes);

        // Shows every group and its content
        void print(std::ostream& out = std::cout) const;


    private:

        // Private functions:
        // Gets variable name and type
        // Returns the substring between ':' and '='
        constexpr auto getVarNameAndType(std::string_view line, std::size_t assignment) const -> std::pair<std::string_view, std::string_view>;
        // Separates the "container" and the "type" elements
        auto getContainerAndType(std::string_view str) -> std::pair<std::string, e_cppTypes>;
        // Helper to add a new single line container
        void makeSingleLineContainer(std::string_view line, std::size_t assignment, std::size_t openCurlyBracket, std::size_t closeCurlyBracket, Group& group);


    private:

        // Private struct
        struct Multiline
        {
            bool isActive;
            std::size_t startLine;

            std::string buffer;
            std::string_view varName;
            std::string_view varType;
        };


    private:

        // <Group name, data>
        using t_groupNameToVars = std::unordered_map<std::string_view, Group>;

        t_groupNameToVars m_groups;

        // Quotation config
        PeponQuotes m_quotes;

};

} // pepon


#endif // PEPON_FILE_HPP
