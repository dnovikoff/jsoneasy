#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>

#include <boost/test/unit_test.hpp>

#include <jsoneasy/parser/string_parser.hpp>
#include <jsoneasy/template/create.hpp>
#include <jsoneasy/template/class.hpp>
#include <jsoneasy/template/class_container.hpp>

#include "one_array.hpp"
#include "parse_string.hpp"

struct Example {
	int first;
	int second;
	std::string other;
};

JE_CLASS_NS( Example , JE_FIELD(first)JE_FIELD(second)JE_FIELD(other) )

BOOST_AUTO_TEST_CASE ( simple ) {
	auto meta = JsonEasy::Template::ClassContainer< Example >::names;
	for( auto p : meta ) {
		std::cout << p.first << " = " << p.second << std::endl;
	}
	BOOST_CHECK( false );
}
