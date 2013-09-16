#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>
#include <sstream>

#include <boost/test/unit_test.hpp>

#include <jsoneasy/parser/stream_parser.hpp>
#include <jsoneasy/template/create.hpp>

#include "one_array.hpp"

template<typename T>
static bool parseStream(std::istream& input, T& data) {
	JsonEasy::Parser::Handler::Ptr h = JsonEasy::Template::createHandler(data);
	JsonEasy::Parser::StreamParser sp;
	if( !sp.parse(input, h) ) return false;
	return true;
}

BOOST_AUTO_TEST_CASE ( simpleTest ) {
	OneArray<int> v;
	v.value = 0;
	std::stringstream ss;
	ss << "[528]";
	BOOST_REQUIRE( parseStream(ss, v) );
	BOOST_CHECK_EQUAL( v.value, 528 );
	ss << "error";
	BOOST_REQUIRE( !parseStream(ss, v) );
}


