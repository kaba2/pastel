// Description: Template instantiation of shapes for testing
// DocumentationOf: shape.h

#include "pastel/sys/mytypes.h"

#include "pastel/geometry/shape/alignedbox.h"
namespace Pastel
{

	template class AlignedBox<dreal, 1>;
	template class AlignedBox<dreal, 2>;
	template class AlignedBox<dreal, 3>;
	template class AlignedBox<dreal, 4>;
	template class AlignedBox<dreal, 5>;

}

#include "pastel/geometry/shape/alignedplane.h"
namespace Pastel
{

	template class AlignedPlane<dreal, 1>;
	template class AlignedPlane<dreal, 2>;
	template class AlignedPlane<dreal, 3>;
	template class AlignedPlane<dreal, 4>;
	template class AlignedPlane<dreal, 5>;

}

#include "pastel/geometry/shape/sphere.h"
namespace Pastel
{

	template class Sphere<dreal, 1>;
	template class Sphere<dreal, 2>;
	template class Sphere<dreal, 3>;
	template class Sphere<dreal, 4>;
	template class Sphere<dreal, 5>;

}

#include "pastel/geometry/shape/segment.h"
namespace Pastel
{

	template class Segment<dreal, 1>;
	template class Segment<dreal, 2>;
	template class Segment<dreal, 3>;
	template class Segment<dreal, 4>;
	template class Segment<dreal, 5>;

}

#include "pastel/geometry/shape/line.h"
namespace Pastel
{

	template class Line<dreal, 1>;
	template class Line<dreal, 2>;
	template class Line<dreal, 3>;
	template class Line<dreal, 4>;
	template class Line<dreal, 5>;

}

#include "pastel/sys/vector.h"
namespace Pastel
{

	template class Vector<dreal, 1>;
	template class Vector<dreal, 2>;
	template class Vector<dreal, 3>;
	template class Vector<dreal, 4>;
	template class Vector<dreal, 5>;

}

#include "pastel/geometry/shape/box.h"
namespace Pastel
{

	template class Box<dreal, 1>;
	template class Box<dreal, 2>;
	template class Box<dreal, 3>;
	template class Box<dreal, 4>;
	template class Box<dreal, 5>;

}

#include "pastel/geometry/shape/plane.h"
namespace Pastel
{

	template class Plane<dreal, 1>;
	template class Plane<dreal, 2>;
	template class Plane<dreal, 3>;
	template class Plane<dreal, 4>;
	template class Plane<dreal, 5>;

}
