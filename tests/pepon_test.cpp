#include <iostream>
#include <vector>
#include <string>

// Pepon
#include "PeponConfig.hpp"

#include <peponLib/Pepon.hpp>
#include <peponLib/PeponFile.hpp>

#include <peponLib/Variables/SingleValue.hpp>
#include <peponLib/Variables/ListValue.hpp>


// Shortcuts
using std::cout;
using std::endl;


// Main function
int main([[maybe_unused]] int argc, [[maybe_unused]] const char* argv[])
{

    // Display some information
    cout << PEPON_LIBRARY_NAME << " version " << PEPON_VERSION_FULL_STRING << endl;
    cout << PEPON_DESCRIPTION << endl;


    // Quotation config
    pepon::PeponQuotes quotes;
    quotes.useQuotes  = true;
    quotes.quotesChar = '\'';

    // Create variables
    pepon::SingleValue<int> myValue("myValue", 100);
    myValue.print(quotes);

    pepon::ListValue<float> floating("floating", std::list<float> { 2.1f, 45.6f, 34.7f });
    floating.print(quotes);


    // Parse Pepon from memory
    std::vector<std::string> lines =
    {
        "// Hello, this is a nice C++ style comment",
        "# And this is a nice Python style comment",
        "[My first group]",
        "a : int = 1",
        "b: string=\"Hello world!\"",
        "   EVIL :   vEctOr  <   inT         >   =   {      1,      2,3}",
        "",
        "d:deque<uint> = { 10, 20,",
        "                  30, 40,",
        "                  50, 60 }",
        "",
        " e : string = \"This is a very,",
        "              very long",
        "              string",
        "\"",
        ""
    };

    pepon::PeponFile pf;
    pf.loadFromMemory(lines);

    // Show parsed data to console
    pf.print();

    // Disable quotation marks
    quotes.useQuotes = false;
    pf.useQuotation(quotes);

    // Save Pepon data to file
    pf.saveFile("./pepon_out.txt");

    return 0;
}
