#include "pastel/ray/shape.h"

#include "pastel/geometry/bounding_alignedbox.h"

namespace Pastel
{

	Shape::Shape()
	{
	}

	Shape::~Shape()
	{
	}

	AlignedBox3 Shape::bound(
		const AffineTransformation3& transformation) const
	{
		// The default implementation returns the
		// bounding aligned box of the affinely transformed
		// local bound.

		return boundingAlignedBox(bound(), transformation);
	}

}
