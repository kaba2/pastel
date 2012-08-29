// Description: Geometric optics
// Documentation: pastelray.txt

#ifndef PASTELRAY_BEAM_TOOLS_H
#define PASTELRAY_BEAM_TOOLS_H

#include "pastel/ray/raylibrary.h"
#include "pastel/ray/beam.h"
#include "pastel/ray/localgeometry.h"

#include "pastel/math/affinetransformation.h"

namespace Pastel
{

	//! Transforms a beam with an affine transformation.
	PASTELRAY Beam transform(
		const Beam& beam, 
		const AffineTransformation3& transformation);

	//! Translates a beam to a surface.
	/*!
	beam:
	The beam to translate.
	
	surface:
	The local geometry in the intersection
	of the beam and some surface.

	t:
	The parametric distance which brings
	the beam on contact with the surface.
	*/
	PASTELRAY Beam transferToSurface(
		const Beam& beam,
		const LocalGeometry& surface,
		real t);

	//! Reflects a beam from a surface.
	/*!
	beam:
	The beam to reflect.

	surface:
	The local geometry on the surface
	which the beam is currently on contact with.
	*/
	PASTELRAY Beam reflectFromSurface(
		const Beam& beam, 
		const LocalGeometry& surface);

}

#endif
