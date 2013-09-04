#ifndef JSONEASY_TEMPLATE_HANDLER_HPP_
#define JSONEASY_TEMPLATE_HANDLER_HPP_

#include <jsoneasy/parser/handler.hpp>
#include <jsoneasy/template/container.hpp>

namespace JsonEasy {
namespace Template {

struct NullTag {};
static const NullTag nullTag;

template<typename T>
class Handler: public Parser::Handler {
	Container<T> nv;
public:
	explicit Handler(T& d): nv(d) {}
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

/**
 * Top object to start parsing with
 * Select first handler (for objects or arrays)
 */
template<typename T>
class StartHandler: public Parser::BaseHandler {
	T& data;
public:
	explicit StartHandler(T& d):data(d) {}
	Ptr object() override {
		// JsonAny will pass
		if( Container<T>::type == JsonArray ) return Ptr();
		Ptr p(new Template::Handler<T>(data));
		return p;
	}
	Ptr array()  override {
		// JsonAny will pass
		if( Container<T>::type == JsonObject ) return Ptr();
		Ptr p(new Template::Handler<T>(data));
		return p;
	}
};

/**
 * Call this function to create handler for your container
 * to use with parser
 */
template<typename T>
Parser::Handler::Ptr createHandler(T& data) {
	T empty;
	boost::swap( data, empty );
	Parser::Handler::Ptr p(new StartHandler<T>(data));
	return p;
}

} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_HANDLER_HPP_ */
