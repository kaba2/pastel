// Template instantiation of math classes
// for compile testing purposes.

#include "pastel/sys/mytypes.h"

#include "pastel/geometry/alignedbox.h"
namespace Pastel
{

	template class AlignedBox<1, real>;
	template class AlignedBox<2, real>;
	template class AlignedBox<3, real>;
	template class AlignedBox<4, real>;
	template class AlignedBox<5, real>;

}

#include "pastel/geometry/alignedplane.h"
namespace Pastel
{

	template class AlignedPlane<1, real>;
	template class AlignedPlane<2, real>;
	template class AlignedPlane<3, real>;
	template class AlignedPlane<4, real>;
	template class AlignedPlane<5, real>;

}

#include "pastel/math/affinetransformation.h"
namespace Pastel
{

	template class AffineTransformation<1, real>;
	template class AffineTransformation<2, real>;
	template class AffineTransformation<3, real>;
	template class AffineTransformation<4, real>;
	template class AffineTransformation<5, real>;

}

#include "pastel/math/matrix.h"
namespace Pastel
{

	template class Matrix<Dynamic, Dynamic, real>;
	template class Matrix<1, 1, real>;
	template class Matrix<2, 2, real>;
	template class Matrix<3, 3, real>;
	template class Matrix<4, 4, real>;
	template class Matrix<2, 1, real>;
	template class Matrix<3, 1, real>;
	template class Matrix<4, 1, real>;
	template class Matrix<1, 2, real>;
	template class Matrix<1, 3, real>;
	template class Matrix<1, 4, real>;
	template class Matrix<7, 5, real>;

}

#include "pastel/math/transformation.h"
namespace Pastel
{

	template class Transformation<1, real>;
	template class Transformation<2, real>;
	template class Transformation<3, real>;
	template class Transformation<4, real>;
	template class Transformation<5, real>;

}

#include "pastel/geometry/sphere.h"
namespace Pastel
{

	template class Sphere<1, real>;
	template class Sphere<2, real>;
	template class Sphere<3, real>;
	template class Sphere<4, real>;
	template class Sphere<5, real>;

}

#include "pastel/geometry/segment.h"
namespace Pastel
{

	template class Segment<1, real>;
	template class Segment<2, real>;
	template class Segment<3, real>;
	template class Segment<4, real>;
	template class Segment<5, real>;

}

#include "pastel/geometry/line.h"
namespace Pastel
{

	template class Line<1, real>;
	template class Line<2, real>;
	template class Line<3, real>;
	template class Line<4, real>;
	template class Line<5, real>;

}

#include "pastel/sys/point.h"
namespace Pastel
{
	template class Point<real, 1>;
	template class Point<real, 2>;
	template class Point<real, 3>;
	template class Point<real, 4>;
	template class Point<real, 5>;
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

	template class Box<1, real>;
	template class Box<2, real>;
	template class Box<3, real>;
	template class Box<4, real>;
	template class Box<5, real>;

}

#include "pastel/geometry/plane.h"
namespace Pastel
{

	template class Plane<1, real>;
	template class Plane<2, real>;
	template class Plane<3, real>;
	template class Plane<4, real>;
	template class Plane<5, real>;

}
