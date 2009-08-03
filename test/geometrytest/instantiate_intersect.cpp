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
		const Flat<TestReal, 1, 2>& aPlane,
		const Plane<TestReal, 1>& bPlane,
		Line<TestReal, 1>& line);

	template
		bool intersect(
		const Flat<TestReal, 2, 2>& aPlane,
		const Plane<TestReal, 2>& bPlane,
		Line<TestReal, 2>& line);

	template
		bool intersect(
		const Flat<TestReal, 3, 2>& aPlane,
		const Plane<TestReal, 3>& bPlane,
		Line<TestReal, 3>& line);

	template
		bool intersect(
		const Flat<TestReal, 4, 2>& aPlane,
		const Plane<TestReal, 4>& bPlane,
		Line<TestReal, 4>& line);

	template
		bool intersect(
		const Flat<TestReal, 5, 2>& aPlane,
		const Plane<TestReal, 5>& bPlane,
		Line<TestReal, 5>& line);

}

#include "pastel/geometry/intersect_line_alignedbox.h"

namespace Pastel
{

	template bool intersect(const Line<TestReal, 1>&,
		const AlignedBox<TestReal, 1>&, Vector<TestReal, 2>&);

	template bool intersect(const Line<TestReal, 2>&,
		const AlignedBox<TestReal, 2>&, Vector<TestReal, 2>&);

	template bool intersect(const Line<TestReal, 3>&,
		const AlignedBox<TestReal, 3>&, Vector<TestReal, 2>&);

	template bool intersect(const Line<TestReal, 4>&,
		const AlignedBox<TestReal, 4>&, Vector<TestReal, 2>&);

	template bool intersect(const Line<TestReal, 5>&,
		const AlignedBox<TestReal, 5>&, Vector<TestReal, 2>&);

}

#include "pastel/geometry/intersect_line_alignedplane.h"

namespace Pastel
{

	template bool intersect(const Line<TestReal, 1>&,
		const AlignedPlane<TestReal, 1>&, TestReal&);

	template bool intersect(const Line<TestReal, 2>&,
		const AlignedPlane<TestReal, 2>&, TestReal&);

	template bool intersect(const Line<TestReal, 3>&,
		const AlignedPlane<TestReal, 3>&, TestReal&);

	template bool intersect(const Line<TestReal, 4>&,
		const AlignedPlane<TestReal, 4>&, TestReal&);

	template bool intersect(const Line<TestReal, 5>&,
		const AlignedPlane<TestReal, 5>&, TestReal&);

}

#include "pastel/geometry/intersect_line_box.h"

namespace Pastel
{

	template bool intersect(const Line<TestReal, 1>&,
		const Box<TestReal, 1>&, Vector<TestReal, 2>&);

	template bool intersect(const Line<TestReal, 2>&,
		const Box<TestReal, 2>&, Vector<TestReal, 2>&);

	template bool intersect(const Line<TestReal, 3>&,
		const Box<TestReal, 3>&, Vector<TestReal, 2>&);

	template bool intersect(const Line<TestReal, 4>&,
		const Box<TestReal, 4>&, Vector<TestReal, 2>&);

	template bool intersect(const Line<TestReal, 5>&,
		const Box<TestReal, 5>&, Vector<TestReal, 2>&);

}

#include "pastel/geometry/intersect_line_plane.h"

namespace Pastel
{

	template bool intersect(const Line<TestReal, 1>&,
		const Plane<TestReal, 1>&, TestReal&);

	template bool intersect(const Line<TestReal, 2>&,
		const Plane<TestReal, 2>&, TestReal&);

	template bool intersect(const Line<TestReal, 3>&,
		const Plane<TestReal, 3>&, TestReal&);

	template bool intersect(const Line<TestReal, 4>&,
		const Plane<TestReal, 4>&, TestReal&);

	template bool intersect(const Line<TestReal, 5>&,
		const Plane<TestReal, 5>&, TestReal&);

}

#include "pastel/geometry/intersect_line_sphere.h"

namespace Pastel
{

	template bool intersect(const Line<TestReal, 1>&,
		const Sphere<TestReal, 1>&, Vector<TestReal, 2>&);

	template bool intersect(const Line<TestReal, 2>&,
		const Sphere<TestReal, 2>&, Vector<TestReal, 2>&);

	template bool intersect(const Line<TestReal, 3>&,
		const Sphere<TestReal, 3>&, Vector<TestReal, 2>&);

	template bool intersect(const Line<TestReal, 4>&,
		const Sphere<TestReal, 4>&, Vector<TestReal, 2>&);

	template bool intersect(const Line<TestReal, 5>&,
		const Sphere<TestReal, 5>&, Vector<TestReal, 2>&);

}

#include "pastel/geometry/intersect_line_triangle.h"

namespace Pastel
{

	template
		bool intersect(
		const Line<TestReal, 3>& line,
		const Triangle<TestReal, 3>& triangle,
		TestReal& tIntersection,
		TestReal& uIntersection,
		TestReal& vIntersection);

}

#include "pastel/geometry/intersect_triangle_triangle.h"

namespace Pastel
{

	template
	bool intersect(
		const Triangle<TestReal, 3>& aTriangle,
		const Triangle<TestReal, 3>& bTriangle,
		Line<TestReal, 3>& intersectionLine,
		AlignedBox<TestReal, 1>& intersectionRange);

}
