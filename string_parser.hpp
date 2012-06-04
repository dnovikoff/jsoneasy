#ifndef _SPIRIT2JSON_STRING_PARSER_HPP_
#define _SPIRIT2JSON_STRING_PARSER_HPP_

#include <string>

#include <boost/scoped_ptr.hpp>

#include "types.hpp"

struct string_parser {
	string_parser();
	~string_parser();
	bool operator()(const std::string& input, client::value_type& output );
	bool operator()(const std::string& input);
private:
	typedef std::string::const_iterator iterator_type;
	typedef client::value_grammar<iterator_type> value_g;

	boost::scoped_ptr<value_g> grammar;
};

#endif // _SPIRIT2JSON_STRING_PARSER_HPP_
