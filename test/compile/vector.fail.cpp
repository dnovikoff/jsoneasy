/**
 * No support/vector.hpp header. Should fail - does not know how to deal with it
 */

#include <vector>
#include <jsoneasy/template/create.hpp>

void compile() {
	std::vector<int> mp;
	JsonEasy::Template::createHandler( mp );
}
