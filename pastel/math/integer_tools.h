/*!
\file
\brief Tools for working with integers.
*/

#ifndef PASTELMATH_INTEGER_TOOLS_H
#define PASTELMATH_INTEGER_TOOLS_H

namespace Pastel
{

	//! Returns the greatest common dividor of 'abs(left)' and 'abs(right)'.

	/*!
	Time complexity: (log2(left) + log2(right))^2
	*/

	template <typename Integer>
	Integer gcd(const Integer& left, const Integer& right);

}

#include "pastel/math/integer_tools.hpp"

#endif
