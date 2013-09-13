#ifndef TEST_PARSE_STRING_HPP_
#define TEST_PARSE_STRING_HPP_

#include <jsoneasy/parser/string_parser.hpp>
#include <jsoneasy/template/create.hpp>

template<typename T>
static bool parseString(const std::string& input, T& data) {
	JsonEasy::Parser::Handler::Ptr h = JsonEasy::Template::createHandler(data);
	JsonEasy::Parser::StringParser sp;
	if( !sp.parse(input, h) ) return false;
	return true;
}

#endif /* TEST_PARSE_STRING_HPP_ */
