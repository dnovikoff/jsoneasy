/**
 * No conversion included for string to int key
 */

#include <jsoneasy/template/support/map.hpp>
#include <jsoneasy/template/create.hpp>

namespace JsonEasy {
namespace Template {

template<>
struct Key<int> {
	static bool convert(std::string&, int&);
};

} // namespace Template
} // namespace JsonEasy

void compile() {
	std::map<int, int> mp;
	JsonEasy::Template::createHandler( mp );
}
