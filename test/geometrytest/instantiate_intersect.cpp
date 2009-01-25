#include "pastel/sys/mytypes.h"

namespace
{

	typedef Pastel::real TestReal;

}

#include "pastel/geometry/intersect_flat_plane.h"

namespace Pastel
{

	template
		bool intersect(
		const Flat<1, TestReal, 2>& aPlane,
		const Plane<1, TestReal>& bPlane,
		Line<1, TestReal>& line);

	template
		bool intersect(
		const Flat<2, TestReal, 2>& aPlane,
		const Plane<2, TestReal>& bPlane,
		Line<2, TestReal>& line);

	template
		bool intersect(
		const Flat<3, TestReal, 2>& aPlane,
		const Plane<3, TestReal>& bPlane,
		Line<3, TestReal>& line);

	template
		bool intersect(
		const Flat<4, TestReal, 2>& aPlane,
		const Plane<4, TestReal>& bPlane,
		Line<4, TestReal>& line);

	template
		bool intersect(
		const Flat<5, TestReal, 2>& aPlane,
		const Plane<5, TestReal>& bPlane,
		Line<5, TestReal>& line);

}

#include "pastel/geometry/intersect_line_alignedbox.h"

namespace Pastel
{

	template bool intersect(const Line<1, TestReal>&,
		const AlignedBox<1, TestReal>&, Vector<2, TestReal>&);

	template bool intersect(const Line<2, TestReal>&,
		const AlignedBox<2, TestReal>&, Vector<2, TestReal>&);

	template bool intersect(const Line<3, TestReal>&,
		const AlignedBox<3, TestReal>&, Vector<2, TestReal>&);

	template bool intersect(const Line<4, TestReal>&,
		const AlignedBox<4, TestReal>&, Vector<2, TestReal>&);

	template bool intersect(const Line<5, TestReal>&,
		const AlignedBox<5, TestReal>&, Vector<2, TestReal>&);

}

#include "pastel/geometry/intersect_line_alignedplane.h"

namespace Pastel
{

	template bool intersect(const Line<1, TestReal>&,
		const AlignedPlane<1, TestReal>&, TestReal&);

	template bool intersect(const Line<2, TestReal>&,
		const AlignedPlane<2, TestReal>&, TestReal&);

	template bool intersect(const Line<3, TestReal>&,
		const AlignedPlane<3, TestReal>&, TestReal&);

	template bool intersect(const Line<4, TestReal>&,
		const AlignedPlane<4, TestReal>&, TestReal&);

	template bool intersect(const Line<5, TestReal>&,
		const AlignedPlane<5, TestReal>&, TestReal&);

}

#include "pastel/geometry/intersect_line_box.h"

namespace Pastel
{

	template bool intersect(const Line<1, TestReal>&,
		const Box<1, TestReal>&, Vector<2, TestReal>&);

	template bool intersect(const Line<2, TestReal>&,
		const Box<2, TestReal>&, Vector<2, TestReal>&);

	template bool intersect(const Line<3, TestReal>&,
		const Box<3, TestReal>&, Vector<2, TestReal>&);

	template bool intersect(const Line<4, TestReal>&,
		const Box<4, TestReal>&, Vector<2, TestReal>&);

	template bool intersect(const Line<5, TestReal>&,
		const Box<5, TestReal>&, Vector<2, TestReal>&);

}

#include "pastel/geometry/intersect_line_plane.h"

namespace Pastel
{

	template bool intersect(const Line<1, TestReal>&,
		const Plane<1, TestReal>&, TestReal&);

	template bool intersect(const Line<2, TestReal>&,
		const Plane<2, TestReal>&, TestReal&);

	template bool intersect(const Line<3, TestReal>&,
		const Plane<3, TestReal>&, TestReal&);

	template bool intersect(const Line<4, TestReal>&,
		const Plane<4, TestReal>&, TestReal&);

	template bool intersect(const Line<5, TestReal>&,
		const Plane<5, TestReal>&, TestReal&);

}

#include "pastel/geometry/intersect_line_sphere.h"

namespace Pastel
{

	template bool intersect(const Line<1, TestReal>&,
		const Sphere<1, TestReal>&, Vector<2, TestReal>&);

	template bool intersect(const Line<2, TestReal>&,
		const Sphere<2, TestReal>&, Vector<2, TestReal>&);

	template bool intersect(const Line<3, TestReal>&,
		const Sphere<3, TestReal>&, Vector<2, TestReal>&);

	template bool intersect(const Line<4, TestReal>&,
		const Sphere<4, TestReal>&, Vector<2, TestReal>&);

	template bool intersect(const Line<5, TestReal>&,
		const Sphere<5, TestReal>&, Vector<2, TestReal>&);

}

#include "pastel/geometry/intersect_line_triangle.h"

namespace Pastel
{

	template
		bool intersect(
		const Line<3, TestReal>& line,
		const Triangle<3, TestReal>& triangle,
		TestReal& tIntersection,
		TestReal& uIntersection,
		TestReal& vIntersection);

}

#include "pastel/geometry/intersect_triangle_triangle.h"

namespace Pastel
{

	template
	bool intersect(
		const Triangle<3, TestReal>& aTriangle,
		const Triangle<3, TestReal>& bTriangle,
		Segment<3, TestReal>& segment);

}
