#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <jsoneasy/template/create.hpp>
#include <jsoneasy/template/class_container.hpp>
#include <jsoneasy/template/support/list.hpp>

struct Example {
	int first;
	int second;
	std::string other;
};

JE_CLASS_NS( Example , JE_FIELD(first)JE_FIELD(second)JE_FIELD(other) )

void compile() {
	Example x;
	JsonEasy::Template::createHandler( x );
}

void compile1() {
	std::list<Example> x;
	JsonEasy::Template::createHandler( x );
}
