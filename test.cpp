#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "spirit.hpp"

BOOST_AUTO_TEST_CASE ( simple ) {
  BOOST_CHECK( parse("null") );
  BOOST_CHECK( parse("false") );
  BOOST_CHECK( parse("true") );

  BOOST_CHECK( parse("1") );
  BOOST_CHECK( parse("-1") );
  BOOST_CHECK( parse("1.121") );

  BOOST_CHECK( parse("\"hello\"") );

  BOOST_CHECK( parse("[]") );
  BOOST_CHECK( parse("{}") );
}

BOOST_AUTO_TEST_CASE ( simple_negative ) {
  BOOST_CHECK( !parse("null1") );
  BOOST_CHECK( !parse("2false") );
  BOOST_CHECK( !parse("true3") );

  BOOST_CHECK( !parse("a1") );
  BOOST_CHECK( !parse("-f1") );
  BOOST_CHECK( !parse("1,1") );

  BOOST_CHECK( !parse("\"hello") );

  BOOST_CHECK( !parse("[") );
  BOOST_CHECK( !parse("}") );
}

BOOST_AUTO_TEST_CASE ( simple_spaces ) {
  BOOST_CHECK( parse("null") );
  BOOST_CHECK( parse(" null") );
  BOOST_CHECK( parse("  null") );
  BOOST_CHECK( parse("null ") );
  BOOST_CHECK( parse("null  ") );
  BOOST_CHECK( parse(" null ") );
  BOOST_CHECK( parse("  null  ") );
}

BOOST_AUTO_TEST_CASE ( simple_string ) {
  BOOST_CHECK( parse("\"\"") );
  BOOST_CHECK( parse("\"\\\"\"") );
  BOOST_CHECK( parse("\"\\\\\"") );
  BOOST_CHECK( parse("\"\\\\\\\"\"") );
  BOOST_CHECK( parse("\"hello world\"") );
}

BOOST_AUTO_TEST_CASE ( simple_array ) {
  BOOST_CHECK( parse("[]") );
  BOOST_CHECK( parse("[1]") );
  BOOST_CHECK( parse("[1,2]") );
  BOOST_CHECK( parse("[1,2,true, null, false,[]]") );
}

BOOST_AUTO_TEST_CASE ( simple_map ) {
  BOOST_CHECK( parse("{}") );
  BOOST_CHECK( parse("{\"hello\": 1}") );
  BOOST_CHECK( parse("{\"hello\": 1, \"world\": [{}]}") );
}


