#include <string>
#include <utility>

#include <jsoneasy/template/support/pair.hpp>
#include <jsoneasy/template/support/optional.hpp>
#include <jsoneasy/template/create.hpp>

void compile() {
	typedef std::pair<int, boost::optional<double> > t1;
	t1 x;
	JsonEasy::Template::createHandler( x );
}

void compile1() {
	typedef std::pair<int, int> t1;
	t1 x;
	JsonEasy::Template::createHandler( x );
}
