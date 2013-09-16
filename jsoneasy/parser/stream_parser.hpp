#ifndef JSONEASY_PARSER_STREAM_PARSER_HPP_
#define JSONEASY_PARSER_STREAM_PARSER_HPP_

#include <istream>
#include <memory>

namespace JsonEasy {
namespace Parser {
class Handler;

class StreamParser {
	// Hiding inner data
	std::shared_ptr<void> data;

	// noncopy
	StreamParser(const StreamParser&) = delete;
	StreamParser &operator=(const StreamParser&) = delete;
public:
	StreamParser();
	~StreamParser();
	bool parse(std::istream& input, std::unique_ptr<Handler>& parser );

	// Just the same as parse, but to be used as functior
	inline bool operator()(std::istream& input, std::unique_ptr<Handler>& parser ) {
		return parse(input, parser);
	}
};

} // namespace Parser
} // namespace JsonEasy


#endif /* JSONEASY_PARSER_STREAM_PARSER_HPP_ */
