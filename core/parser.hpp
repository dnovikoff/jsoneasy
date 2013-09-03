#ifndef _SPIRIT2JSON_PARSER_HPP_
#define _SPIRIT2JSON_PARSER_HPP_

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <map>
#include <memory>

#include "user_parser.hpp"

namespace client {
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;

	struct qchar_ : qi::symbols<char, char> {
		qchar_() {
			add
				("\""    , '\"')
				("\\"    , '\\')
				("/"    , '/')
				("b"    , '\b')
				("f"    , '\f')
				("n"    , '\n')
				("r"    , '\r')
				("t"    , '\t')
			;
		}
	};
	typedef std::runtime_error ParseException;

	template <typename Iterator>
	class value_grammar : public qi::grammar<Iterator> {
		typedef value_grammar<Iterator> self_t;


		qi::rule<Iterator, std::string()> qstring;
		boost::spirit::qi::uint_parser<unsigned, 16, 4, 4> myhex;

		qi::rule<Iterator> array;

		qi::rule<Iterator> pair;
		qi::rule<Iterator> map;

		qi::rule<Iterator> value;

		qi::rule<Iterator> start;
		qi::rule<Iterator> skipper;

		qi::real_parser<double, qi::strict_real_policies<double> > strict_double;

		typedef std::stack<UserParser::Ptr> stack_t;
		stack_t parsers;

		UserParser& currentParser() { return *parsers.top(); }

		template<typename T>
		void unexpectedException(const char * x, const T& value) {
			std::ostringstream oss;
			oss << "Unexpected " << x << " = (" << value << ")";
			throw ParseException( oss.str());
		}

		void unexpectedException(const char * x) {
			std::ostringstream oss;
			oss << "Unexpected " << x;
			throw ParseException( oss.str());
		}

		void intParsed(const int x) {
			if(!currentParser()( x )) {
				unexpectedException("int", x);
			}
		}
		void boolParsed(const bool x) {
			if(!currentParser()( x )) {
				unexpectedException("bool", x);
			}
		}
		void doubleParsed(double x) {
			if(!currentParser()( x )) {
				unexpectedException("double", x);
			}
		}
		// Hack for leading zeros
		void zeroParsed() {
			intParsed(0);
		}
		void nullParsed() {
			if(!currentParser().null()) {
				unexpectedException("null");
			}
		}
		void stringParsed(std::string& x) {
			if(!currentParser()( x )) {
				unexpectedException("string", x);
			}
		}
		void keyParsed(std::string& x) {
			if(!currentParser().key( x )) {
				unexpectedException("key", x);
			}
		}
		void pushParser(UserParser::Ptr& x) {
			parsers.push( std::move(x) );
		}
		void newObject() {
			auto x = currentParser().object();
			if(!x) {
				unexpectedException("object");
			}
			pushParser( x );
		}
		void newArray() {
			auto x = currentParser().array();
			if(!x) {
				unexpectedException("array");
			}
			pushParser( x );
		}
		void onParsed() {
			if( !currentParser().onParsed() ) {
				unexpectedException("content");
			}
			parsers.pop();
		}
	public:
		void setTop(UserParser::Ptr& topParser) {
			stack_t tmp;
			tmp.push( std::move(topParser) );
			tmp.swap( parsers );
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
		value_grammar() : value_grammar::base_type(start) {
			using qi::lit;
			using qi::_val;
			using qi::eps;
			using qi::int_;
			using qi::char_;
			using qi::_pass;
			using qi::double_;
			using qi::bool_;
			using boost::phoenix::push_back;
			using boost::phoenix::insert;
			skipper = *ascii::space;

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
			initQString();
		}
	};
} // namespace client

#endif // SPIRIT2JSON_PARSER_HPP_
