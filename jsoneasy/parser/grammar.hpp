#ifndef JSONEASY_PARSER_GRAMMAR_HPP_
#define JSONEASY_PARSER_GRAMMAR_HPP_

#include <string>
#include <memory>
#include <stack>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <jsoneasy/parser/handler.hpp>

namespace JsonEasy {
namespace Parser   {
namespace qi    = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

struct qchar_ : qi::symbols<char, char> {
	qchar_() {
		add
			("\"", '\"')
			("\\", '\\')
			("/" , '/' )
			("b" , '\b')
			("f" , '\f')
			("n" , '\n')
			("r" , '\r')
			("t" , '\t')
		;
	}
};

/**
 * This is grammar to be used for parsing json
 */
template <typename Iterator>
class Grammar : public qi::grammar<Iterator> {
	typedef Grammar<Iterator> self_t;
	// It is said in doucmentation, that maximum allowed level is 20
	static const size_t maximumDepthAllowed = 20;
	static const size_t handlersLimit = maximumDepthAllowed;

	qi::rule<Iterator, std::string()> qstring;
	boost::spirit::qi::uint_parser<unsigned, 16, 4, 4> myhex;

	qi::rule<Iterator> array;

	qi::rule<Iterator> pair;
	qi::rule<Iterator> map;

	qi::rule<Iterator> value;

	qi::rule<Iterator> start;
	qi::rule<Iterator> skipper;

	qi::real_parser<double, qi::strict_real_policies<double> > strict_double;

	typedef std::stack<Handler::Ptr> stack_t;
	stack_t handlers;

	Handler& currentHandler() const { return *handlers.top(); }

	// helper funciton
	template<typename T>
	bool p(T& x) {
		return currentHandler()( x );
	}

	bool intParsed(int64_t x) {
		Integer i(x);
		return p( i );
	}
	bool boolParsed(bool x) {
		return p( x );
	}
	bool doubleParsed(double x) {
		return p( x );
	}
	// Hack for leading zeros
	bool zeroParsed() {
		return intParsed(0);
	}
	bool nullParsed() {
		return currentHandler().null();
	}
	bool stringParsed(std::string& x) {
		return p( x );
	}
	bool keyParsed(std::string& x) {
		return currentHandler().key( x );
	}
	void pushHandler(Handler::Ptr& x) {
		handlers.push( std::move(x) );
	}
	bool oneMoreHandlerAllowed() {
		return handlers.size() < handlersLimit;
	}
	bool newObject() {
		if(!oneMoreHandlerAllowed()) return false;
		if(auto x = currentHandler().object()) {
			pushHandler( x );
			return true;
		}
		return false;
	}
	bool newArray() {
		if(!oneMoreHandlerAllowed()) return false;
		if(auto x = currentHandler().array()) {
			pushHandler( x );
			return true;
		}
		return false;
	}
	bool onParsed() {
		if( !currentHandler().onParsed() ) {
			return false;
		}
		handlers.pop();
		return true;
	}
public:
	/**
	 * If using grammar you should set top handler for parsing to
	 */
	void setTop(Handler::Ptr& topHandler) {
		stack_t tmp;
		tmp.push( std::move(topHandler) );
		tmp.swap( handlers );
	}
	void initQString() {
		using qi::hex;
		using qi::_val;
		using qi::char_;
		using qi::_1;
		static const qchar_ qchar;
		qstring = char_('\"') [ _val = "" ] >>
			*(
				(char_('\\') >>
					( ( char_('u') >> myhex [ _val += _1 ] )
					|
					qchar [ _val += _1 ]
				)
			) | (char_ - '\"' - '\\' - '\t' - '\n' - '\r')[ _val += _1 ] )
			>> '\"'
		;
	}
	void initGrammar() {
		using qi::lit;
		using qi::long_long;
		using qi::char_;
		using qi::bool_;
		using qi::_pass;

#define BIND(X) _pass = boost::phoenix::bind(&self_t::X, this)
#define BIND1(X) _pass = boost::phoenix::bind(&self_t::X, this, boost::spirit::_1)
		array = char_('[') [ BIND(newArray) ] >> skipper >> -(value % ',') >> char_(']') [ BIND(onParsed) ];

		pair = skipper >> qstring [ BIND1(keyParsed) ] >> skipper >> char_(':') >> value ;
		map = char_('{') [ _pass = BIND(newObject) ] >> skipper >> -(pair % ',') >> char_('}') [ BIND(onParsed) ];
		value = skipper >> (
				(lit("null")[ BIND(nullParsed) ]) |
				qstring [ BIND1(stringParsed) ] |
				strict_double [ BIND1(doubleParsed) ] |
				// leading zero hack
				char_('0')[ BIND(zeroParsed)] |
				long_long[ BIND1(intParsed) ] |
				bool_[ BIND1(boolParsed) ] |
				array | map
				) >> skipper;

		start = array | map;
#undef BIND
#undef BIND1
	}
	Grammar() : Grammar::base_type(start) {
		skipper = *ascii::space;
		initQString();
		initGrammar();
	}
};

} // namespace Parser
} // namespace JsonEasy

#endif // JSONEASY_PARSER_GRAMMAR_HPP_
