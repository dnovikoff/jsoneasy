/**
 * No conversion included for string to int key
 */

#include <jsoneasy/template/support/map.hpp>
#include <jsoneasy/template/create.hpp>

void compile() {
	std::map<int, int> mp;
	JsonEasy::Template::createHandler( mp );
}
