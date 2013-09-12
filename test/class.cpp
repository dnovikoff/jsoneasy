#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>

#include <boost/test/unit_test.hpp>

#include <jsoneasy/parser/string_parser.hpp>
#include <jsoneasy/template/create.hpp>
#include <jsoneasy/template/class.hpp>

#include "one_array.hpp"

namespace JsonEasy {
namespace Template {

struct Example {
	int first;
	int second;
	std::string other;
};

template<>
class Class<Example> {
public:
	JSONEASY_TEMPLATE_CLASS_INFO( ("first", &Example::first)("second", &Example::second) );
};

} // namespace Template
} // namespace JsonEasy

BOOST_AUTO_TEST_CASE ( simple ) {

}
