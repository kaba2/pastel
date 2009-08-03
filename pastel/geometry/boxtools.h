// Description: Tools for working with boxes
// Documentation: shapes.txt

#ifndef PASTEL_BOX_TOOLS_H
#define PASTEL_BOX_TOOLS_H

#include "pastel/geometry/box.h"
#include "pastel/math/affinetransformation.h"

namespace Pastel
{

	//! Transforma an box by the given transformation.
	/*!
	The matrix part of the transformation must be
	special orthogonal, otherwise the assumptions
	of the Box are broken down and some geometric
	algorithms work incorrectly.
	*/

	template <typename Real, int N>
	Box<Real, N>& operator*=(Box<Real, N>& box,
		const AffineTransformation<Real, N>& rigidTransform);

	//! Returns an box transformed by the given transformation.
	/*!
	The matrix part of the transformation must be
	special orthogonal, otherwise the assumptions
	of the Box are broken down and some geometric
	algorithms work incorrectly.
	*/

	template <typename Real, int N>
	Box<Real, N> operator*(const Box<Real, N>& box,
		const AffineTransformation<Real, N>& rigidTransform);

}

#include "pastel/geometry/box_tools.hpp"

#endif
