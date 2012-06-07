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
	class qstring_grammar : public qi::grammar<Iterator, std::string()> {
		qi::rule<Iterator, std::string()> start;
		boost::spirit::qi::uint_parser<unsigned, 16, 4, 4> myhex;
	public:
		qstring_grammar() : qstring_grammar::base_type(start) {
			using qi::hex;
			using qi::_val;
			using qi::char_;
			using qi::_1;

			start = char_('\"') [ _val = "" ] >>
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
	};

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
		void reset() {
			depth = 0;
		}

		qi::rule<Iterator> skipper;

		qstring_grammar<Iterator> qstring;

		qi::rule<Iterator, vector_t() > array;

		qi::rule<Iterator, pair_t() > pair;
		qi::rule<Iterator, map_t() > map;

		qi::rule<Iterator, null_type()> null;

		qi::rule<Iterator, value_type()> value;

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
			using qi::bool_;
			using boost::phoenix::push_back;
			using boost::phoenix::insert;
			using boost::phoenix::bind;
			using ascii::space;

			skipper = *space;

			null = lit("null") [ _val = null_type() ];
			BOOST_AUTO(inc, bind(&self_t::incDepth, this));
			BOOST_AUTO(dec, bind(&self_t::decDepth, this));
			array = char_('[') [ _val = vector_t() ] >> eps[ _pass = inc ] >> ((value [ push_back( _val, _1 ) ] % ',') | skipper) >> char_(']') [ dec ];

			pair = skipper >> qstring [ bind( &pair_t::first, _val) = _1 ] >> skipper >> ':' >> value [ bind( &pair_t::second, _val) = _1 ];
			map = char_('{') [ _val = map_t() ] >> ((pair [ insert( _val, _1 ) ] % ',') | skipper) >> '}';

			value = skipper >> ((qstring | double_ | bool_ | null | array| map) [ _val = _1 ]) >> skipper;
			start = eps[ bind(&self_t::reset, this) ] >> skipper >> (array | map) >> skipper;
		}
	};
} // namespace client

#endif // SPIRIT2JSON_PARSER_HPP_
