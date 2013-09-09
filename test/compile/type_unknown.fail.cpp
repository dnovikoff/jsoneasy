#include <jsoneasy/template/create.hpp>

class Unknown {};

void compile() {
	Unknown u;
	JsonEasy::Template::createHandler( u );
}
