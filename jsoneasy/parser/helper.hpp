#ifndef JSONEASY_PARSER_HELPER_HPP_
#define JSONEASY_PARSER_HELPER_HPP_

#include <string>

#include <set>
#include <list>
#include <map>
#include <stack>

#include <jsoneasy/parser/handler.hpp>

namespace JsonEasy {
namespace Parser   {

struct NullTag {};

template<typename T>
class TemplateHandler;

template<typename ParsedType, typename UserType>
struct ConvertType {
	static bool apply(ParsedType&, UserType&) { return false; }
};

template<typename T>
struct ConvertType<T,T> {
	static bool apply(T& parsed, T& user) {
		user = parsed;
		return true;
	}
};

template<typename ParsedType, typename UserType>
static bool ConvertHelper(ParsedType& pt, UserType& ut) {
	return ConvertType<ParsedType, UserType>::apply( pt, ut );
}

enum ValueType { ObjectType, ArrayType };

template<typename T>
class NextValue {
	T& data;
public:
	static const ValueType type = ArrayType;

	explicit NextValue(T& d):data(d) {}

	template<typename X>
	bool insert(X& x) {
		typename T::value_type tmp;
		if(!ConvertHelper(x,tmp)) return false;
		data.push_back( std::move(tmp) );
		return true;
	}
	bool key(std::string&) { return false; }
};

template<typename T, typename C, typename A>
class NextValue<std::set<T,C,A > > {
	typedef std::set<T,C,A> container_t;
	container_t& data;
public:
	static const ValueType type = ArrayType;
	explicit NextValue(container_t& d):data(d) {}

	template<typename X>
	bool insert(X& x) {
		T tmp;
		if(!ConvertHelper(x,tmp)) return false;
		return data.insert( std::move(tmp) ).second;
	}
	bool key(std::string&) { return false; }
};

template<typename T, typename C, typename A>
class NextValue<std::multiset<T,C,A > > {
	typedef std::multiset<T,C,A> container_t;
	container_t& data;
public:
	static const ValueType type = ArrayType;
	explicit NextValue(container_t& d):data(d) {}

	template<typename X>
	bool insert(X& x) {
		T tmp;
		if(!ConvertHelper(x,tmp)) return false;
		data.insert( std::move(tmp) );
		return true;
	}
	bool key(std::string&) { return false; }
};

template<typename T, typename D>
class NextValue<std::stack<T,D> > {
	typedef std::stack<T,D> container_t;
	container_t& data;
public:
	static const ValueType type = ArrayType;
	explicit NextValue(container_t& d):data(d) {}

	template<typename X>
	bool insert(X& x) {
		T tmp;
		if(!ConvertHelper(x,tmp)) return false;
		data.push( std::move(tmp) );
		return true;
	}
	bool key(std::string&) { return false; }
};

template<typename K,typename V, typename C, typename A>
class NextValue<std::map<K,V,C,A> > {
	typedef std::map<K,V,C,A> container_t;
	container_t& data;
	std::string keyS;
public:
	static const ValueType type = ObjectType;
	explicit NextValue(container_t& d):data(d) {}

	template<typename X>
	bool insert(X& x) {
		V tmp;
		if(!ConvertHelper(x,tmp)) return false;
		auto p = std::make_pair(std::move(keyS), std::move(tmp));
		return data.insert(std::move(p)).second;
	}
	bool key(std::string& k) {
		keyS.swap(k);
		return true;
	}
};

static const NullTag nullTag;

template<typename T>
class TemplateHandler: public Handler {
	T& data;
	NextValue<T> nv;
public:
	explicit TemplateHandler(T& d):data(d), nv(d) {}
	bool operator()(int x) override {
		return nv.insert(x);
	}
	bool operator()(bool x) override {
		return nv.insert(x);
	}
	bool operator()(double x) override {
		return nv.insert(x);
	}
	bool operator()(std::string& x) override {
		return nv.insert(x);
	}
	bool null() override {
		return nv.insert(nullTag);
	}
	bool key(std::string& k) override {
		return nv.key(k);
	}

	Ptr object() override { return Ptr(); }
	Ptr array()  override { return Ptr(); }

	// Note that onParsed will return true by default
	bool onParsed() override { return true; }
};

template<typename T>
class StartParser: public BaseHandler {
	T& data;
public:
	explicit StartParser(T& d):data(d) {}
	Ptr object() override {
		if( NextValue<T>::type != ObjectType ) return Ptr();
		Ptr p(new TemplateHandler<T>(data));
		return p;
	}
	Ptr array()  override {
		if( NextValue<T>::type != ArrayType ) return Ptr();
		Ptr p(new TemplateHandler<T>(data));
		return p;
	}
};

template<typename T>
void parseTo(const std::string& data, T& container) {
	StringParser sp;
	T tmp;
	Handler::Ptr p(new StartParser<T>(tmp));
	sp(data, p);
	boost::swap(tmp, container);
}

} // namespace Parser
} // namespace JsonEasy

#endif /* JSONEASY_PARSER_HELPER_HPP_ */
