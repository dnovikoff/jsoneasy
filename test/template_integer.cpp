#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>

#include <boost/test/unit_test.hpp>

#include <jsoneasy/template/convert.hpp>
#include <jsoneasy/parser/string_parser.hpp>
#include <jsoneasy/template/create.hpp>
#include <test/one_array.hpp>

BOOST_AUTO_TEST_CASE ( typeTest ) {
	using JsonEasy::Parser::Integer;
	int x = 0;
	Integer i(64);
	bool r = JsonEasy::Template::jsonToUser(i, x);
	BOOST_REQUIRE( r );
	BOOST_CHECK_EQUAL( x, 64 );
}

template<typename T>
static bool parseTo(const std::string& input, T& data) {
	JsonEasy::Parser::Handler::Ptr h = JsonEasy::Template::createHandler(data);
	JsonEasy::Parser::StringParser sp;
	if( !sp.parse(input, h) ) return false;
	return true;
}

BOOST_AUTO_TEST_CASE ( int64Test ) {
	OneArray<int64_t> o;

	// Max
	BOOST_REQUIRE( parseTo("[9223372036854775807]", o) );
	BOOST_CHECK_EQUAL( 9223372036854775807, *o );

	// Too big
	BOOST_REQUIRE( !parseTo("[9223372036854775808]", o) );

	// Min
	BOOST_REQUIRE( parseTo("[-9223372036854775808]", o) );
	BOOST_CHECK_EQUAL( std::numeric_limits<int64_t>::min(), *o );

	// Too low
	BOOST_REQUIRE( !parseTo("[-9223372036854775809]", o) );
}

BOOST_AUTO_TEST_CASE ( uint64Test ) {
	OneArray<uint64_t> o;

	// Still max value for uint64_t, since it is stored in int64
	BOOST_REQUIRE( parseTo("[9223372036854775807]", o) );
	BOOST_CHECK_EQUAL( 9223372036854775807, *o );

	// Too big
	BOOST_REQUIRE( !parseTo("[9223372036854775808]", o) );

	// Min
	BOOST_REQUIRE( parseTo("[0]", o) );
	BOOST_CHECK_EQUAL( std::numeric_limits<uint64_t>::min(), *o );

	// Too low
	BOOST_REQUIRE( !parseTo("[-9223372036854775809]", o) );
	BOOST_REQUIRE( !parseTo("[-9223372036854775808]", o) );
	BOOST_REQUIRE( !parseTo("[-1]", o) );
}
