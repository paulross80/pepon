#include <fstream>

#include "PeponFile.hpp"
#include "PeponConfig.hpp"

#include <extLibs/Utils/MapFinder.hpp>
#include <extLibs/Utils/StringUtils.hpp>

namespace pepon
{

// Load and parse an entire Pepon file from disk
void PeponFile::loadFile(t_filePath fileName)
{

    // Open file
    std::ifstream handle;
    handle.open(fileName);

    if (!handle)
        throw SyntaxError("File cannot be opened!");

    // Buffers
    std::vector<std::string> entireFile;
    std::string line;

    // Read line by line
    while(std::getline(handle, line))
        entireFile.push_back(line);

    // Close it
    handle.close();


    // Load from memory
    //this->loadFromString(data.str());
    this->loadFromMemory(entireFile);

}

// Save Pepon data to a file on disk
void PeponFile::saveFile(t_filePath fileName)
{

    // Nothing to write?
    if (m_groups.empty())
        return;


    // Get a file handle
    std::ofstream handle;
    handle.open(fileName, std::ios_base::trunc);

    this->print(handle);

    handle.close();

}


// Get group by reference/const reference
auto PeponFile::getGroup(std::string_view name) -> Group&
{
    auto itr = myoga::mapFinder(m_groups, name,
                                std::string(PEPON_LIBRARY_NAME) + ": Group [" + std::string(name) + "] not found!");

    return const_cast<Group&>(itr->second);
}

auto PeponFile::getGroup(std::string_view name) const -> const Group&
{
    const auto cItr = myoga::mapFinder(m_groups, name,
                                       std::string(PEPON_LIBRARY_NAME) + ": Group [" + std::string(name) + "] not found!");

    return cItr->second;
}


// Setters:
// Specify quotations use and style
void PeponFile::useQuotation(PeponQuotes quotes)
{
    m_quotes = std::move(quotes);
}


// Shows every group and its content
void PeponFile::print(std::ostream& out) const
{

    // Nothing to write?
    if (m_groups.empty())
    {
        out << "PeponFile is empty!" << std::endl;
        return;
    }

    std::size_t written = 0U;

    // Iterate through every group
    for (auto& [_, group] : m_groups)
    {
        group.print(m_quotes, out);

        if (written < m_groups.size() - 1)
        {
            out << std::endl;
            ++written;
        }

    }

}



// Private functions
// Separates the "container" and the "type" elements
// Ex: input = "vector<int>", output = { "vector", e_cppTypes::INT }
auto PeponFile::getContainerAndType(std::string_view str) -> std::pair<std::string, e_cppTypes>
{

    auto lessThan    = str.find('<');
    auto greaterThan = str.find('>');

    if (lessThan == std::string_view::npos || greaterThan == std::string_view::npos)
        throw SyntaxError("Complete container<type> for containers must be specified\nEx: elements : vector<int> = { -1, -2, -3 }");

    // Get container and its inner template typename
    auto container = myoga::lowercase(myoga::trimStr(str.substr(0, lessThan)));
    auto innerType = myoga::lowercase(myoga::trimStr(str.substr(lessThan + 1, greaterThan - lessThan - 1)));

    // Type to enum
    auto typeAsEnum = stringTypeToEnum(innerType);

    if (typeAsEnum == e_cppTypes::UNKNOWN)
        throw SyntaxError("Unknown template data type for \"" + container + "<" + innerType + ">\"");

    return { container, typeAsEnum };

}


// Gets variable name and type
// Returns the substring between ':' and '='
// Ex: input = "elements : vector<int> = { -1, -2, -3 }", output = { "elements", "vector<int>" }"
constexpr auto PeponFile::getVarNameAndType(std::string_view line, std::size_t assignment) const -> std::pair<std::string_view, std::string_view>
{

    auto colon = line.find_first_of(':');

    if (colon == std::string_view::npos || assignment < colon)
        throw SyntaxError("Colon character \':\' not found, or equal sign \'=\' is in the wrong position\n a : int = 10");

    auto name = myoga::trimStr(line.substr(0, colon));
    auto type = myoga::trimStr(line.substr(colon + 1, assignment - colon - 1));

    return std::make_pair(name, type);

}


// Helper to add a new single line container
void PeponFile::makeSingleLineContainer(std::string_view line, std::size_t assignment, std::size_t openCurlyBracket, std::size_t closeCurlyBracket, Group& group)
{

    // Get Container<Type>
    auto nameAndType = this->getVarNameAndType(line, assignment);

    // Get "container", enum type
    auto contAndType = this->getContainerAndType(nameAndType.second);

    // Remove curly braces from the values
    auto cleanValues = line.substr(openCurlyBracket + 1, closeCurlyBracket - openCurlyBracket - 1);

//    cout << " --->>> SL Container: \"" << nameAndType.first << "\" : \"" << contAndType.first << ", values: " << cleanValues << endl;

    // Make it!
    group.addContainer(nameAndType.first, contAndType.first, contAndType.second, cleanValues);

}



// Load and parse Pepon data from strings
void PeponFile::loadFromMemory(const std::vector<std::string>& lines)
{

    // Token not found constant
    constexpr auto STR_TOKEN_NOT_FOUND = std::string_view::npos;

    // Current line number being processed
    std::size_t lineNumber = 0U;

    // Are we looking at a value that
    // is splitted into multiple lines?
    Multiline multi;
    multi.isActive = false;

    // Comments
    Group::t_commentsContainer comments;


    // Initialize iterator to invalid state
    t_groupNameToVars::iterator currGroup = m_groups.end();


    // Process the entire text
    while(lineNumber < lines.size())
    {

        // Implicit convertion operator
        // https://en.cppreference.com/w/cpp/string/basic_string/operator_basic_string_view
        std::string_view line = lines[lineNumber];
        line = myoga::trimStr(line);


        // Find comments
        if (line.size() > 1)
        {

            // C & C++ (//), Lua (--) style comments
            if ((line[0] == '/' && line[1] == '/') || (line[0] == '-' && line[1] == '-'))
            {
                // Remove those two characters
                line.remove_prefix(2);

                // Save the line
                comments.emplace_back(myoga::trimStr(line));
//                cout << '[' << lineNumber << "]: Comment" << endl;

                // Continue iteration
                ++lineNumber;
                continue;
            }
            else
            // Python style comments are supported too
            if (line[0] == '#')
            {
                // Remove that character
                line.remove_prefix(1);

                // Save the line
                comments.emplace_back(myoga::trimStr(line));
//                cout << '[' << lineNumber << "]: Comment" << endl;

                // Continue iteration
                ++lineNumber;
                continue;
            }

        }



        // Find [Group]
        auto openBracket = line.find('[');

        // Check for group name
        if (openBracket != STR_TOKEN_NOT_FOUND)
        {

            // Find close bracket
            auto closeBracket = line.find(']', ++openBracket);

            if (closeBracket == STR_TOKEN_NOT_FOUND)
                throw SyntaxError("Syntax error: expected \']\' after group name\nin line: " + std::to_string(lineNumber));

            // Remove brackets and empty characters
            auto groupName = line.substr(openBracket, closeBracket - openBracket);
            groupName = myoga::trimStr(groupName);


            // Create the new group (with an empty container)
//            cout << '[' << lineNumber << "]: Group [" << groupName << ']' << endl;

            // Try to create the new group in place
            auto [itr, emplaced] = m_groups.try_emplace(groupName, Group(groupName));

            if (!emplaced)
                throw SyntaxError("New group was not emplaced correctly");

            // Update iterator
            currGroup = itr;

            // Save comments, if not empty
            currGroup->second.takeOwnershipOfComments(comments);

            // Delete them
            comments.clear();


            // Continue iteration
            ++lineNumber;
            continue;

        }



        // Check for variable assignment
        auto assignment = line.find('=');

        if (assignment != STR_TOKEN_NOT_FOUND)
        {

            // In order to make a new variable,
            // a group is absolutely needed
            if (currGroup == m_groups.end())
                throw SyntaxError("You cannot create variables without defining a [Group] first");


            // Find curly brackets
            auto openCurlyBracket  = line.find_first_of('{', assignment + 1);
            auto closeCurlyBracket = line.find_last_of ('}');

            bool hasOpenCurlyBracket   = (openCurlyBracket  != STR_TOKEN_NOT_FOUND);
            bool hasClosedCurlyBracket = (closeCurlyBracket != STR_TOKEN_NOT_FOUND);

            /// TODO: check if has open && closed position > open

            // Both curly brackets?
            // This is a single line container
            if (hasOpenCurlyBracket && hasClosedCurlyBracket)
            {

//                cout << '[' << lineNumber << "]: SingleLine container: " << line << endl;
                this->makeSingleLineContainer(line, assignment, openCurlyBracket, closeCurlyBracket, currGroup->second);

            }
            else
            // Only the opening bracket?
            // this is a multiline container
            if (hasOpenCurlyBracket && !hasClosedCurlyBracket)
            {

                auto nameAndType = this->getVarNameAndType(line, assignment);

                // Activate multiline mode
                // starting from this line
                multi.isActive  = true;
                multi.startLine = lineNumber;

                // Save var name and type for later
                multi.varName = nameAndType.first;
                multi.varType = nameAndType.second;

                // Is there any value beyond the bracket? Take it!
                auto something = myoga::trimStr(line.substr(openCurlyBracket + 1));

                // Only add non empty strings to the multiline buffer
                if (!myoga::isEmptyStr(something))
                {
                    multi.buffer += something;
                    multi.buffer += '\n';
                }

//                cout << '[' << lineNumber << "]: MultiLine container START! -> name: \"" << multi.varName << "\", type: \"" << multi.varType << "\"" << endl;

            }
            else
            // Only the closing bracket after assignment?
            // Not good!
            if (!hasOpenCurlyBracket && hasClosedCurlyBracket)
            {

                throw SyntaxError("Syntax error at line " + std::to_string(lineNumber) + "\nAssignment. No open curly bracket, but closing. What?");

            }
            else

            // No curly brackets at all?
            {

                // Search for double quotation marks
                auto openQuotetionMark  = line.find_first_of('"', assignment + 1);
                auto closeQuotetionMark = line.find_last_of('"');

                bool hasOpeningQuotation = (openQuotetionMark   != STR_TOKEN_NOT_FOUND);
                bool hasClosingQuotation = ((closeQuotetionMark != STR_TOKEN_NOT_FOUND) &&
                                            (closeQuotetionMark > openQuotetionMark));


                // Both quotation marks?
                // Then this is a single line string
                if (hasOpeningQuotation && hasClosingQuotation)
                {

//                    cout << '[' << lineNumber << "]: SingleLine string -> " << line << endl;

                    // Get var name and type
                    auto nameAndType = this->getVarNameAndType(line, assignment);
                    auto typeAsEnum = stringTypeToEnum(nameAndType.second);

                    // Type MUST be string!
                    if (typeAsEnum != e_cppTypes::STRING_UTF8)
                        throw SyntaxError("Opening and closing quotation marks should be used only for strings!\nCheck line " + std::to_string(lineNumber));

                    // Get clean value (without quotation marks)
                    auto value = line.substr(openQuotetionMark + 1, closeQuotetionMark - openQuotetionMark - 1);

                    // Make it!
                    currGroup->second.addVariable(nameAndType.first, value, e_cppTypes::STRING_UTF8);

//                    cout << " --->>> SL String: \"" << nameAndType.first << "\" : \"string\" = \"" << value << "\"" << endl;

                }
                else
                // Only the opening quotation mark?
                // This is a multiline string
                if (hasOpeningQuotation && !hasClosingQuotation)
                {

                    // Get variable name and type
                    auto nameAndType = this->getVarNameAndType(line, assignment);
                    auto typeAsEnum = stringTypeToEnum(nameAndType.second);

                    // Type MUST be string!
                    if (typeAsEnum != e_cppTypes::STRING_UTF8)
                        throw SyntaxError("Opening quotation marks should be used only for strings!\nCheck line " + std::to_string(lineNumber));

                    // Activate multiline mode
                    // starting from this line
                    multi.isActive  = true;
                    multi.startLine = lineNumber;

                    // Save var name and type for later
                    multi.varName = nameAndType.first;
//                    multi.varType = nameAndType.second;

                    // Is there any value beyond the quotation mark? Take it!
                    auto something = line.substr(openQuotetionMark + 1);

                    // Empty lines are ok for strings
                    multi.buffer += something;
                    multi.buffer += '\n';

//                    cout << '[' << lineNumber << "]: MultiLine string START! -> " << line << endl;

                }
                else
                // No quotation marks at all?
                // Then this is a single line variable
                // (no container, no string)
                {

                    auto nameAndType = this->getVarNameAndType(line, assignment);
                    auto typeAsEnum = stringTypeToEnum(nameAndType.second);

                    auto value = myoga::trimStr(line.substr(assignment + 1));

                    // Create the new variable
                    currGroup->second.addVariable(nameAndType.first, value, typeAsEnum);

//                    cout << '[' << lineNumber << "]: SingleLine variable -> name: \"" << nameAndType.first << "\", type: \"" << nameAndType.second << "\", value: \"" << value << "\"" << endl;

                }


            } // if (brackets)

        } // if (assignment)


        else
        // No assignment?
        // Check for closing multiline variables
        {

            auto closeCurlyBracket  = line.find_last_of('}');
            auto closeQuotetionMark = line.find_last_of('"');

            // Closing a multiline container
            if (closeCurlyBracket != STR_TOKEN_NOT_FOUND)
            {

                // Disable multiline mode
                multi.isActive = false;

                // Is there any value before the closing bracket? Take it!
                auto something = myoga::trimStr(line.substr(0, closeCurlyBracket));

                if (!myoga::isEmptyStr(something))
                    multi.buffer += something;


                // Get container and type
                auto contAndType = this->getContainerAndType(multi.varType);


//                    throw SyntaxError("Syntax error on line " + std::to_string(multi.startLine) +
//                                      "\nContainer variables must include its <type>" +
//                                      "\nEx: vector<int>");



//                cout << '[' << lineNumber << "]: MultiLine container ENDS! -> name: \"" << multi.varName << "\", container: \"" << contAndType.first << "\", started at line " << multi.startLine << endl;
//                cout << '\'' << multi.buffer << '\'' << endl;

                currGroup->second.addContainer(multi.varName, contAndType.first, contAndType.second, multi.buffer);

                // Clear the multiline buffer
                multi.buffer.clear();

            }
            else
            // Closing a multiline string
            if (closeQuotetionMark != STR_TOKEN_NOT_FOUND)
            {

                // Disable multiline mode
                multi.isActive = false;

                // Is there any value before the closing quotation mark? Take it!
                auto something = line.substr(0, closeQuotetionMark);
                multi.buffer += something;

//                cout << '[' << lineNumber << "]: MultiLine string ENDS! -> name: \"" << multi.varName << "\", started at line " << multi.startLine << endl;

                currGroup->second.addVariable(multi.varName, multi.buffer, e_cppTypes::STRING_UTF8);

                // Clear the multiline buffer
                multi.buffer.clear();

            }
            else

            // No multiline string/container closing
            // Add this line to the multiline buffer
            // (only if multiline mode is activated)
            {

                if (multi.isActive)
                {
                    multi.buffer += line;
                    multi.buffer += '\n';

//                    cout << "\tadding line to buffer... " << line << endl;
                }

            }

        }  // no assignment



        // Advance to next line
        ++lineNumber;

    } // while


//    cout << endl;

}

} // pepon
