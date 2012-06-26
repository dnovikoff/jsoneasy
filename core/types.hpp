#ifndef _SPIRIT2JSON_TYPES_HPP_
#define _SPIRIT2JSON_TYPES_HPP_

#include <vector>
#include <map>
#include <string>
#include <boost/variant.hpp>

namespace client {
	struct null_type {};
	typedef boost::recursive_variant_ rvariant;
	typedef boost::recursive_wrapper<rvariant> rwrapper;
	typedef std::vector<rvariant> rvector_t;
	typedef std::map<std::string, rwrapper> rmap_t;

	typedef boost::make_recursive_variant<
		int,
		double,
		bool,
		null_type,
		std::string,
		rvector_t,
		rmap_t
	>::type value_type;

	typedef std::vector<value_type> vector_t;

	typedef std::map<std::string, boost::recursive_wrapper<value_type> > map_t;
	typedef std::pair<std::string, value_type> pair_t;
	
	template<typename I> class value_grammar;	
} // namespace client

#endif // _SPIRIT2JSON_TYPES_HPP_
