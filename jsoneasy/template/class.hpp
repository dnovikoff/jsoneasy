#ifndef JSONEASY_TEMPLATE_CLASS_HPP_
#define JSONEASY_TEMPLATE_CLASS_HPP_

#include <string>

namespace JsonEasy {
namespace Template {

struct NotClass {};

template<typename T>
class Class: public NotClass {};

struct Example {
	int first;
	int second;
	std::string other;
};

class String {
	const char * const begin_;
	size_t size_;
public:
	template< size_t N >
	constexpr String( const char(&arr)[N] ) : begin_(arr), size_(N - 1) {
		static_assert( N >= 1, "not a string literal");
	}

	constexpr const char * const toString() const {
		return begin_;
	}

	constexpr size_t size() const {
		return size_;
	}
};

template<typename Type, typename FieldType>
class FieldInfo {
	const String name;
	FieldType Type::* field;
public:
	constexpr FieldInfo( const String n, FieldType Type::* f ) : name(n), field(f) {}
};

template<typename FieldType, typename Type, size_t N>
constexpr FieldInfo<Type, FieldType> createInfo(const char(&arr)[N], FieldType Type::* f) {
	return FieldInfo<Type, FieldType>( String(arr), f);
}

template<typename... Fields>
class ClassInfo {
public:
	constexpr ClassInfo() {}
};


#define JSONEASY_TEMPLATE_CLASS_INFO( X ) static auto info() -> decltype( X ) { return X; }

template<>
class Class<Example> {
public:
	JSONEASY_TEMPLATE_CLASS_INFO( createInfo("first", &Example::first));
};

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_CLASS_HPP_ */
