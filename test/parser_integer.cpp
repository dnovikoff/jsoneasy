#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/typeof/typeof.hpp>

#include <jsoneasy/parser/integer.hpp>

using JsonEasy::Parser::Integer;

BOOST_AUTO_TEST_CASE ( uint8Test ) {
	Integer i( 255 ) ;
	uint8_t v;
	BOOST_REQUIRE( i.to(v) );
	BOOST_CHECK_EQUAL( v, 255 );

	// Too much
	i = Integer(256);
	BOOST_REQUIRE( !i.to(v) );

	i = Integer(0);
	BOOST_REQUIRE( i.to(v) );
	BOOST_CHECK_EQUAL( v, 0 );

	// negative
	i = Integer(-1);
	BOOST_REQUIRE( !i.to(v) );
}

BOOST_AUTO_TEST_CASE ( int8Test ) {
	Integer i( 255 ) ;
	int8_t v;
	// Too much
	BOOST_REQUIRE( !i.to(v) );
	// Still too much
	i = Integer(128);
	BOOST_REQUIRE( !i.to(v) );
	// Not too much
	i = Integer(127);
	BOOST_REQUIRE( i.to(v) );
	BOOST_CHECK_EQUAL( v, 127 );

	// negative ok
	i = Integer(-1);
	BOOST_REQUIRE( i.to(v) );
	BOOST_CHECK_EQUAL( v, -1 );

	i = Integer(-128);
	BOOST_REQUIRE( i.to(v) );
	BOOST_CHECK_EQUAL( v, -128 );

	i = Integer(-129);
	BOOST_REQUIRE( !i.to(v) );
}

BOOST_AUTO_TEST_CASE ( sizetTest ) {
	Integer i( 32768 ) ;
	size_t v;
	BOOST_REQUIRE( i.to(v) );
	BOOST_CHECK_EQUAL( v, 32768 );

	// negative
	i = Integer(-1);
	BOOST_REQUIRE( !i.to(v) );
}

BOOST_AUTO_TEST_CASE ( doubleTest ) {
	Integer i( 32768 ) ;
	double v;
	BOOST_REQUIRE( i.to(v) );
	BOOST_CHECK_EQUAL( v, 32768 );

	// negative
	i = Integer(-1);
	BOOST_REQUIRE( i.to(v) );
	BOOST_CHECK_EQUAL( v, -1.0 );

	i = Integer(-128);
	BOOST_REQUIRE( i.to(v) );
	BOOST_CHECK_EQUAL( v, -128.0 );
}

BOOST_AUTO_TEST_CASE ( int64Test ) {
	Integer i( 9223372036854775807 ) ;
	int64_t v;
	BOOST_REQUIRE( i.to(v) );
	BOOST_CHECK_EQUAL( v, 9223372036854775807 );

	i = Integer( std::numeric_limits<int64_t>::min() );
	BOOST_REQUIRE( i.to(v) );
	BOOST_CHECK_EQUAL( v, std::numeric_limits<int64_t>::min() );
}

BOOST_AUTO_TEST_CASE ( uint64Test ) {
	Integer i( 9223372036854775807 ) ;
	uint64_t v;
	BOOST_REQUIRE( i.to(v) );
	BOOST_CHECK_EQUAL( v, 9223372036854775807 );

	i = Integer( std::numeric_limits<int64_t>::min() );
	BOOST_REQUIRE( !i.to(v) );
}
