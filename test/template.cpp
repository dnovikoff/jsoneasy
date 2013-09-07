#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>

#include <boost/variant/get.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/typeof/typeof.hpp>

#include <jsoneasy/parser/string_parser.hpp>
#include <jsoneasy/template.hpp>

#include <boost/assign.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/range.hpp>

using namespace boost::assign;

template<typename R>
static void dumpRange(const R& x1, const std::string& label) {
	BOOST_AUTO(x1b, boost::begin(x1));
	BOOST_AUTO(x1e, boost::end(x1));

	std::cout << "Dumping range " << label << std::endl;
	std::cout << "Size = " << std::distance(x1b, x1e) << std::endl;
	for(BOOST_AUTO(i, boost::begin(x1)); i != boost::end(x1); ++i) {
		std::cout << *i << std::endl;
	}
	std::cout << std::endl;
}

template<typename R1, typename R2>
static bool cequals(const R1& x1, const R2& x2) {
	BOOST_AUTO(x1b, boost::begin(x1));
	BOOST_AUTO(x1e, boost::end(x1));

	BOOST_AUTO(x2b, boost::begin(x2));

	BOOST_AUTO(d1, boost::distance(x1));
	BOOST_AUTO(d2, boost::distance(x2));

	if( d1 != d2 || !std::equal(x1b, x1e, x2b) ) {
		dumpRange(x1, "left");
		dumpRange(x2, "right");
		return false;
	}
	return true;
}

template<typename T>
static bool parseTo(const std::string& input, T& data) {
	JsonEasy::Parser::Handler::Ptr h = JsonEasy::Template::createHandler(data);
	JsonEasy::Parser::StringParser sp;
	if( !sp.parse(input, h) ) return false;
	return true;
}

BOOST_AUTO_TEST_CASE ( vectorTest ) {
	std::vector<int> x;
	BOOST_CHECK( parseTo("[]", x) );
	BOOST_CHECK( x.empty() );

	BOOST_CHECK( parseTo("[721]", x) );
	BOOST_REQUIRE_EQUAL( x.size(), 1u );
	BOOST_CHECK_EQUAL(x.back(), 721);

	// old data should be removed
	BOOST_CHECK( parseTo("[123]", x) );
	BOOST_REQUIRE_EQUAL( x.size(), 1u );
	BOOST_CHECK_EQUAL(x.back(), 123);

	BOOST_CHECK( parseTo("[123, 321, 0, 4, 12]", x) );
	{
		std::vector<int> ethalon;
		ethalon += 123,321,0,4,12;
		BOOST_CHECK( cequals(x, ethalon) );
	}
}

BOOST_AUTO_TEST_CASE ( listTest ) {
	std::list<int> x;
	BOOST_CHECK( parseTo("[]", x) );
	BOOST_CHECK( x.empty() );

	BOOST_CHECK( parseTo("[721]", x) );
	BOOST_REQUIRE_EQUAL( x.size(), 1u );
	BOOST_CHECK_EQUAL(x.back(), 721);

	// old data should be removed
	BOOST_CHECK( parseTo("[123]", x) );
	BOOST_REQUIRE_EQUAL( x.size(), 1u );
	BOOST_CHECK_EQUAL(x.back(), 123);

	parseTo("[123, 321, 0, 4, 12]", x);
	{
		std::vector<int> ethalon;
		ethalon += 123,321,0,4,12;
		BOOST_CHECK( cequals(x, ethalon) );
	}
}

BOOST_AUTO_TEST_CASE ( setTest ) {
	std::set<int> x;
	BOOST_CHECK( parseTo("[]", x) );
	BOOST_CHECK( x.empty() );

	BOOST_CHECK( parseTo("[721]", x) );
	BOOST_REQUIRE_EQUAL( x.size(), 1u );
	BOOST_CHECK_EQUAL(*x.begin(), 721);

	// old data should be removed
	BOOST_CHECK( parseTo("[123]", x) );
	BOOST_REQUIRE_EQUAL( x.size(), 1u );
	BOOST_CHECK_EQUAL(*x.begin(), 123);

	parseTo("[123, 321, 0, 4, 12]", x);
	{
		std::vector<int> ethalon;
		ethalon += 0,4,12,123,321;
		BOOST_CHECK( cequals(x, ethalon) );
	}

	// multiple same values
	BOOST_CHECK( !parseTo("[1,4,1]", x) );

	//just compilation test
	std::set<int, std::greater<int> > x2;
	BOOST_CHECK( parseTo("[]", x2) );
}

BOOST_AUTO_TEST_CASE ( multisetTest ) {
	{
		std::multiset<int> x;
		BOOST_CHECK( parseTo("[123, 321, 0, 4, 12, 0, 0, 123]", x) );

		std::vector<int> ethalon;
		ethalon += 0,0,0,4,12,123,123,321;
		BOOST_CHECK( cequals(x, ethalon) );
	}
	// compilation tests
	{
		std::multiset<int, std::greater<int> > x;
		BOOST_CHECK( parseTo("[123, 321, 0, 4, 12, 0, 0, 123]", x) );
	}
}

BOOST_AUTO_TEST_CASE ( stackTest ) {
	std::stack<int> x;
	BOOST_CHECK( parseTo("[123, 321, 0, 4, 12, 0, 0, 123]", x) );
	BOOST_REQUIRE_EQUAL(x.size(), 8u);
	BOOST_REQUIRE_EQUAL(x.top(), 123);
}

BOOST_AUTO_TEST_CASE ( dequeTest ) {
	std::deque<int> x;
	BOOST_CHECK( parseTo("[123, 321, 0, 4, 12, 0, 0, 123]", x) );
	BOOST_REQUIRE_EQUAL(x.size(), 8u);
	BOOST_REQUIRE_EQUAL(x.back(), 123);
}

BOOST_AUTO_TEST_CASE ( mapTest ) {
	std::map<std::string, int> x;
	BOOST_CHECK( parseTo("{}", x) );
	BOOST_CHECK( x.empty() );
	BOOST_CHECK( parseTo("{\"hello\":1}", x) );
	BOOST_CHECK_EQUAL( x.size(), 1u );
	BOOST_CHECK_EQUAL(x["hello"], 1);
	BOOST_CHECK( parseTo("{\"hello\":1, \"world\":5}", x) );
	BOOST_CHECK_EQUAL( x.size(), 2u );
	BOOST_CHECK_EQUAL(x["hello"], 1);
	BOOST_CHECK_EQUAL(x["world"], 5);
}

BOOST_AUTO_TEST_CASE ( startTest ) {
	std::map<std::string, int> x1;
	std::vector<int> x2;
	BOOST_CHECK( !parseTo("[]", x1) );
	BOOST_CHECK( !parseTo("{}", x2) );
	BOOST_CHECK( parseTo("[]", x2) );
	BOOST_CHECK( parseTo("{}", x1) );
}

BOOST_AUTO_TEST_CASE ( doubleTest ) {
	std::vector<double> x;
	BOOST_CHECK( parseTo("[]", x) );

	BOOST_CHECK( parseTo("[1.0,2.0]", x) );
	{
		std::vector<double> ethalon;
		ethalon += 1.0,2.0;
		BOOST_CHECK( cequals(x, ethalon) );
	}

	// special hack for integers as double
	BOOST_REQUIRE( parseTo("[8.6,15,9.12]", x) );
	{
		std::vector<double> ethalon;
		ethalon += 8.6,15,9.12;
		BOOST_CHECK( cequals(x, ethalon) );
	}
}

BOOST_AUTO_TEST_CASE ( boolTest ) {
	std::vector<bool> x;
	BOOST_CHECK( !parseTo("[1]", x) );
	BOOST_CHECK( !parseTo("[null]", x) );
	BOOST_CHECK( parseTo("[true]", x) );
	BOOST_CHECK( parseTo("[false]", x) );
	BOOST_CHECK( !parseTo("[ture]", x) );
	BOOST_CHECK( !parseTo("[truefalse]", x) );

	BOOST_CHECK( parseTo("[true,false]", x) );
	{
		std::vector<bool> ethalon;
		ethalon += true, false;
		BOOST_CHECK( cequals(x, ethalon) );
	}
}

BOOST_AUTO_TEST_CASE ( nullTest ) {
	std::vector<JsonEasy::Template::NullTag> x;
	BOOST_REQUIRE( parseTo("[]", x) );
	BOOST_REQUIRE( x.empty() );

	BOOST_REQUIRE( parseTo("[null]", x) );
	BOOST_CHECK_EQUAL( x.size(), 1u);

	BOOST_REQUIRE( parseTo("[null, null, null ]", x) );

	BOOST_CHECK_EQUAL( x.size(), 3u);
}

BOOST_AUTO_TEST_CASE ( optionalTest ) {
	std::vector<boost::optional<int> > x;
	BOOST_REQUIRE( parseTo("[1]", x) );
	BOOST_REQUIRE( x.back().is_initialized() );
	BOOST_CHECK_EQUAL( x.back(), 1);

	BOOST_REQUIRE( parseTo("[null]", x) );
	BOOST_REQUIRE( !x.back().is_initialized() );

	BOOST_REQUIRE( parseTo("[1,null,2]", x) );
	{
		std::vector<boost::optional<int> > ethalon;
		ethalon += 1, boost::optional<int>(), 2;
		BOOST_CHECK( cequals(x, ethalon) );
	}
}

BOOST_AUTO_TEST_CASE ( stringSimpleEscapingTest ) {
	std::vector<std::string> x;

	BOOST_REQUIRE( parseTo("[\"Hello\"]", x) );
	BOOST_CHECK_EQUAL( x.back(), "Hello");

	BOOST_REQUIRE( parseTo(R"(["Hello\"World"])", x) );
	BOOST_CHECK_EQUAL( x.back(), R"(Hello"World)");

	BOOST_REQUIRE( parseTo(R"(["Hello\\\"World"])", x) );
	BOOST_CHECK_EQUAL( x.back(), R"(Hello\"World)");

	BOOST_REQUIRE( parseTo(R"(["Hello\\"])", x) );
	BOOST_CHECK_EQUAL( x.back(), R"(Hello\)");

	BOOST_REQUIRE( parseTo("[\"Hello\\t\\n\\t\"]", x) );
	BOOST_CHECK_EQUAL( x.back(), "Hello\t\n\t");
}

BOOST_AUTO_TEST_CASE ( complexContainerTest ) {
	typedef std::list<int> list_t;
	typedef std::map<std::string, list_t> map_t;
	typedef std::vector<map_t> vec_t;

	vec_t x;
	BOOST_REQUIRE( parseTo("[]", x) );
	BOOST_REQUIRE( x.empty() );
	BOOST_REQUIRE( !parseTo("[1]", x) );

	BOOST_CHECK( !parseTo("[[]]", x) );
	BOOST_CHECK( parseTo("[{}]", x) );
	BOOST_CHECK( x.size() == 1u);
	BOOST_CHECK( x.back().empty() );

	BOOST_REQUIRE( parseTo(R"([{"hello":[]}])", x) );
	BOOST_REQUIRE(x.size() == 1u);
	BOOST_REQUIRE(x.back().size() == 1u);
	BOOST_REQUIRE(x.back().begin()->first == "hello");
	BOOST_REQUIRE(x.back().begin()->second.empty());

	BOOST_REQUIRE( parseTo(R"([{"hello":[4,2,3]}])", x) );

	BOOST_REQUIRE(x.size() == 1u);
	BOOST_REQUIRE(x.back().size() == 1u);
	const list_t& lst = x.back()["hello"];
	{
		std::vector<int> ethalon;
		ethalon += 4,2,3;
		BOOST_CHECK( cequals(lst, ethalon) );
	}
}

/**
 * Object keys in JSON a declraed to be only of string type.
 * But we want implicit conversion to work
 */
BOOST_AUTO_TEST_CASE ( mapWitCustomKey ) {
	std::map<int, int> x;
	BOOST_CHECK( parseTo("{}", x) );
	BOOST_REQUIRE( parseTo(R"({"6": 4,"0": 18})", x) );
	BOOST_REQUIRE_EQUAL( x.size(), 2u );
	BOOST_CHECK_EQUAL( x.at(6), 4);
	BOOST_CHECK_EQUAL( x.at(0), 18);
}

BOOST_AUTO_TEST_CASE( pairTest ) {
	std::pair<int,bool> x;
	// exactly two elements
	BOOST_CHECK( !parseTo("[]", x) );
	BOOST_CHECK( !parseTo("[1]", x) );
	BOOST_CHECK( !parseTo("[1,2]", x) );
	BOOST_CHECK( !parseTo("[1,true,1]", x) );
	BOOST_CHECK( !parseTo("[1,true,false]", x) );
	BOOST_CHECK( !parseTo("[true,1]", x) );

	// of the correct type
	BOOST_CHECK( parseTo("[1,true]", x) );
	BOOST_CHECK_EQUAL( x.first, 1);
	BOOST_CHECK_EQUAL( x.second, true);

	BOOST_CHECK( parseTo("[7,false]", x) );
	BOOST_CHECK_EQUAL( x.first, 7);
	BOOST_CHECK_EQUAL( x.second, false);
}

BOOST_AUTO_TEST_CASE( complexPairTest ) {
	// complex first
	{
		std::pair<std::vector<bool>,int> x;
		BOOST_CHECK( parseTo("[[],1]", x) );
		BOOST_CHECK( !parseTo("[{},1]", x) );
		BOOST_CHECK( parseTo("[[false,false,true],1]", x) );
		BOOST_CHECK( !parseTo("[[false,false,true],[]]", x) );
		BOOST_CHECK( !parseTo("[[false,false,true],{}]", x) );
	}
	// complex second
	{
		std::pair<int, std::vector<bool>> x;
		BOOST_CHECK( parseTo("[1,[]]", x) );
		BOOST_CHECK( !parseTo("[1,{}]", x) );
		BOOST_CHECK( parseTo("[1,[false,false,true]]", x) );
		BOOST_CHECK_EQUAL( x.second.size(), 3u);
	}
	// complex both
	{
		typedef std::pair<std::string, boost::optional<double> > t1;
		typedef std::pair<int, std::vector<int> > t2;
		typedef std::pair<t1, t2> t3;
		t3 x;
		BOOST_CHECK( !parseTo("[[],[]]", x) );
		BOOST_CHECK( parseTo(R"([["hi",null],[1,[]]])", x) );
	}
}

BOOST_AUTO_TEST_CASE( tupleTest ) {
	{
		std::tuple<int> x;
		BOOST_REQUIRE( parseTo("[6]", x) );
		BOOST_CHECK_EQUAL( std::get<0>(x), 6 );

		BOOST_CHECK( !parseTo("[]", x) );
		BOOST_CHECK( !parseTo("[6,7]", x) );
	}
	{
		std::tuple<int, bool, std::string, double> x;
		BOOST_REQUIRE( parseTo(R"([6,false,"hi",0.25])", x) );
		BOOST_CHECK_EQUAL( std::get<0>(x), 6 );
		BOOST_CHECK_EQUAL( std::get<1>(x), false );
		BOOST_CHECK_EQUAL( std::get<2>(x), "hi" );
		BOOST_CHECK_EQUAL( std::get<3>(x), 0.25 );
	}
	{
		std::tuple<std::pair<int,int>, bool, std::vector<std::string>, double> x;
		BOOST_REQUIRE( parseTo("[[6,7],false,[],0.25]", x) );
		BOOST_CHECK_EQUAL( std::get<0>(x).first, 6 );
		BOOST_CHECK_EQUAL( std::get<0>(x).second, 7 );
		BOOST_CHECK_EQUAL( std::get<1>(x), false );
		BOOST_CHECK( std::get<2>(x).empty() );
		BOOST_CHECK_EQUAL( std::get<3>(x), 0.25 );
	}

}

template<typename T, typename Y>
const T& getOne(const std::vector<Y>& x) {
	assert( x.size() == 1u);
	return boost::get<T>( x.back() );
}

BOOST_AUTO_TEST_CASE( doubleAsIntTest ) {
	std::vector<int> x;
	BOOST_REQUIRE( parseTo("[6]", x ) );
	BOOST_REQUIRE_EQUAL( x.size(), 1u);
	BOOST_REQUIRE_EQUAL( x.back(), 6);
	BOOST_REQUIRE( !parseTo("[6.5]", x ) );
}

BOOST_AUTO_TEST_CASE( variantTest ) {
	{
		std::vector<boost::variant<int> > x;
		BOOST_REQUIRE( parseTo("[6]", x ) );
		BOOST_CHECK_EQUAL(getOne<int>(x), 6);
	}
	{
		// will try int first
		std::vector<boost::variant<int, double> > x;
		BOOST_REQUIRE( parseTo("[4]", x ) );
		BOOST_CHECK_EQUAL(getOne<int>(x), 4);

		BOOST_REQUIRE( parseTo("[6.5]", x ) );
		BOOST_CHECK_EQUAL(getOne<double>(x), 6.5);
	}
	{
		// will try double first. Will be always double for this reason
		std::vector<boost::variant<double, int> > x;
		BOOST_REQUIRE( parseTo("[6]", x ) );
		BOOST_CHECK_EQUAL(getOne<double>(x), 6);

		BOOST_REQUIRE( parseTo("[6.5]", x ) );
		BOOST_CHECK_EQUAL(getOne<double>(x), 6.5);
	}
}

BOOST_AUTO_TEST_CASE( tupleVariantTest ) {
	std::tuple<boost::variant<int, double, std::string> > x;
	BOOST_REQUIRE( parseTo("[7]", x ) );
	BOOST_CHECK_EQUAL( boost::get<int>( std::get<0>(x) ), 7 );
}

template<typename T>
T& getFirst(std::tuple<T>& x) {
	return std::get<0>(x);
}

BOOST_AUTO_TEST_CASE( optinalContainer ) {
	std::tuple<boost::optional<std::vector<int> > > x;
	BOOST_REQUIRE( parseTo("[null]", x ) );
	BOOST_CHECK( !getFirst(x).is_initialized() );

	BOOST_REQUIRE( parseTo("[[]]", x ) );
	BOOST_REQUIRE( getFirst(x).is_initialized() );
	BOOST_REQUIRE( getFirst(x)->empty() );
}

BOOST_AUTO_TEST_CASE( variantContainerTest ) {
	typedef std::vector<int> vec_t;
	typedef std::map<std::string, std::string> map_t;
	boost::variant<vec_t,map_t> x;
	{
		BOOST_REQUIRE( parseTo("[7]", x ) );
		const auto& v = boost::get<vec_t>(x);
		BOOST_REQUIRE_EQUAL( v.size(), 1u);
		BOOST_CHECK_EQUAL( v.back(), 7);
	}
	{
		BOOST_REQUIRE( parseTo("[7,6]", x ) );
		const vec_t& v = boost::get<vec_t>(x);
		BOOST_REQUIRE_EQUAL( v.size(), 2u);
		BOOST_CHECK_EQUAL( v.back(), 6);
		BOOST_CHECK_EQUAL( v.front(), 7);
	}
	{
		BOOST_REQUIRE( parseTo("{}", x ) );
		const auto& v = boost::get<map_t>(x);
		BOOST_REQUIRE( v.empty() );
	}
	{
		BOOST_REQUIRE( parseTo(R"({"hello":"world"})", x ) );
		const auto& v = boost::get<map_t>(x);
		BOOST_REQUIRE_EQUAL( v.size(), 1u );
		BOOST_REQUIRE_EQUAL( v.at("hello"), "world" );
	}
}

// class type key test

// utf symbols tests
// int64
// uint
// char
// test for negative parsing to uint
// test for parsing value over numeric limits
// test for 1000 trying to parse to char type
// size_t
// negative keys for size_t

