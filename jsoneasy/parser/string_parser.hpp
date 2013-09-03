#ifndef JSONEASY_PARSER_STRING_PARSER_HPP_
#define JSONEASY_PARSER_STRING_PARSER_HPP_

#include <string>
#include <memory>

namespace JsonEasy {
namespace Parser {
class Handler;

/**
 * This is an easy string-based parser
 * You can create your own parser based on stream of const char *
 * taking this one as an example
 *
 * The purpose of this parser is to hide spirit compilition to
 * one translation unit to save your time
 */
class StringParser {
	// Hiding inner data
	std::shared_ptr<void> data;

	// noncopy
	StringParser(const StringParser&) = delete;
	StringParser &operator=(const StringParser&) = delete;
public:
	StringParser();
	~StringParser();
	bool parse(const std::string& input, std::unique_ptr<Handler>& parser );

	// Just the same as parse, but to be used as functior
	inline bool operator()(const std::string& input, std::unique_ptr<Handler>& parser ) {
		return parse(input, parser);
	}
};

} // namespace Parser
} // namespace JsonEasy

#endif // JSONEASY_PARSER_STRING_PARSER_HPP_
