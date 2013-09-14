#ifndef JSONEASY_TEMPLATE_CLASS_CONTAINER_HPP_
#define JSONEASY_TEMPLATE_CLASS_CONTAINER_HPP_

#include <map>
#include <string>

#include <jsoneasy/template/class.hpp>
#include <jsoneasy/template/container.hpp>

namespace JsonEasy {
namespace Template {

struct MetadataFillVisitor {
	size_t counter;
	typedef std::map<std::string, size_t> map_t;
	map_t& meta;
public:
	explicit MetadataFillVisitor(map_t& m):counter(0u), meta(m) {}

	template<typename T>
	bool operator()(T& field) {
		meta[field.getName()] = counter++;
		return true;
	}
};

template<typename DataType, typename JsonType>
struct FieldSetVisitor {
	DataType& data;
	JsonType& jsonData;
	size_t index;
	bool result;
public:
	FieldSetVisitor(DataType& d, JsonType& jd,  size_t i):data(d), jsonData(jd), index(i), result(false) {}

	template<typename T>
	bool operator()(T& field) {
		if( index > 0 ) {
			--index;
			return true;
		}
		result = jsonToUser(jsonData, (data.*field.getField()) );
		return false; // Index found - stop
	}

	bool getResult() const { return result; }
};

template<typename DataType, typename CreatorType>
struct CreateFieldVisitor {
	CreatorType& creator;
	size_t index;
	bool result;
public:
	CreateFieldVisitor(CreatorType& c, size_t i):creator(c), index(i), result(false) {}

	template<typename T>
	bool operator()(T&) {
		if( index > 0 ) {
			--index;
			return true;
		}
		result = creator.template create<typename T::FieldType>();
		return false; // Index found - stop
	}

	bool getResult() const { return result; }
};

template<typename ClassType>
class ClassContainer {
	typedef Class<ClassType> MetaClass;
	typedef decltype(MetaClass::metadata()) MetaDataType;
	static constexpr MetaDataType metadata() { return MetaClass::metadata(); }

	static std::map<std::string, size_t> fillNames() {
		std::map<std::string, size_t> tmp;
		MetadataFillVisitor v(tmp);
		metadata().visit( v );
		return tmp;
	}
	// key to its index
	static const std::map<std::string, size_t> names;
	size_t fieldCounter;
public:
	ClassContainer():fieldCounter(0) {}

	ClassType data;

	typedef typename MetaDataType::FieldTypes ValueType;
	typedef std::string KeyType;

	template<typename T>
	bool insert(KeyType& key, T& val) {
		auto i = names.find(key);
		if( i == names.end() ) return false;
		const size_t index = i->second;
		FieldSetVisitor<ClassType, T> v( data, val, index );
		metadata().visit( v );
		++fieldCounter;
		return v.getResult();
	}

	template<typename C>
	bool create(const KeyType& key, C& c) {
		auto i = names.find(key);
		if( i == names.end() ) return false;
		const size_t index = i->second;
		CreateFieldVisitor<ClassType, C> v( c, index );
		metadata().visit( v );
		return v.getResult();
	}

	// Number of fields should match exactly
	bool validate() const { return fieldCounter == names.size(); }
};

template<typename ClassType>
const std::map<std::string, size_t> ClassContainer<ClassType>::names = fillNames();

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_CLASS_CONTAINER_HPP_ */
