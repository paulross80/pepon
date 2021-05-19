# PEPON

C++ Object Notation file format/reading/writing library for modern C++(17+)

Pepon lets you save and load your **C++ data** easily

```
#include "peponLib/PeponFile.hpp"

std::vector<int> numbers { 1, 2, 3, 4, 5, 6 };
std::deque<unsigned int> units { 10, 20, 30, 40, 50, 60 };
std::list<float> ff { 1.1f, 2.2f, 3.3f };

// All this data can be saved and loaded from Pepon files
pepon::PeponFile pf;
pf.saveFile("./pepon_out.txt");
```

## Configuration:

* Set **PEPON_USE_STL_PATH** variable (in the CMakeLists.txt file) to value 1 to specify file paths using __std::filesystem::path__. Otherwise, you must specify file path using __std::string_view__

* struct **PeponQuotes**: You can enable/disable quotation marks for every value shown by the **print()** functions. And you can set the quotation character you want to use

## File format:

* Comments are optional. Each line starting with '//' (C++), '--' (Lua) or a single '#' (Python style) is detected as a comment and saved, to be attached later to a Group

* Groups. Before declaring variables, you need to create at least one group. Think of it as the sections found in .INI files. Group declaration is written using square brackets. Example:

```
// This is my very first group!
[MyFirstGroup]
```

* Variables: They can be **single values**, for example, just an int or a double. Or they can be **containers**, like a vector of floats. The general format for writing a variable is this:

```
a : int = 1
```
This specifies an __int__ value, named __'a'__, with a value of __1__. 
Variable name, type and value **MUST** be set, always

String values must be written using opening and closing double quotation marks ('"')

```
b : string = "Hello, this is my first string"
```

Containers template type must be always set. Container values must be specified using curly brackets ('{' and '}' characters)

```
c : list<bool> = { true, false, NOT_TRUE, 1, I_JUST_LIED }
```

You can write container values in multiple lines, but the opening '{' must be specified in the same line where the container is declared

```
c : list<bool> = {
    true,
    false,
    0
}
```

You can also write long strings in multiple lines, but the opening '"' must be written in the same line where the string is declared. Please note that the endline characters will be part of the string

```
text : string = "First line ............
                 Second line ...........
                 Third line ............"
```

**Note**: You can't write multiline strings if they are the values of a container. Sorry

## Use examples:

```
// Parse Pepon data from memory
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
```

Open **tests/pepon_test.cpp** for more examples

## Future:

* Add support for more containers? (set, unordered_set, map, unordered_map)
