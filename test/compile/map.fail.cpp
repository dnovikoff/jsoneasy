/**
 * No support/map.hpp header. Should fail - does not know how to deal with it
 */

#include <map>
#include <jsoneasy/template/create.hpp>

void compile() {
	std::map<std::string, int> mp;
	JsonEasy::Template::createHandler( mp );
}
