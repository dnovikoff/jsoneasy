JsonEasy::Template
===========

This is Template engine. The purpose is to create parser for structures of custom difficulties
No variant-like tree's are created when parsing

**Example of use:**

typedef std::list<int> list_t;
typedef std::map<std::string, list_t> map_t;
typedef std::vector<map_t> vec_t;

vec_t x;
JsonEasy::Parser::StringParser sp;
sp.parse("[{\"hello\":[4,2,3]}]", JsonEasy::Template::createHandler(x));


You can hide combination of parser+handler creation to your function with two params signature like

template<typename T>
static bool parseTo(const std::string& input, T& data) {
	JsonEasy::Parser::StringParser sp;
	return sp.parse(input, JsonEasy::Template::createHandler(data));
}

The reason there is no such function is that you have an ability to choose not a StringParser (ex. based on stream one)


container.hpp
--------------
Special class to be specialized by user to support his container types. See "support" folder as an example

type.hpp
--------------
Special class to be specialized by user to support his simple (not container) types. See "support" folder as an example

What do you need to include?
--------------
include <jsoneasy/template.hpp> to include Template with support of all containers

**or** you can include support for only types you need. Like:

include <jsonyeasy/template/create.hpp>
include <jsonyeasy/template/support/vector.hpp>
include <jsonyeasy/template/support/optional.hpp>
