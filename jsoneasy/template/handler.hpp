#ifndef JSONEASY_TEMPLATE_HANDLER_HPP_
#define JSONEASY_TEMPLATE_HANDLER_HPP_

#include <functional>

#include <jsoneasy/parser/handler.hpp>
#include <jsoneasy/template/container.hpp>

namespace JsonEasy {
namespace Template {

template<JsonContainerType JsonType, typename ValueType, typename ParentT> class Handler;

template<bool enable, JsonContainerType JsonType, typename VT, typename ParentAssist>
struct SubHandler {
	static Parser::Handler::Ptr create(ParentAssist&) {
		return Parser::Handler::Ptr();
	}
};

template<JsonContainerType JsonType, typename VT, typename ParentAssist>
struct SubHandler<true, JsonType, VT, ParentAssist> {
	static Parser::Handler::Ptr create(ParentAssist& assist) {
		Parser::Handler::Ptr p(new Handler< JsonType, VT, ParentAssist>(assist) );
		return p;
	}
};

template<JsonContainerType JsonType, typename HandlerT>
static Parser::Handler::Ptr createSubHandler(HandlerT& h) {
	typedef typename HandlerT::ContainerType::ValueType SubValueType;
	static const bool enabled = (JsonType==Container<JsonType, SubValueType>::type);
	typedef SubHandler<enabled , JsonType, SubValueType, typename HandlerT::AssistType> SubHandlerT;
	return SubHandlerT::create(h.assist);
}

/**
 * Assist will call different functions, depending on object type
 * This one is for JsonArray type
 */
template<bool isObject, typename ContainerT>
class InsertAssist {
	ContainerT& container;
public:
	explicit InsertAssist(ContainerT& c):container(c) {}
	bool key(std::string&) { return false; }

	template<typename T>
	bool insert(T& x) { return container.insert(x); }
};

/**
 * * This one is for JsonObject type
 */
template<typename ContainerT>
class InsertAssist<true, ContainerT> {
	ContainerT& container;
	std::string tmpKey;
public:
	explicit InsertAssist(ContainerT& c):container(c) {}
	bool key(std::string& k) {
		k.swap(tmpKey);
		return true;
	}

	template<typename T>
	bool insert(T& v) {
		return container.insert(tmpKey, v);
	}
};

template<JsonContainerType JsonType, typename VT, typename ParentAssist>
class Handler: public Parser::Handler {
public:
	typedef Container<JsonType, VT> ContainerType;
	typedef VT ValueType;
	static const bool isObject = (JsonType==JsonObject);
	typedef InsertAssist<isObject, ContainerType> AssistType;
private:
	ContainerType container;
	ParentAssist& parentAssist;
public:
	AssistType assist;

	explicit Handler(ParentAssist& p): parentAssist(p), assist(container) {}
	bool operator()(int x) override {
		return assist.insert(x);
	}
	bool operator()(bool x) override {
		return assist.insert(x);
	}
	bool operator()(double x) override {
		return assist.insert(x);
	}
	bool operator()(std::string& x) override {
		return assist.insert(x);
	}
	bool null() override {
		static NullTag nullTag; // Should be non const
		return assist.insert(nullTag);
	}
	bool key(std::string& k) override {
		return assist.key(k);
	}

	Ptr object() override {
		return createSubHandler< JsonObject >( *this );
	}

	Ptr array()  override {
		return createSubHandler< JsonArray >( *this );
	}

	bool onParsed() override {
		parentAssist.insert( container.data );
		return true;
	}
};

template<typename T>
class SwapContainer {
public:
	T& data;

	typedef T ValueType;

	explicit SwapContainer(T& d):data(d) {}

	bool insert(T& newData) {
		boost::swap( data, newData );
		return true;
	}
};

/**
 * Top object to start parsing with
 * Select first handler (for objects or arrays)
 */
template<typename T>
class StartHandler: public Parser::BaseHandler {
public:
	typedef SwapContainer<T> ContainerType;
	typedef T ValueType;
	typedef InsertAssist<false, ContainerType> AssistType;
private:
	ContainerType swapper;
public:
	AssistType assist;
	explicit StartHandler(T& d):swapper(d), assist(swapper) {}
	Ptr object() override {
		return createSubHandler< JsonObject >( *this );
	}

	Ptr array()  override {
		return createSubHandler< JsonArray >( *this );
	}
};

/**
 * Call this function to create handler for your container
 * to use with parser
 */
template<typename T>
Parser::Handler::Ptr createHandler(T& data) {
	Parser::Handler::Ptr p(new StartHandler<T>(data));
	return p;
}

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_HANDLER_HPP_ */
