// Description: Template instantiation of shapes for testing
// Documentation: test_pastelmath.txt

#include "pastel/sys/mytypes.h"

#include "pastel/geometry/shapes/alignedbox.h"
namespace Pastel
{

	template class AlignedBox<real, 1>;
	template class AlignedBox<real, 2>;
	template class AlignedBox<real, 3>;
	template class AlignedBox<real, 4>;
	template class AlignedBox<real, 5>;

}

#include "pastel/geometry/shapes/alignedplane.h"
namespace Pastel
{

	template class AlignedPlane<real, 1>;
	template class AlignedPlane<real, 2>;
	template class AlignedPlane<real, 3>;
	template class AlignedPlane<real, 4>;
	template class AlignedPlane<real, 5>;

}

#include "pastel/geometry/shapes/sphere.h"
namespace Pastel
{

	template class Sphere<real, 1>;
	template class Sphere<real, 2>;
	template class Sphere<real, 3>;
	template class Sphere<real, 4>;
	template class Sphere<real, 5>;

}

#include "pastel/geometry/shapes/segment.h"
namespace Pastel
{

	template class Segment<real, 1>;
	template class Segment<real, 2>;
	template class Segment<real, 3>;
	template class Segment<real, 4>;
	template class Segment<real, 5>;

}

#include "pastel/geometry/shapes/line.h"
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

#include "pastel/geometry/shapes/box.h"
namespace Pastel
{

	template class Box<real, 1>;
	template class Box<real, 2>;
	template class Box<real, 3>;
	template class Box<real, 4>;
	template class Box<real, 5>;

}

#include "pastel/geometry/shapes/plane.h"
namespace Pastel
{

	template class Plane<real, 1>;
	template class Plane<real, 2>;
	template class Plane<real, 3>;
	template class Plane<real, 4>;
	template class Plane<real, 5>;

}
