#ifndef PASTELSYS_SCIENTIFIC_NOTATION_FWD_H
#define PASTELSYS_SCIENTIFIC_NOTATION_FWD_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! A number in scientific notation.
	/*! 
	This representation of a number is meant to act as
	an intermediate type to which all the native types
	can be converted to without loss. In particular,
	this representation is like the IEEE floating point
	formats with extended bit-counts.

	The value of the number is given by
	
		(-1)^s 2^e (1 + m / 2^64)

	where

		s is the sign of the number,
		e is the exponent of the number, and
		m is the mantissa of the number.
	*/
	struct ScientificNotation
	{
		//! The sign s of the number (+ = false, - = true)
		bool negative;

		//! The exponent e of the number.
		integer exponent;

		//! The mantissa m of the number.
		uint64 mantissa;
	};

}

#endif
