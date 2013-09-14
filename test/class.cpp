#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>

#include <boost/test/unit_test.hpp>

#include <jsoneasy/parser/string_parser.hpp>
#include <jsoneasy/template/create.hpp>
#include <jsoneasy/template/class_container.hpp>

#include "one_array.hpp"
#include "parse_string.hpp"

struct Example {
	int first;
	int second;
	std::string other;
};

JE_CLASS_NS( Example , JE_FIELD(first)JE_FIELD(second)JE_FIELD(other) )

BOOST_AUTO_TEST_CASE ( simpleTest ) {
	Example e;
	BOOST_CHECK( !parseString("{}", e) );

	BOOST_REQUIRE( parseString( R"({"first":345, "second" : 999, "other": "Dmitri Novikov"})", e) );
	BOOST_CHECK_EQUAL( e.first, 345 );
	BOOST_CHECK_EQUAL( e.second, 999 );
	BOOST_CHECK_EQUAL( e.other, "Dmitri Novikov" );

	// one more field
	BOOST_REQUIRE( !parseString( R"({"first":345, "second" : 999, "other": "Dmitri Novikov", "onmorefield": 0})", e) );

	// wrong type
	BOOST_REQUIRE( !parseString( R"({"first": "d", "second" : 999, "other": "Dmitri Novikov"})", e) );

	// one field less
	BOOST_REQUIRE( !parseString( R"({"first":345, "second" : 999})", e) );
}

BOOST_AUTO_TEST_CASE ( complexTest ) {
	OneArray<Example> e;
	BOOST_CHECK( !parseString("[{}]", e) );

	BOOST_REQUIRE( parseString( R"([{"first":345, "second" : 999, "other": "Dmitri Novikov"}])", e) );
	BOOST_CHECK_EQUAL( e->first, 345 );
	BOOST_CHECK_EQUAL( e->second, 999 );
	BOOST_CHECK_EQUAL( e->other, "Dmitri Novikov" );
}

// Subtype
struct Example1 {
	std::string name;
	Example data;
};

JE_CLASS_NS( Example1 , JE_FIELD(name)JE_FIELD(data) )

BOOST_AUTO_TEST_CASE ( subClassTest ) {
	Example1 e;
	BOOST_REQUIRE( parseString( R"({"name":"MyObject", "data":{"first":345, "second" : 999, "other": "Dmitri Novikov"}})", e) );
	BOOST_CHECK_EQUAL( e.name, "MyObject" );
	BOOST_CHECK_EQUAL( e.data.first, 345 );
	BOOST_CHECK_EQUAL( e.data.second, 999 );
	BOOST_CHECK_EQUAL( e.data.other, "Dmitri Novikov" );
}

