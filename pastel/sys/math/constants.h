// Description: Mathematical constants
// Documentation: numbers.txt

#ifndef PASTELSYS_CONSTANTS_H
#define PASTELSYS_CONSTANTS_H

#include "pastel/sys/mytypes.h"

#include <string>

namespace Pastel
{

	//! Converts a text string to number.
	template <
		typename Real,
		Requires<std::is_floating_point<Real>> = 0>
	Real stringAsReal(const std::string& number);

	//! Returns pi constant.
	template <
		typename Real,
		Requires<std::is_floating_point<Real>> = 0>
	Real constantPi();

	//! Returns neper constant.
	template <
		typename Real,
		Requires<std::is_floating_point<Real>> = 0>
	Real constantNeper();

	//! Returns ln(2) constant.
	template <
		typename Real,
		Requires<std::is_floating_point<Real>> = 0>
	Real constantLn2();

	//! Returns Euler-Mascheroni constant.
	template <
		typename Real,
		Requires<std::is_floating_point<Real>> = 0>
	Real constantEulerMascheroni();

}

#include "pastel/sys/math/constants.hpp"

#endif
