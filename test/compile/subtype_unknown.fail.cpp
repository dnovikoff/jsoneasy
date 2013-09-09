#include <jsoneasy/template/support/vector.hpp>
#include <jsoneasy/template/create.hpp>

class Unknown {};

void compile() {
	std::vector< Unknown > u;
	JsonEasy::Template::createHandler( u );
}
