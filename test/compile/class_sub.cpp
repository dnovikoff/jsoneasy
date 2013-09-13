#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <jsoneasy/template/class.hpp>

struct Example {
	int first;
	int second;
	std::string other;
};

struct Example1 {
	std::string name;
	Example data;
};

JE_CLASS_NS( Example, JE_FIELD(first)JE_FIELD(second)JE_FIELD(other) )
JE_CLASS_NS( Example1, JE_FIELD(name)JE_FIELD(data) )

