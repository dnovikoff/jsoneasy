#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>

#include <boost/test/unit_test.hpp>

#include <jsoneasy/parser/string_parser.hpp>
#include <jsoneasy/template/create.hpp>
#include <jsoneasy/template/class.hpp>

#include "one_array.hpp"

struct Example {
	int first;
	int second;
	std::string other;
};

JE_CLASS_NS( Example , JE_FIELD(first)JE_FIELD(second) )

BOOST_AUTO_TEST_CASE ( simple ) {

}
