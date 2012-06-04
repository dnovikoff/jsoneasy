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

BOOST_AUTO_TEST_CASE ( json_checker ) { // http://json.org/JSON_checker/
	//fail24.json
	BOOST_CHECK(! parse("[\'single quote\']") );

	//fail2.json
	BOOST_CHECK(! parse("[\"Unclosed array\"") );

	//fail3.json
	BOOST_CHECK(! parse("{unquoted_key: \"keys must be quoted\"}") );

	//fail11.json
	BOOST_CHECK(! parse("{\"Illegal expression\": 1 + 2}") );

	//fail13.json
	BOOST_CHECK(! parse("{\"Numbers cannot have leading zeroes\": 013}") );

	//fail15.json
	BOOST_CHECK(! parse("[\"Illegal backslash escape: \\x15\"]") );

	//pass3.json
	BOOST_CHECK( parse("{\\n    \"JSON Test Pattern pass3\": {\\n        \"The outermost value\": \"must be an object or array.\",\\n        \"In this test\": \"It is an object.\"\\n    }\\n}\\n") );

	//fail31.json
	BOOST_CHECK(! parse("[0e+-1]") );

	//fail19.json
	BOOST_CHECK(! parse("{\"Missing colon\" null}") );

	//fail21.json
	BOOST_CHECK(! parse("{\"Comma instead of colon\", null}") );

	//fail1.json
	BOOST_CHECK(! parse("\"A JSON payload should be an object or array, not a string.\"") );

	//fail7.json
	BOOST_CHECK(! parse("[\"Comma after the close\"],") );

	//fail27.json
	BOOST_CHECK(! parse("[\"line\\nbreak\"]") );

	//fail33.json
	BOOST_CHECK(! parse("[\"mismatch\"}") );

	//fail17.json
	BOOST_CHECK(! parse("[\"Illegal backslash escape: \\017\"]") );

	//fail28.json
	BOOST_CHECK(! parse("[\"line\\\\nbreak\"]") );

	//fail6.json
	BOOST_CHECK(! parse("[   , \"<-- missing value\"]") );

	//fail8.json
	BOOST_CHECK(! parse("[\"Extra close\"]]") );

	//fail20.json
	BOOST_CHECK(! parse("{\"Double colon\":: null}") );

	//fail29.json
	BOOST_CHECK(! parse("[0e]") );

	//fail12.json
	BOOST_CHECK(! parse("{\"Illegal invocation\": alert()}") );

	//fail5.json
	BOOST_CHECK(! parse("[\"double extra comma\",,]") );

	//pass1.json
	BOOST_CHECK( parse("[\\n    \"JSON Test Pattern pass1\",\\n    {\"object with 1 member\":[\"array with 1 element\"]},\\n    {},\\n    [],\\n    -42,\\n    true,\\n    false,\\n    null,\\n    {\\n        \"integer\": 1234567890,\\n        \"real\": -9876.543210,\\n        \"e\": 0.123456789e-12,\\n        \"E\": 1.234567890E+34,\\n        \"\":  23456789012E66,\\n        \"zero\": 0,\\n        \"one\": 1,\\n        \"space\": \" \",\\n        \"quote\": \"\\\"\",\\n        \"backslash\": \"\\\\\",\\n        \"controls\": \"\\b\\f\\n\\r\\t\",\\n        \"slash\": \"/ & \\/\",\\n        \"alpha\": \"abcdefghijklmnopqrstuvwyz\",\\n        \"ALPHA\": \"ABCDEFGHIJKLMNOPQRSTUVWYZ\",\\n        \"digit\": \"0123456789\",\\n        \"0123456789\": \"digit\",\\n        \"special\": \"`1~!@#$%^&*()_+-={\':[,]}|;.</>?\",\\n        \"hex\": \"\\u0123\\u4567\\u89AB\\uCDEF\\uabcd\\uef4A\",\\n        \"true\": true,\\n        \"false\": false,\\n        \"null\": null,\\n        \"array\":[  ],\\n        \"object\":{  },\\n        \"address\": \"50 St. James Street\",\\n        \"url\": \"http://www.JSON.org/\",\\n        \"comment\": \"// /* <!-- --\",\\n        \"# -- --> */\": \" \",\\n        \" s p a c e d \" :[1,2 , 3\\n\\n,\\n\\n4 , 5        ,          6           ,7        ],\"compact\":[1,2,3,4,5,6,7],\\n        \"jsontext\": \"{\\\"object with 1 member\\\":[\\\"array with 1 element\\\"]}\",\\n        \"quotes\": \"&#34; \\u0022 %22 0x22 034 &#x22;\",\\n        \"\\/\\\\\\\"\\uCAFE\\uBABE\\uAB98\\uFCDE\\ubcda\\uef4A\\b\\f\\n\\r\\t`1~!@#$%^&*()_+-=[]{}|;:\',./<>?\"\\n: \"A key can be any string\"\\n    },\\n    0.5 ,98.6\\n,\\n99.44\\n,\\n\\n1066,\\n1e1,\\n0.1e1,\\n1e-1,\\n1e00,2e+00,2e-00\\n,\"rosebud\"]") );

	//fail9.json
	BOOST_CHECK(! parse("{\"Extra comma\": true,}") );

	//fail16.json
	BOOST_CHECK(! parse("[\\naked]") );

	//fail25.json
	BOOST_CHECK(! parse("[\"	tab	character	in	string	\"]") );

	//fail18.json
	BOOST_CHECK(! parse("[[[[[[[[[[[[[[[[[[[[\"Too deep\"]]]]]]]]]]]]]]]]]]]]") );

	//fail10.json
	BOOST_CHECK(! parse("{\"Extra value after close\": true} \"misplaced quoted value\"") );

	//fail32.json
	BOOST_CHECK(! parse("{\"Comma instead if closing brace\": true,") );

	//fail14.json
	BOOST_CHECK(! parse("{\"Numbers cannot be hex\": 0x14}") );

	//fail4.json
	BOOST_CHECK(! parse("[\"extra comma\",]") );

	//fail23.json
	BOOST_CHECK(! parse("[\"Bad value\", truth]") );

	//fail22.json
	BOOST_CHECK(! parse("[\"Colon instead of comma\": false]") );

	//pass2.json
	BOOST_CHECK( parse("[[[[[[[[[[[[[[[[[[[\"Not too deep\"]]]]]]]]]]]]]]]]]]]") );

	//fail26.json
	BOOST_CHECK(! parse("[\"tab\\   character\\   in\\  string\\  \"]") );

	//fail30.json
	BOOST_CHECK(! parse("[0e+]") );
}

