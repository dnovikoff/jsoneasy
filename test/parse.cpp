#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>
#include <boost/typeof/typeof.hpp>

#include <jsoneasy/parser/string_parser.hpp>
#include <jsoneasy/parser/handler.hpp>

/**
 * Parse all
 */
class MyParser: public JsonEasy::Parser::Handler {
	const size_t depth;
	std::ostringstream tmpstream;
public:
	std::ostream& shift() {
		tmpstream << std::string(depth,' ');
		return tmpstream;
	}
	explicit MyParser(const size_t d = 0):depth(d) {}

	bool operator()(JsonEasy::Parser::Integer i) override {
		int x = 0;
		i.to( x );
		shift() << "int=" << x << std::endl;
		return true;
	}
	bool operator()(bool x) override {
		shift() << "bool=" << x << std::endl;
		return true;
	}
	bool operator()(double x) override {
		shift() << "double=" << x << std::endl;
		return true;
	}
	bool operator()(std::string& x) override {
		shift() << "string=" << x << std::endl;
		return true;
	}
	bool null() override {
		shift() << "null" << std::endl;
		return true;
	}
	bool key(std::string& x) override {
		shift() << "key=" << x << std::endl;
		return true;
	}

	Ptr object() override {
		shift() << "object{" << std::endl;
		return Ptr(new MyParser(depth+1));
	}
	Ptr array() override {
		shift() << "array[" << std::endl;
		return Ptr(new MyParser(depth+1));
	}

	bool onParsed() override {
		shift() << "}/]" << std::endl;
		return true;
	}
};


class MyStringParser: public JsonEasy::Parser::StringParser {
public:
	bool operator()(const std::string& input) {
		JsonEasy::Parser::Handler::Ptr h(new MyParser);
		return parse(input, h);
	}
};

namespace {

bool parse(const std::string& input ) {
	MyStringParser p;
	return p( input );
}

} // namespace

BOOST_AUTO_TEST_CASE ( whiteSpace ) {
	BOOST_CHECK( parse("[]") );
	BOOST_CHECK( parse(" []") );
	BOOST_CHECK( parse("[] ") );
	BOOST_CHECK( parse("[ ]") );
	BOOST_CHECK( parse(" [ ] ") );
	BOOST_CHECK( parse("       [    \n  \n\n]   \t\t  \t ") );
}

BOOST_AUTO_TEST_CASE ( simple ) {
	BOOST_CHECK( parse("[null]") );
	BOOST_CHECK( parse("[false]") );
	BOOST_CHECK( parse("[true]") );

	BOOST_CHECK( parse("[1]") );
	BOOST_CHECK( parse("[-1]") );
	BOOST_CHECK( parse("[1.121]") );

	BOOST_CHECK( parse("[\"hello\"]") );

	BOOST_CHECK( parse("[]") );
	BOOST_CHECK( parse("{}") );
}

BOOST_AUTO_TEST_CASE ( simple_negative ) {
	BOOST_CHECK( !parse("[null1]") );
	BOOST_CHECK( !parse("[2false]") );
	BOOST_CHECK( !parse("[true3]") );

	BOOST_CHECK( !parse("[a1]") );
	BOOST_CHECK( !parse("[-f1]") );
	BOOST_CHECK( !parse("1,1") );

	BOOST_CHECK( !parse("[\"hello]") );

	BOOST_CHECK( !parse("[") );
	BOOST_CHECK( !parse("}") );
}

BOOST_AUTO_TEST_CASE ( simple_spaces ) {
	BOOST_CHECK( parse("[null]") );
	BOOST_CHECK( parse("[ null]") );
	BOOST_CHECK( parse("[  null]") );
	BOOST_CHECK( parse("[null ]") );
	BOOST_CHECK( parse("[null  ]") );
	BOOST_CHECK( parse("[ null ]") );
	BOOST_CHECK( parse("[  null  ]") );
}

BOOST_AUTO_TEST_CASE ( simple_string ) {
	BOOST_CHECK( parse("[\"\"]") );
	BOOST_CHECK( parse("[\"\\\"\"]") );
	BOOST_CHECK( parse("[\"\\\\\"]") );
	BOOST_CHECK( parse("[\"\\\\\\\"\"]") );
	BOOST_CHECK( parse("[\"hello world\"]") );
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

BOOST_AUTO_TEST_CASE ( simple_hex ) {
	BOOST_CHECK( parse("[\"\\uCAFE\"]") ); // Upper case
	BOOST_CHECK( !parse("[\"\\uCAF\"]") );
	BOOST_CHECK( !parse("[\"\\UCAFE\"]") );
	BOOST_CHECK( parse("[\"\\ubcda\"]") ); // Lower case
	BOOST_CHECK( parse("[\"\\uef4A\"]") ); // Combined case

	BOOST_CHECK( parse("{\"\\uCAFE\\uBABE\\uAB98\\uFCDE\\ubcda\\uef4A\":\"A key can be any string\"}") );
}

BOOST_AUTO_TEST_CASE ( jsonChecker ) {
	BOOST_CHECK( ! parse("\"A JSON payload should be an object or array, not a string.\"") );
	BOOST_CHECK( /* fail10.json */ ! parse("{\"Extra value after close\": true} \"misplaced quoted value\"") );
	BOOST_CHECK( /* fail11.json */ ! parse("{\"Illegal expression\": 1 + 2}") );
	BOOST_CHECK( /* fail12.json */ ! parse("{\"Illegal invocation\": alert()}") );
	BOOST_CHECK( /* fail13.json */ ! parse("{\"Numbers cannot have leading zeroes\": 013}") );
	BOOST_CHECK( /* fail14.json */ ! parse("{\"Numbers cannot be hex\": 0x14}") );
	BOOST_CHECK( /* fail15.json */ ! parse("[\"Illegal backslash escape: \\x15\"]") );
	BOOST_CHECK( /* fail16.json */ ! parse("[\\naked]") );
	BOOST_CHECK( /* fail17.json */ ! parse("[\"Illegal backslash escape: \\017\"]") );
	BOOST_CHECK( /* fail18.json */ ! parse("[[[[[[[[[[[[[[[[[[[[\"Too deep\"]]]]]]]]]]]]]]]]]]]]") );
	BOOST_CHECK( /* fail19.json */ ! parse("{\"Missing colon\" null}") );
	BOOST_CHECK( /* fail2.json */ ! parse("[\"Unclosed array\"") );
	BOOST_CHECK( /* fail20.json */ ! parse("{\"Double colon\":: null}") );
	BOOST_CHECK( /* fail21.json */ ! parse("{\"Comma instead of colon\", null}") );
	BOOST_CHECK( /* fail22.json */ ! parse("[\"Colon instead of comma\": false]") );
	BOOST_CHECK( /* fail23.json */ ! parse("[\"Bad value\", truth]") );
	BOOST_CHECK( /* fail24.json */ ! parse("[\'single quote\']") );
	BOOST_CHECK( /* fail25.json */ ! parse("[\"	tab	character	in	string	\"]") );
	BOOST_CHECK( /* fail26.json */ ! parse("[\"tab\\   character\\   in\\  string\\  \"]") );
	BOOST_CHECK( /* fail27.json */ ! parse("[\"line\nbreak\"]") );
	BOOST_CHECK( /* fail28.json */ ! parse("[\"line\\\nbreak\"]") );
	BOOST_CHECK( /* fail29.json */ ! parse("[0e]") );
	BOOST_CHECK( /* fail3.json */ ! parse("{unquoted_key: \"keys must be quoted\"}") );
	BOOST_CHECK( /* fail30.json */ ! parse("[0e+]") );
	BOOST_CHECK( /* fail31.json */ ! parse("[0e+-1]") );
	BOOST_CHECK( /* fail32.json */ ! parse("{\"Comma instead if closing brace\": true,") );
	BOOST_CHECK( /* fail33.json */ ! parse("[\"mismatch\"}") );
	BOOST_CHECK( /* fail4.json */ ! parse("[\"extra comma\",]") );
	BOOST_CHECK( /* fail5.json */ ! parse("[\"double extra comma\",,]") );
	BOOST_CHECK( /* fail6.json */ ! parse("[   , \"<-- missing value\"]") );
	BOOST_CHECK( /* fail7.json */ ! parse("[\"Comma after the close\"],") );
	BOOST_CHECK( /* fail8.json */ ! parse("[\"Extra close\"]]") );
	BOOST_CHECK( /* fail9.json */ ! parse("{\"Extra comma\": true,}") );

	BOOST_CHECK( /* pass1.json */  parse("[\n    \"JSON Test Pattern pass1\",\n    {\"object with 1 member\":[\"array with 1 element\"]},\n    {},\n    [],\n    -42,\n    true,\n    false,\n    null,\n    {\n        \"integer\": 1234567890,\n        \"real\": -9876.543210,\n        \"e\": 0.123456789e-12,\n        \"E\": 1.234567890E+34,\n        \"\":  23456789012E66,\n        \"zero\": 0,\n        \"one\": 1,\n        \"space\": \" \",\n        \"quote\": \"\\\"\",\n        \"backslash\": \"\\\\\",\n        \"controls\": \"\\b\\f\\n\\r\\t\",\n        \"slash\": \"/ & \\/\",\n        \"alpha\": \"abcdefghijklmnopqrstuvwyz\",\n        \"ALPHA\": \"ABCDEFGHIJKLMNOPQRSTUVWYZ\",\n        \"digit\": \"0123456789\",\n        \"0123456789\": \"digit\",\n        \"special\": \"`1~!@#$%^&*()_+-={\':[,]}|;.</>?\",\n        \"hex\": \"\\u0123\\u4567\\u89AB\\uCDEF\\uabcd\\uef4A\",\n        \"true\": true,\n        \"false\": false,\n        \"null\": null,\n        \"array\":[  ],\n        \"object\":{  },\n        \"address\": \"50 St. James Street\",\n        \"url\": \"http://www.JSON.org/\",\n        \"comment\": \"// /* <!-- --\",\n        \"# -- --> */\": \" \",\n        \" s p a c e d \" :[1,2 , 3\n\n,\n\n4 , 5        ,          6           ,7        ],\"compact\":[1,2,3,4,5,6,7],\n        \"jsontext\": \"{\\\"object with 1 member\\\":[\\\"array with 1 element\\\"]}\",\n        \"quotes\": \"&#34; \\u0022 %22 0x22 034 &#x22;\",\n        \"\\/\\\\\\\"\\uCAFE\\uBABE\\uAB98\\uFCDE\\ubcda\\uef4A\\b\\f\\n\\r\\t`1~!@#$%^&*()_+-=[]{}|;:\',./<>?\"\n: \"A key can be any string\"\n    },\n    0.5 ,98.6\n,\n99.44\n,\n\n1066,\n1e1,\n0.1e1,\n1e-1,\n1e00,2e+00,2e-00\n,\"rosebud\"]") );
	BOOST_CHECK( /* pass2.json */  parse("[[[[[[[[[[[[[[[[[[[\"Not too deep\"]]]]]]]]]]]]]]]]]]]") );
	BOOST_CHECK( /* pass3.json */  parse("{\n    \"JSON Test Pattern pass3\": {\n        \"The outermost value\": \"must be an object or array.\",\n        \"In this test\": \"It is an object.\"\n    }\n}\n") );
}

