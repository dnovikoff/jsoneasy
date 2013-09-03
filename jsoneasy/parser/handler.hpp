#ifndef JSONEASY_PARSER_HANDLER_HPP_
#define JSONEASY_PARSER_HANDLER_HPP_

#include <memory>
#include <string>

namespace JsonEasy {
namespace Parser   {
/**
 * Interface for your parser
 */
class Handler {
	// noncopy
	Handler(const Handler&);
	Handler& opeartor(const Handler&);
public:
	virtual ~Handler() = 0;
	typedef std::unique_ptr<Handler> Ptr;

	Handler() {}

	/**
	 * Parsing different types
	 * return false if it this is not the type expected
	 */
	// @{
	virtual bool operator()(int) = 0;
	virtual bool operator()(bool) = 0;
	virtual bool operator()(double) = 0;
	virtual bool operator()(std::string&) = 0;
	// Called when null passes
	virtual bool null() = 0;
	// Called on map key
	virtual bool key(std::string&) = 0;
	// @}

	// empty ptr means failure
	virtual Ptr object() = 0;
	// empty ptr means failure
	virtual Ptr array() = 0;

	/**
	 * Called when parsing for this object/array finished
	 * You can do some checks here, like checking size
	 * return false if the result does not match what you whant
	 */
	virtual bool onParsed() = 0;
};

/**
 * Base parser to be inherited from
 * All operations fails by default
 */
class BaseHandler: public Handler {
public:
	virtual ~BaseHandler() = 0;
	bool operator()(int)          override { return false; }
	bool operator()(bool)         override { return false; }
	bool operator()(double)       override { return false; }
	bool operator()(std::string&) override { return false; }

	bool null() override { return false; }

	bool key(std::string&) override { return false; }

	Ptr object() override { return Ptr(); }
	Ptr array()  override { return Ptr(); }

	// Note that onParsed will return true by default
	bool onParsed() override { return true; }
};

} // namespace Parser
} // namespace JsonEasy

#endif /* JSONEASY_PARSER_HANDLER_HPP_ */
