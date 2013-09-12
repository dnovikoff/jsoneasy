#ifndef JSONEASY_TEMPLATE_CLASS_HPP_
#define JSONEASY_TEMPLATE_CLASS_HPP_

#include <string>

namespace JsonEasy {
namespace Template {

struct NotClass {};

template<typename T>
class Class: public NotClass {};

class String {
	const char * const begin_;
	size_t size_;
public:
	template< size_t N >
	constexpr String( const char(&arr)[N] ) : begin_(arr), size_(N - 1) {
		static_assert( N >= 1, "not a string literal");
	}

	constexpr const char * cString() const {
		return begin_;
	}

	constexpr size_t size() const {
		return size_;
	}
};

template<typename Type, typename FieldType>
class FieldInfo {
	typedef FieldInfo<Type, FieldType> self_t;
	const String name;
	FieldType Type::* field;
public:
	constexpr FieldInfo( const String n, FieldType Type::* f ) : name(n), field(f) {}
};

template<typename FirstField, typename... OtherFields>
class ClassInfo;

template<typename FirstField>
class ClassInfo<FirstField> {
	const FirstField field;
	typedef ClassInfo<FirstField> self_t;
public:
	constexpr explicit ClassInfo(const FirstField& ff):field(ff) {}

	template<typename Type, typename FieldType, size_t N>
	constexpr ClassInfo<FirstField, FieldInfo<Type, FieldType> > operator()( const char(&str)[N], FieldType Type::* f ) {
		return ClassInfo<FirstField, FieldInfo<Type, FieldType> >( FieldInfo<Type, FieldType>(String(str), f), *this );
	}
};

template<typename FirstField, typename... OtherFields>
class ClassInfo {
	typedef ClassInfo<FirstField> OneParent;
	typedef ClassInfo<OtherFields...> OtherParent;

	OneParent one;
	OtherParent other;
	typedef ClassInfo<FirstField, OtherFields...> self_t;
public:
	constexpr ClassInfo(const FirstField& ff, const OtherParent& op):one(ff), other(op) {}

	template<typename Type, typename FieldType, size_t N>
	constexpr ClassInfo<FieldInfo<Type, FieldType>, FirstField, OtherFields...> operator()( const char(&str)[N], FieldType Type::* f ) {
		return ClassInfo<FieldInfo<Type, FieldType>, FirstField, OtherFields... >( FieldInfo<Type, FieldType>(String(str), f), *this );
	}
};

template<typename Type, typename FieldType, size_t N>
constexpr ClassInfo<FieldInfo<Type, FieldType> > createClass( const char(&str)[N], FieldType Type::* f ) {
	return ClassInfo<FieldInfo<Type, FieldType> >( FieldInfo<Type, FieldType>(String(str), f) );
}

#define JSONEASY_TEMPLATE_CLASS_INFO1( X ) static auto metadata() -> decltype( X ) { return X; }
#define JE_META( NAME, X ) typedef NAME ClassType; JSONEASY_TEMPLATE_CLASS_INFO1( createClass X )
#define JE_FIELD( X ) ( #X, &ClassType::X )
#define JE_CLASS( NAME, X ) template<> class Class<NAME> { public: JE_META( NAME, X ) };
#define JE_CLASS_NS( NAME, X ) namespace JsonEasy { namespace Template { JE_CLASS( NAME, X ) } }

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_CLASS_HPP_ */
