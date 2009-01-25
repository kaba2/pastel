#ifndef PASTELSYS_CONSTANTS_H
#define PASTELSYS_CONSTANTS_H

#include "pastel/sys/mytypes.h"

#include <string>

namespace Pastel
{

	//! Returns an infinity.

	template <typename Real>
	inline Real infinity();

	//! Returns a Not-a-Number.

	template <typename Real>
	inline Real nan();

	//! Converts a text string to number.

	// Too big for inlining.

	template <typename Real>
	Real constant(const std::string& number);

	//! Returns pi constant.

	template <typename Real>
	inline Real constantPi();

	//! Returns neper constant.

	template <typename Real>
	inline Real constantNeper();

	//! Returns ln(2) constant.

	template <typename Real>
	inline Real constantLn2();

}

#include "pastel/sys/constants.hpp"

#endif
