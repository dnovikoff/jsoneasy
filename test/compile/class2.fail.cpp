#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <jsoneasy/template/create.hpp>
#include <jsoneasy/template/support/list.hpp>

struct UnknownType {};

struct Example {
	int first;
	UnknownType second;
	std::string other;
};

JE_CLASS_NS( Example , JE_FIELD(first)JE_FIELD(second)JE_FIELD(other) )

void compile() {
	std::list<Example> x;
	JsonEasy::Template::createHandler( x );
}

