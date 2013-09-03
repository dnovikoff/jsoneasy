#ifndef JSONEASY_PARSER_EXCEPTION_HPP_
#define JSONEASY_PARSER_EXCEPTION_HPP_

#include <stdexcept>
#include <boost/lexical_cast.hpp>

namespace JsonEasy {
namespace Parser   {

class Exception: public std::runtime_error {
public:
	explicit Exception(const std::string& m): std::runtime_error(m) {}
};

class UnexpectedException: public Exception {
public:
	explicit UnexpectedException(const std::string& objectName): Exception("Unexpected " + objectName) {}
	template<typename T>
	explicit UnexpectedException(const std::string& objectName, const T& value): Exception("Unexpected " + objectName + "(" + boost::lexical_cast<std::string>(value) + ")") {}
};

} // namespace Parser
} // namespace JsonEasy

#endif /* JSONEASY_PARSER_EXCEPTION_HPP_ */
