#include <boost/spirit/include/support_istream_iterator.hpp>

#include "grammar.hpp"
#include "handler.hpp"

#include "stream_parser.hpp"

namespace JsonEasy {
namespace Parser {

typedef boost::spirit::istream_iterator iterator_type;
typedef Grammar<iterator_type> GrammarType;

StreamParser::StreamParser() {
	data = std::make_shared<GrammarType>();
}

StreamParser::~StreamParser() {}

bool StreamParser::parse(std::istream& input, std::unique_ptr<Handler>& parser ) {
	namespace qi = boost::spirit::qi;
	iterator_type iter( input );
	iterator_type end;
	GrammarType& grammar = *reinterpret_cast<GrammarType*>(data.get());
	grammar.setTop( parser );
	const bool r = qi::phrase_parse(iter, end, grammar, qi::ascii::space);
	return r && (iter == end);
}

} // namespace Parser
} // namespace JsonEasy
