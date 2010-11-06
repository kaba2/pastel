// Description: Native integer type modeling the Integer concept.
// Documentation: integer.txt

#ifndef PASTEL_INTEGER_INTEGER_H
#define PASTEL_INTEGER_INTEGER_H

#include "pastel/sys/integer_concept.h"

namespace Pastel
{

	//! Returns 'that' == 0.
	bool zero(integer that);

	//! Returns 'that' < 0.
	bool negative(integer that);

	//! Returns 'that' > 0.
	bool positive(integer that);

	//! Returns numeric_limits<integer>::max().
	/*!
	This function approximates the Real concept.
	See the discussion in 'integer_integer.txt'.
	*/
	integer infinity(integer *);

}

#include "pastel/sys/integer_integer.hpp"

#endif
