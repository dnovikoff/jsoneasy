#ifndef JSONEASY_PARSER_INTEGER_HPP_
#define JSONEASY_PARSER_INTEGER_HPP_

#include <stdint.h>
#include <type_traits>
#include <boost/utility/enable_if.hpp>
#include <boost/integer_traits.hpp>

namespace JsonEasy {
namespace Parser   {
/**
 * Special class to simplify work with intger and to hide its real type ( to be possible changed )
 */
class Integer {
	typedef int64_t DataType;
	DataType data;
public:
	explicit Integer(int64_t v):data(v) {}
	template<typename T>
	bool to(T& integral) const {
		static const DataType originalMax = boost::integer_traits<T>::const_max;

		// Does not accept top uint64 values
		static const DataType maxValue = (originalMax!=-1)?originalMax:boost::integer_traits<DataType>::const_max;
		static const DataType minValue = boost::integer_traits<T>::const_min;

		if( data < minValue || data > maxValue ) return false;
		integral = static_cast<T>( data );
		return true;
	}

	bool to(double& d) const {
		d = static_cast<double>( data );
		return true;
	}

	bool to(float& d) const {
		d = static_cast<float>( data );
		return true;
	}
};

} // namespace Parser
} // namespace JsonEasy


#endif /* JSONEASY_PARSER_INTEGER_HPP_ */
