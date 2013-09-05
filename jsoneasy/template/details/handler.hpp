#ifndef JSONEASY_TEMPLATE_DETAILS_HANDLER_HPP_
#define JSONEASY_TEMPLATE_DETAILS_HANDLER_HPP_

#include <jsoneasy/parser/handler.hpp>
#include <jsoneasy/template/container.hpp>
#include <jsoneasy/template/details/container_assist.hpp>
#include <jsoneasy/template/details/sub_handler.hpp>

namespace JsonEasy {
namespace Template {
namespace Details  {

template<JsonContainerType JsonType, typename VT, typename ParentAssist>
class Handler: public Parser::Handler {
public:
	typedef Container<JsonType, VT> ContainerType;
	typedef VT ValueType;
	static const bool isObject = (JsonType==JsonObject);
	typedef ContainerAssist<isObject, ContainerType> AssistType;
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

} // namespace Details
} // namespace Template
} // namespace JsonEasy

#endif /* JSONEASY_TEMPLATE_DETAILS_HANDLER_HPP_ */
