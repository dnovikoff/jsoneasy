#include <core/parser.hpp>

#include "string_parser.hpp"

StringParser::StringParser() {
	grammar.reset(new value_g);
}

StringParser::~StringParser() {}

bool StringParser::operator()(const std::string& input, UserParser::Ptr& parser ) {
	namespace qi = boost::spirit::qi;
	iterator_type iter = input.begin();
	const iterator_type end = input.end();
	grammar->setTop( parser );
	const bool r = qi::phrase_parse(iter, end, *grammar, qi::ascii::space);
	return r && (iter == end);
}
