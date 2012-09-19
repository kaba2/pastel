// Description: Template instantiation of shapes for testing
// Documentation: test_pastelmath.txt

#include "pastel/sys/mytypes.h"

#include "pastel/geometry/alignedbox.h"
namespace Pastel
{

	template class AlignedBox<real, 1>;
	template class AlignedBox<real, 2>;
	template class AlignedBox<real, 3>;
	template class AlignedBox<real, 4>;
	template class AlignedBox<real, 5>;

}

#include "pastel/geometry/alignedplane.h"
namespace Pastel
{

	template class AlignedPlane<real, 1>;
	template class AlignedPlane<real, 2>;
	template class AlignedPlane<real, 3>;
	template class AlignedPlane<real, 4>;
	template class AlignedPlane<real, 5>;

}

#include "pastel/math/affinetransformation.h"
namespace Pastel
{

	template class AffineTransformation<real, 1>;
	template class AffineTransformation<real, 2>;
	template class AffineTransformation<real, 3>;
	template class AffineTransformation<real, 4>;
	template class AffineTransformation<real, 5>;

}

#include "pastel/math/matrix.h"
namespace Pastel
{

	template class Matrix<real>;

}

#include "pastel/geometry/sphere.h"
namespace Pastel
{

	template class Sphere<real, 1>;
	template class Sphere<real, 2>;
	template class Sphere<real, 3>;
	template class Sphere<real, 4>;
	template class Sphere<real, 5>;

}

#include "pastel/geometry/segment.h"
namespace Pastel
{

	template class Segment<real, 1>;
	template class Segment<real, 2>;
	template class Segment<real, 3>;
	template class Segment<real, 4>;
	template class Segment<real, 5>;

}

#include "pastel/geometry/line.h"
namespace Pastel
{

	template class Line<real, 1>;
	template class Line<real, 2>;
	template class Line<real, 3>;
	template class Line<real, 4>;
	template class Line<real, 5>;

}

#include "pastel/sys/vector.h"
namespace Pastel
{

	template class Vector<real, 1>;
	template class Vector<real, 2>;
	template class Vector<real, 3>;
	template class Vector<real, 4>;
	template class Vector<real, 5>;

}

#include "pastel/geometry/box.h"
namespace Pastel
{

	template class Box<real, 1>;
	template class Box<real, 2>;
	template class Box<real, 3>;
	template class Box<real, 4>;
	template class Box<real, 5>;

}

#include "pastel/geometry/plane.h"
namespace Pastel
{

	template class Plane<real, 1>;
	template class Plane<real, 2>;
	template class Plane<real, 3>;
	template class Plane<real, 4>;
	template class Plane<real, 5>;

}
