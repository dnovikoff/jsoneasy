#include "string_parser.hpp"
#include "parser.hpp"

string_parser::string_parser() {
	grammar.reset(new value_g);
}

string_parser::~string_parser() {}

bool string_parser::operator()(const std::string& input, client::value_type& output ) {
	namespace qi = boost::spirit::qi;
	std:: cout << "Parsing: " << input << std::endl;

	iterator_type iter = input.begin();
	iterator_type end = input.end();
	bool r = qi::parse(iter, end, *grammar, output);
	std:: cout << "Parse result: " << (r?"OK":"ERROR") << std::endl;
	std:: cout << "Parsed to the end: " << ((iter == end)?"YES":"NO") << std::endl;
	if(iter != end) {
		std::cout << "Parse error near: " << std::string(iter, end) << std::endl;
	}
	return r && (iter == end);
}

bool string_parser::operator()(const std::string& input) {
	client::value_type output;
	return operator()( input, output);
}