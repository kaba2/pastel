/*!
\file
\brief Miscellaneous commonly needed tools.
*/

#ifndef PASTEL_SYSCOMMON_H
#define PASTEL_SYSCOMMON_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/syslibrary.h"
#include "pastel/sys/point.h"

#include <vector>
#include <string>

#include <boost/utility/enable_if.hpp>

namespace Pastel
{

	//! Tests two variables for equivalence.
	/*!
	'left' and 'right' are equivalent under
	a comparison 'compare' if it holds that
	!compare(left, right) && !compare(right, left).
	*/

	template <typename Type, typename Compare>
	bool equivalent(const Type& left, const Type& right,
		const Compare& compare);

	//! Tests two variables for equivalence.
	/*!
	This is a convenience function that uses
	std::less<Type> as the comparison functor
	for the more general equivalent() function.
	See the documentation for that.
	*/
	template <typename Type>
	bool equivalent(const Type& left, const Type& right);

	/*!
	Example:
	Given a 2d horizontal line segment [xMin, xMax[ on row y,
	xMin, xMax real numbers, y an integer.
	Which pixels should be set? Answer:
	[toPixelSpanPoint(xMin), toPixelSpanPoint(xMax)[
	on row y.
	*/
	integer toPixelSpanPoint(real t);

	template <int N>
	TemporaryPoint<N, integer> toPixelSpanPoint(
		const Point<N, real>& that);

	//! Scales an integer from [0, 2^FromBits - 1] to [0, 2^ToBits - 1].
	/*!
	Preconditions:
	FromBits > 0
	ToBits > 0
	ToBits <= 4 * FromBits

	The last requirement can be extended by implementing
	new functions for the larger ranges.

	The computation that is done is effectively:
	round[(number * (2^ToBits - 1)) / (2^(FromBits) - 1)]
	This computation would be a bit expensive if
	evaluated as it is. However, it turns out
	that you can compute this quantity very efficiently
	by using integer shifts and additions alone.
	*/

	template <int FromBits, int ToBits, typename Integer>
	typename boost::enable_if_c<(ToBits < FromBits), Integer>::type
		scaleInteger(const Integer& number);

	template <int FromBits, int ToBits, typename Integer>
	typename boost::enable_if_c<(ToBits == FromBits), Integer>::type
		scaleInteger(const Integer& number);

	template <int FromBits, int ToBits, typename Integer>
	typename boost::enable_if_c<(ToBits > FromBits && ToBits <= 2 * FromBits), Integer>::type
		scaleInteger(const Integer& number);

	template <int FromBits, int ToBits, typename Integer>
	typename boost::enable_if_c<(ToBits > 2 * FromBits && ToBits <= 3 * FromBits), Integer>::type
		scaleInteger(const Integer& number);

	template <int FromBits, int ToBits, typename Integer>
	typename boost::enable_if_c<(ToBits > 3 * FromBits && ToBits <= 4 * FromBits), Integer>::type
		scaleInteger(const Integer& number);

	//! Returns the bits at range [FromBit, FromBit + Bits[.
	/*!
	Preconditions:
	FromBit > 0
	Bits > 0
	Bits <= 32

	The computation is defined by:
	(number >> FromBit) & (1 << Bits - 1);
	*/

	template <int FromBit, int Bits, typename Integer>
	Integer extractBits(const Integer& number);

}

#include "pastel/sys/syscommon.hpp"

#endif
