/*
 * This pack is the same as test2. Except it loads data from .json files in compile time
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/typeof/typeof.hpp>

#include <parser/string_parser.hpp>

#define INCLUDE_FILE(NAME,DIR,EXT) \
asm( \
  #NAME "_begin:\n" \
  ".incbin \"" DIR #NAME EXT"\"\n" \
  #NAME "_end:\n" \
  ".byte 0\n" \
  #NAME"_len:\n" \
  ".long .-" #NAME "_begin\n" \
  ); \
extern char NAME##_begin;\

#define INCLUDE_TEST(NAME) INCLUDE_FILE(NAME, "test/data/",".json")

#define SHOW(NAME) INCLUDE_TEST(NAME); std::cout << &NAME##_begin << std::endl;
#define POSITIVE(NAME) INCLUDE_TEST(NAME); BOOST_CHECK( parse( &NAME##_begin ) );

#define PINCLUDE_MACRO(z, n, text) INCLUDE_TEST(pass##n);
#define PRUN_MACRO(z, n, text) BOOST_CHECK( parse( &pass##n##_begin ) );

#define NINCLUDE_MACRO(z, n, text) INCLUDE_TEST(fail##n);
#define NRUN_MACRO(z, n, text) BOOST_CHECK( !parse( &fail##n##_begin ) );


BOOST_PP_REPEAT_FROM_TO(1, 3, PINCLUDE_MACRO,);
BOOST_PP_REPEAT_FROM_TO(1, 34, NINCLUDE_MACRO,);

BOOST_AUTO_TEST_CASE ( json_checker ) { // http://json.org/JSON_checker/
	string_parser parse;
	BOOST_PP_REPEAT_FROM_TO(1, 3, PRUN_MACRO,);
	BOOST_PP_REPEAT_FROM_TO(1, 34, NRUN_MACRO,);
}

