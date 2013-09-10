#include <string>
#include <utility>

#include <jsoneasy/template/support/pair.hpp>
#include <jsoneasy/template/support/optional.hpp>
#include <jsoneasy/template/support/vector.hpp>
#include <jsoneasy/template/create.hpp>

void compile() {
	typedef std::pair<std::string, boost::optional<double> > t1;
	typedef std::pair<int, std::vector<int> > t2;
	typedef std::pair<t1, t2> t3;

	t3 x;
	JsonEasy::Template::createHandler( x );
}
