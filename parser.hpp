#ifndef _SPIRIT2JSON_PARSER_HPP_
#define _SPIRIT2JSON_PARSER_HPP_

#include <boost/typeof/typeof.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "types.hpp"

namespace client {
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;

	struct hex_ : qi::symbols<char, char> {
		hex_() {
			add
				("0"    , 0)
				("1"    , 1)
				("2"    , 2)
				("3"    , 3)
				("4"    , 4)
				("5"    , 5)
				("6"    , 6)
				("7"    , 7)
				("8"    , 8)
				("9"    , 9)
				("A"    , 10)
				("B"    , 11)
				("C"    , 12)
				("D"    , 13)
				("E"    , 14)
				("F"    , 15)
			;
		}
	} hex;

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
	} qchar;

	template <typename Iterator>
	class value_grammar : public qi::grammar<Iterator, value_type()> {
		typedef value_grammar<Iterator> self_t;
		size_t depth;
		static const size_t MAX_DEPTH = 20;
		bool incDepth() {
			return (++depth < MAX_DEPTH );
		}
		void decDepth() {
			--depth;
		}

		qi::rule<Iterator> skipper;

		qi::rule<Iterator, char()> hexchar;
		qi::rule<Iterator, std::string()> qstring;

		qi::rule<Iterator, vector_t() > array;

		qi::rule<Iterator, pair_t() > pair;
		qi::rule<Iterator, map_t() > map;

		qi::rule<Iterator, bool()> boolean;
		qi::rule<Iterator, null_type()> null;

		qi::rule<Iterator, value_type()> start;
	public:

		value_grammar() : value_grammar::base_type(start), depth(0) {
			using qi::lit;
			using qi::_val;
			using qi::_1;
			using qi::eps;
			using qi::int_;
			using qi::char_;
			using qi::_pass;
			using qi::double_;
			using boost::phoenix::push_back;
			using boost::phoenix::insert;
			using boost::phoenix::bind;
			using ascii::space;

			skipper = *space;

			hexchar = hex [ _val = _1*16 ] >> hex [ _val += _1 ];
			qstring = char_('\"') [ _val = "" ] >>
					*((lit('\\') >> (
					qchar [ _val += _1]
					| ( char_('u') >> hexchar [ _val += _1 ] >> hexchar [ _val += _1 ])
					)) | (char_ - '\"' - '\\' - '\t' - '\n' - '\r')[ _val += _1 ])
					>> '\"'
					;

			boolean = ( lit("true")[ _val = true ] ) | ( lit("false") [ _val = false ] );
			null = lit("null") [ _val = null_type() ];
			BOOST_AUTO(inc, bind(&self_t::incDepth, this));
			BOOST_AUTO(dec, bind(&self_t::decDepth, this));
			array = char_('[') [ _val = vector_t() ] >> eps[ _pass = inc ] >> ((start [ push_back( _val, _1 ) ] % ',') | skipper) >> char_(']') [ dec ];

			pair = skipper >> qstring [ bind( &pair_t::first, _val) = _1 ] >> skipper >> ':' >> start [ bind( &pair_t::second, _val) = _1 ];
			map = char_('{') [ _val = map_t() ] >> ((pair [ insert( _val, _1 ) ] % ',') | skipper) >> '}';

			start = skipper >> ((qstring | double_ | boolean | null | array| map) [ _val = _1 ]) >> skipper;
		}
	};
} // namespace client

#endif // SPIRIT2JSON_PARSER_HPP_
