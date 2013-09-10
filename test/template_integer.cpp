#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>

#include <boost/test/unit_test.hpp>

#include <jsoneasy/template/convert.hpp>

BOOST_AUTO_TEST_CASE ( typeTest ) {
	using JsonEasy::Parser::Integer;
	int x = 0;
	Integer i(64);
	bool r = JsonEasy::Template::jsonToUser(i, x);
	BOOST_REQUIRE( r );
	BOOST_CHECK_EQUAL( x, 64 );

	const Integer i2(i);
	r = JsonEasy::Template::jsonToUser(i2, x);
	BOOST_REQUIRE( r );
	BOOST_CHECK_EQUAL( x, 64 );
}
