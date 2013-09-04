#ifndef JSONEASY_TEMPLATE_HANDLER_HPP_
#define JSONEASY_TEMPLATE_HANDLER_HPP_

#include <functional>

#include <jsoneasy/parser/handler.hpp>
#include <jsoneasy/template/container.hpp>

namespace JsonEasy {
namespace Template {

template<typename T, typename ParentT> class Handler;

template<bool enable, JsonContainerType JsonType, typename ContainerT>
struct SubHandler {
	static Parser::Handler::Ptr create(ContainerT&) {
		return Parser::Handler::Ptr();
	}
};

template<JsonContainerType JsonType, typename ContainerT>
struct SubHandler<true, JsonType, ContainerT> {
	typedef typename ContainerT::value_type value_type;

	static Parser::Handler::Ptr create(ContainerT& x) {
		Parser::Handler::Ptr p(new Handler< Container<JsonType, value_type>, ContainerT>(x) );
		return p;
	}
};

template<JsonContainerType JsonType, typename ContainerT>
static Parser::Handler::Ptr createSubHandler(ContainerT& x) {
	typedef typename ContainerT::value_type value_type;
	static const bool enabled = (JsonType==Container<JsonType, value_type>::type);
	typedef SubHandler<enabled , JsonType, ContainerT> SubHandlerT;
	return SubHandlerT::create(x);
}

template<typename ContainerT, typename ParentT>
class Handler: public Parser::Handler {
	ContainerT container;
	ParentT& parent;
	typedef typename ContainerT::value_type value_type;
public:
	explicit Handler(ParentT& p): parent(p) {}
	bool operator()(int x) override {
		return container.insert(x);
	}
	bool operator()(bool x) override {
		return container.insert(x);
	}
	bool operator()(double x) override {
		return container.insert(x);
	}
	bool operator()(std::string& x) override {
		return container.insert(x);
	}
	bool null() override {
		static NullTag nullTag; // Should be non const
		return container.insert(nullTag);
	}
	bool key(std::string& k) override {
		return container.key(k);
	}

	Ptr object() override {
		return createSubHandler< JsonObject >( container );
	}

	Ptr array()  override {
		return createSubHandler< JsonArray >( container );
	}

	bool onParsed() override {
		parent.insert( container.data );
		return true;
	}
};

template<typename T>
class SwapContainer {
public:
	T& data;

	typedef T value_type;

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
	SwapContainer<T> swapper;
public:
	explicit StartHandler(T& d):swapper(d) {}
	Ptr object() override {
		return createSubHandler< JsonObject >( swapper );
	}

	Ptr array()  override {
		return createSubHandler< JsonArray >( swapper );
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
