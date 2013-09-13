#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <jsoneasy/template/class.hpp>

struct Example {
	int first;
	int second;
	std::string other;
};

namespace JsonEasy {
namespace Template {

struct t1 { JE_META( Example, JE_FIELD(first) ) };
struct t2 { JE_META( Example, JE_FIELD(first)JE_FIELD(second) ) };
struct t3 { JE_META( Example, JE_FIELD(first)JE_FIELD(second)JE_FIELD(other) ) };

} // namespace Template
} // namespace JsonEasy

