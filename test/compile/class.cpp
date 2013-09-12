#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <jsoneasy/template/class.hpp>

namespace JsonEasy {
namespace Template {

struct Example {
	int first;
	int second;
	std::string other;
};

struct t1 { JSONEASY_TEMPLATE_CLASS_INFO( ("first", &Example::first) ) };
struct t2 { JSONEASY_TEMPLATE_CLASS_INFO( ("first", &Example::first)("second", &Example::second) ) };
struct t3 { JSONEASY_TEMPLATE_CLASS_INFO( ("first", &Example::first)("second", &Example::second)("other", &Example::other) ) };

} // namespace Template
} // namespace JsonEasy

