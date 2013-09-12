#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/variant/get.hpp>

#include <jsoneasy/parser/string_parser.hpp>
#include <jsoneasy/template/create.hpp>

#include <test/one_array.hpp>
#include <test/parse_string.hpp>

BOOST_AUTO_TEST_CASE ( oneTest ) {
	OneArray<int> x;
	BOOST_CHECK( !parseString("[]", x) );
	BOOST_CHECK( !parseString(R"(["hello"])", x) );
	BOOST_CHECK( !parseString("[6,8]", x) );
	BOOST_CHECK( parseString("[517]", x) );
	BOOST_CHECK_EQUAL( x.value, 517);
	BOOST_CHECK( parseString(R"([916])", x) );
	BOOST_CHECK_EQUAL( x.value, 916);
}

/**
 * This class will have a runtime check and will accept only strings of maximum N chars
 * This is just to check runtime type validation
 */
template<size_t MaxChars>
class StringClass {
public:
	static const size_t maxChars = MaxChars;
	std::string value;

	/**
	 * This is for BOOST_CHECK_EQUAL to work
	 */
	bool operator==(const std::string& str) const {
		return value == str;
	}
};

/**
 * This is for BOOST_CHECK_EQUAL to work
 */
template<size_t MaxChars>
std::ostream& operator<<(std::ostream& out, const StringClass<MaxChars>& s) {
	return out << s;
}

namespace JsonEasy {
namespace Template {

/**
 * Custom converter for our class
 * This will accept only string as input
 */
template<size_t MaxChars>
struct Type<std::string, StringClass<MaxChars> >{
	static bool jsonToUser(std::string& j, StringClass<MaxChars>& u) {
		if(j.size() > MaxChars) return false;
		u.value = std::move(j);
		return true;
	}
};

} // namespace Template
} // namespace JsonEasy

BOOST_AUTO_TEST_CASE ( stringClassTest ) {
	OneArray<StringClass<3> > x;
	BOOST_CHECK( parseString(R"([""])", x) );
	BOOST_CHECK_EQUAL( *x, "");

	BOOST_CHECK( parseString(R"(["a"])", x) );
	BOOST_CHECK_EQUAL( *x, "a");

	BOOST_CHECK( parseString(R"(["abc"])", x) );
	BOOST_CHECK_EQUAL( *x, "abc");

	// More then 3 chars
	BOOST_CHECK( !parseString(R"(["abcd"])", x) );
}

#include <jsoneasy/template/support/variant.hpp>

/**
 * Variant could try to apply value to several types if failed in runtimer
 */
BOOST_AUTO_TEST_CASE ( variantRuntimeSelectTest ) {
	/**
	 * up to 3 chars should pass to first type
	 * up to 9 to second
	 * others - to last
	 */
	typedef StringClass<3> S3;
	typedef StringClass<9> S9;
	typedef std::string S;
	typedef boost::variant<S3, S9, int, S> VariantType;

	OneArray<VariantType> x;
	BOOST_CHECK( parseString(R"([""])", x) );
	BOOST_CHECK_EQUAL( x.get<S3>(), "");

	BOOST_CHECK( parseString(R"(["abc"])", x) );
	BOOST_CHECK_EQUAL( x.get<S3>(), "abc");

	BOOST_CHECK( parseString(R"(["abcd"])", x) );
	BOOST_CHECK_EQUAL( x.get<S9>(), "abcd");

	BOOST_CHECK( parseString(R"(["123456789"])", x) );
	BOOST_CHECK_EQUAL( x.get<S9>(), "123456789");

	BOOST_CHECK( parseString(R"(["1234567890"])", x) );
	BOOST_CHECK_EQUAL( x.get<std::string>(), "1234567890");

	BOOST_CHECK( parseString( "[666]", x) );
	BOOST_CHECK_EQUAL( x.get<int>(), 666);
}

// If variant values will be lested in reverse order - this will not work
BOOST_AUTO_TEST_CASE ( variantRuntimeSelectTest2 ) {
	typedef StringClass<3> S3;
	typedef StringClass<9> S9;
	typedef std::string S;
	// S3 and S9( will be unreachable this way
	typedef boost::variant<S, S3, S9> VariantType;

	OneArray<VariantType> x;
	BOOST_CHECK( parseString(R"([""])", x) );
	BOOST_CHECK_EQUAL( x.get<std::string>(), "");

	BOOST_CHECK( parseString(R"(["abc"])", x) );
	BOOST_CHECK_EQUAL( x.get<std::string>(), "abc");

	BOOST_CHECK( parseString(R"(["abcd"])", x) );
	BOOST_CHECK_EQUAL( x.get<std::string>(), "abcd");

	BOOST_CHECK( parseString(R"(["123456789"])", x) );
	BOOST_CHECK_EQUAL( x.get<std::string>(), "123456789");

	BOOST_CHECK( parseString(R"(["1234567890"])", x) );
	BOOST_CHECK_EQUAL( x.get<std::string>(), "1234567890");
}

struct CustomKey {
	size_t value;

	// with implicit conversion from size_t
	CustomKey(size_t v):value(v) {}
	CustomKey():value(0) {}

	// Should be comparable to be stored in map
	bool operator<(const CustomKey& other) const {
		return value < other.value;
	}
};

namespace JsonEasy {
namespace Template {

// This key will convert string to its size. Just to show abilities
template<>
struct Key<CustomKey> {
	static bool convert(std::string& parsed, CustomKey& user) {
		user.value = parsed.size();
		return true;
	}
};

} // namespace Template
} // namespace JsonEasy

#include <jsoneasy/template/support/map.hpp>

// If variant values will be lested in reverse order - this will not work
BOOST_AUTO_TEST_CASE ( customKeyTest ) {
	std::map<CustomKey, int> x;
	BOOST_CHECK( parseString(R"({})", x) );
	BOOST_REQUIRE( x.empty() );
	BOOST_CHECK( parseString(R"({"":894, "abc":0, "01234567890": 11222 })", x) );
	BOOST_REQUIRE_EQUAL( x.size(), 3u);
	BOOST_CHECK_EQUAL( x.at(0u), 894);
	BOOST_CHECK_EQUAL( x.at(3u), 0);
	BOOST_CHECK_EQUAL( x.at(11u), 11222);
}

// check that movable, non-copyable types parsers compiles
// add set of compilation tests
