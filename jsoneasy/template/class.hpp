#ifndef JSONEASY_TEMPLATE_CLASS_HPP_
#define JSONEASY_TEMPLATE_CLASS_HPP_

#include <string>
#include <type_traits>

#include <jsoneasy/template/any_type.hpp>

namespace JsonEasy {
namespace Template {

struct NotClass {};

template<typename T>
class Class: public NotClass {};

template<typename T>
struct IsClass {
	const static bool value = !std::is_base_of<NotClass, Class<T> >::value;
};

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

template<typename Type, typename FT>
class FieldInfo {
	typedef FieldInfo<Type, FT> self_t;
	const String name;
	FT Type::* field;
public:
	typedef FT FieldType;

	constexpr FieldInfo( const String n, FieldType Type::* f ) : name(n), field(f) {}
	constexpr const char * getName() const { return name.cString(); }
	constexpr auto getField() -> decltype(field) const { return field; }
};

template<typename FirstField, typename... OtherFields>
class ClassInfo;

template<typename FirstField>
class ClassInfo<FirstField> {
	const FirstField field;
	typedef ClassInfo<FirstField> self_t;
public:
	typedef AnyType<typename FirstField::FieldType> FieldTypes;

	constexpr explicit ClassInfo(const FirstField& ff):field(ff) {}

	template<typename Type, typename FieldType, size_t N>
	constexpr ClassInfo<FirstField, FieldInfo<Type, FieldType> > operator()( const char(&str)[N], FieldType Type::* f ) {
		return ClassInfo<FirstField, FieldInfo<Type, FieldType> >( FieldInfo<Type, FieldType>(String(str), f), *this );
	}

	/**
	 * Returns false if stopped
	 */
	template<typename T>
	bool visit(T& x) const {
		if( !x( field ) ) return false;
		return true;
	}
};

template<typename FirstField, typename... OtherFields>
class ClassInfo {
	typedef ClassInfo<FirstField> OneParent;
	typedef ClassInfo<OtherFields...> OtherParent;

	OneParent one;
	OtherParent other;
	typedef ClassInfo<FirstField, OtherFields...> self_t;

	typedef typename FirstField::FieldType FirstType;
	typedef typename OtherParent::FieldTypes OtherTypes;
public:
	typedef typename OtherTypes::template Add<FirstType>::type FieldTypes;
	constexpr ClassInfo(const FirstField& ff, const OtherParent& op):one(ff), other(op) {}

	template<typename Type, typename FieldType, size_t N>
	constexpr ClassInfo<FieldInfo<Type, FieldType>, FirstField, OtherFields...> operator()( const char(&str)[N], FieldType Type::* f ) {
		return ClassInfo<FieldInfo<Type, FieldType>, FirstField, OtherFields... >( FieldInfo<Type, FieldType>(String(str), f), *this );
	}

	/**
	 * Returns false if stopped
	 */
	template<typename T>
	bool visit(T& x) const {
		if( !one  .visit( x ) ) return false;
		if( !other.visit( x ) ) return false;
		return true;
	}
};

template<typename Type, typename FieldType, size_t N>
constexpr ClassInfo<FieldInfo<Type, FieldType> > createClass( const char(&str)[N], FieldType Type::* f ) {
	return ClassInfo<FieldInfo<Type, FieldType> >( FieldInfo<Type, FieldType>(String(str), f) );
}

/**
 * Only meta function
 */
#define JE_META_ONLY( X ) static constexpr const decltype( X ) metadata() { return X; }

/**
 * Declaration for meta function with define for ClassType (used by JE_FIELD macro)
 */
#define JE_META( NAME, X ) typedef NAME ClassType; JE_META_ONLY( createClass X )

/**
 * Helper function to omit multiple name repeat
 */
#define JE_FIELD( X ) ( #X, &ClassType::X )

/**
 * Specialization for class without namespace (if you have JsonEasy::Template around)
 */
#define JE_CLASS( NAME, X ) template<> class Class<NAME> { public: JE_META( NAME, X ) };

/**
 * Specialization for class within correct namespace
 */
#define JE_CLASS_NS( NAME, X ) namespace JsonEasy { namespace Template { JE_CLASS( NAME, X ) } }

// TODO: compiletime check for multiple filed usage

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_CLASS_HPP_ */
