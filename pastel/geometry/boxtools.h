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

	template <int N, typename Real>
	Box<N, Real>& operator*=(Box<N, Real>& box,
		const AffineTransformation<N, Real>& rigidTransform);

	//! Returns an box transformed by the given transformation.
	/*!
	The matrix part of the transformation must be
	special orthogonal, otherwise the assumptions
	of the Box are broken down and some geometric
	algorithms work incorrectly.
	*/

	template <int N, typename Real>
	Box<N, Real> operator*(const Box<N, Real>& box,
		const AffineTransformation<N, Real>& rigidTransform);

}

#include "pastel/geometry/box_tools.hpp"

#endif
