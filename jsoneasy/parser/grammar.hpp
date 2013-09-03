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
#include <jsoneasy/parser/exception.hpp>

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

	template<typename T>
	void unexpectedException(const char * x, const T& val) {
		throw UnexpectedException(x, val);
	}

	void unexpectedException(const char * x, const std::string& val) {
		throw UnexpectedException(x, val.size()); // Avoid output corruption. Just size
	}

	void unexpectedException(const char * x) {
		throw UnexpectedException(x);
	}

	void intParsed(const int x) {
		if(!currentHandler()( x )) {
			unexpectedException("int", x);
		}
	}
	void boolParsed(const bool x) {
		if(!currentHandler()( x )) {
			unexpectedException("bool", x);
		}
	}
	void doubleParsed(double x) {
		if(!currentHandler()( x )) {
			unexpectedException("double", x);
		}
	}
	// Hack for leading zeros
	void zeroParsed() {
		intParsed(0);
	}
	void nullParsed() {
		if(!currentHandler().null()) {
			unexpectedException("null");
		}
	}
	void stringParsed(std::string& x) {
		if(!currentHandler()( x )) {
			unexpectedException("string", x);
		}
	}
	void keyParsed(std::string& x) {
		if(!currentHandler().key( x )) {
			unexpectedException("key", x);
		}
	}
	void pushHandler(Handler::Ptr& x) {
		handlers.push( std::move(x) );
	}
	void newObject() {
		auto x = currentHandler().object();
		if(!x) {
			unexpectedException("object");
		}
		pushHandler( x );
	}
	void newArray() {
		auto x = currentHandler().array();
		if(!x) {
			unexpectedException("array");
		}
		pushHandler( x );
	}
	void onParsed() {
		if( !currentHandler().onParsed() ) {
			unexpectedException("content");
		}
		handlers.pop();
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
		using qi::int_;
		using qi::char_;
		using qi::bool_;

#define BIND(X) boost::phoenix::bind(&self_t::X, this)
#define BIND1(X) boost::phoenix::bind(&self_t::X, this, boost::spirit::_1)
		array = char_('[') [ BIND(newArray) ] >> skipper >> -(value % ',') >> char_(']') [ BIND(onParsed) ];

		pair = skipper >> qstring [ BIND1(keyParsed) ] >> skipper >> char_(':') >> value ;
		map = char_('{') [ BIND(newObject) ] >> skipper >> -(pair % ',') >> char_('}') [ BIND(onParsed) ];
		value = skipper >> (
				(lit("null")[ BIND(nullParsed) ]) |
				qstring [ BIND1(stringParsed) ] |
				strict_double [ BIND1(doubleParsed) ] |
				// leading zero hack
				char_('0')[ BIND(zeroParsed)] |
				int_[ BIND1(intParsed) ] |
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
