#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#include <iostream>

#include <boost/test/unit_test.hpp>
#include <boost/typeof/typeof.hpp>

#include <jsoneasy/parser/string_parser.hpp>
#include <jsoneasy/template.hpp>

template<typename T>
static bool parseTo(const std::string& input, T& data) {
	JsonEasy::Parser::Handler::Ptr h = JsonEasy::Template::createHandler(data);
	JsonEasy::Parser::StringParser sp;
	if( !sp.parse(input, h) ) return false;
	return true;
}

template<typename T>
class OneArray {
public:
	T value;
	T& operator*() { return value; }
	T* operator->() { return &value; }
	// for variant fast get

	template<typename Y>
	Y& get() {
		return boost::get<Y>( value );
	}
};

namespace JsonEasy {
namespace Template {

/**
 * Helper class for easy testing
 * Still is shows how to write specialization for your own container
 * This container allows arrays of only one value
 */
template<typename T>
class Container<JsonArray, OneArray<T> > {
	typedef OneArray<T> OneType;
	bool inserted;
public:
	Container():inserted(false) {}

	OneType data;
	typedef T ValueType;

	bool insert(ValueType& x) {
		if(inserted) return false;
		inserted = true;
		data.value = std::move(x);
		return true;
	}

	bool validate() { return inserted; }
};

} // namespace Template
} // namespace JsonEasy

BOOST_AUTO_TEST_CASE ( oneTest ) {
	OneArray<int> x;
	BOOST_CHECK( !parseTo("[]", x) );
	BOOST_CHECK( !parseTo(R"(["hello"])", x) );
	BOOST_CHECK( !parseTo("[6,8]", x) );
	BOOST_CHECK( parseTo("[517]", x) );
	BOOST_CHECK_EQUAL( x.value, 517);
	BOOST_CHECK( parseTo(R"([916])", x) );
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
 * This will acept only string as input
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
	BOOST_CHECK( parseTo(R"([""])", x) );
	BOOST_CHECK_EQUAL( x->value, "");

	BOOST_CHECK( parseTo(R"(["a"])", x) );
	BOOST_CHECK_EQUAL( x->value, "a");

	BOOST_CHECK( parseTo(R"(["abc"])", x) );
	BOOST_CHECK_EQUAL( x->value, "abc");

	// More then 3 chars
	BOOST_CHECK( !parseTo(R"(["abcd"])", x) );
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
	BOOST_CHECK( parseTo(R"([""])", x) );
	BOOST_CHECK_EQUAL( x.get<S3>(), "");

	BOOST_CHECK( parseTo(R"(["abc"])", x) );
	BOOST_CHECK_EQUAL( x.get<S3>(), "abc");

	BOOST_CHECK( parseTo(R"(["abcd"])", x) );
	BOOST_CHECK_EQUAL( x.get<S9>(), "abcd");

	BOOST_CHECK( parseTo(R"(["123456789"])", x) );
	BOOST_CHECK_EQUAL( x.get<S9>(), "123456789");

	BOOST_CHECK( parseTo(R"(["1234567890"])", x) );
	BOOST_CHECK_EQUAL( x.get<std::string>(), "1234567890");

	BOOST_CHECK( parseTo( "[666]", x) );
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
	BOOST_CHECK( parseTo(R"([""])", x) );
	BOOST_CHECK_EQUAL( x.get<std::string>(), "");

	BOOST_CHECK( parseTo(R"(["abc"])", x) );
	BOOST_CHECK_EQUAL( x.get<std::string>(), "abc");

	BOOST_CHECK( parseTo(R"(["abcd"])", x) );
	BOOST_CHECK_EQUAL( x.get<std::string>(), "abcd");

	BOOST_CHECK( parseTo(R"(["123456789"])", x) );
	BOOST_CHECK_EQUAL( x.get<std::string>(), "123456789");

	BOOST_CHECK( parseTo(R"(["1234567890"])", x) );
	BOOST_CHECK_EQUAL( x.get<std::string>(), "1234567890");
}
