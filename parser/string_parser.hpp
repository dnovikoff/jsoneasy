#ifndef _SPIRIT2JSON_STRING_PARSER_HPP_
#define _SPIRIT2JSON_STRING_PARSER_HPP_

#include <string>
#include <memory>
#include <core/parser.hpp>
class UserParser;

class StringParser {
	typedef std::string::const_iterator iterator_type;
	typedef client::value_grammar<iterator_type> value_g;
	std::unique_ptr<value_g> grammar;
public:
	StringParser();
	~StringParser();
	bool operator()(const std::string& input, UserParser::Ptr& parser );
private:



};

#endif // _SPIRIT2JSON_STRING_PARSER_HPP_
