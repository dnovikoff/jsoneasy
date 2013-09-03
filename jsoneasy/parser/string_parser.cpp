#include "grammar.hpp"
#include "handler.hpp"
#include "string_parser.hpp"

namespace JsonEasy {
namespace Parser {

typedef std::string::const_iterator iterator_type;
typedef Grammar<iterator_type> GrammarType;

StringParser::StringParser() {
	data = std::make_shared<GrammarType>();
}

StringParser::~StringParser() {}

bool StringParser::parse(const std::string& input, std::unique_ptr<Handler>& parser ) {
	namespace qi = boost::spirit::qi;
	iterator_type iter = input.begin();
	const iterator_type end = input.end();
	GrammarType& grammar = *reinterpret_cast<GrammarType*>(data.get());
	grammar.setTop( parser );
	const bool r = qi::phrase_parse(iter, end, grammar, qi::ascii::space);
	return r && (iter == end);
}

} // namespace Parser
} // namespace JsonEasy
