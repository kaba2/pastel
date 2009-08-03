#include "pastel/sys/mytypes.h"

namespace
{

	typedef Pastel::real TestReal;

}

#include "pastel/geometry/distance_alignedbox_alignedbox.h"

namespace Pastel
{

	template
		TestReal distance2(
			const AlignedBox<TestReal, 1>& aBox,
			const AlignedBox<TestReal, 1>& bBox);

	template
		TestReal distance2(
			const AlignedBox<TestReal, 2>& aBox,
			const AlignedBox<TestReal, 2>& bBox);

	template
		TestReal distance2(
			const AlignedBox<TestReal, 3>& aBox,
			const AlignedBox<TestReal, 3>& bBox);

	template
		TestReal distance2(
			const AlignedBox<TestReal, 4>& aBox,
			const AlignedBox<TestReal, 4>& bBox);

	template
		TestReal distance2(
			const AlignedBox<TestReal, 5>& aBox,
			const AlignedBox<TestReal, 5>& bBox);

}

#include "pastel/geometry/distance_alignedbox_point.h"

namespace Pastel
{

	template
		TestReal distance2(
			const AlignedBox<TestReal, 1>& alignedBox,
			const Point<TestReal, 1>& point);

	template
		TestReal distance2(
			const AlignedBox<TestReal, 2>& alignedBox,
			const Point<TestReal, 2>& point);

	template
		TestReal distance2(
			const AlignedBox<TestReal, 3>& alignedBox,
			const Point<TestReal, 3>& point);

	template
		TestReal distance2(
			const AlignedBox<TestReal, 4>& alignedBox,
			const Point<TestReal, 4>& point);

	template
		TestReal distance2(
			const AlignedBox<TestReal, 5>& alignedBox,
			const Point<TestReal, 5>& point);

}

#include "pastel/geometry/distance_alignedbox_sphere.h"

namespace Pastel
{

	template
	TestReal distance2(
		const AlignedBox<TestReal, 1>& alignedBox,
		const Sphere<TestReal, 1>& sphere);

	template
	TestReal distance2(
		const AlignedBox<TestReal, 2>& alignedBox,
		const Sphere<TestReal, 2>& sphere);

	template
	TestReal distance2(
		const AlignedBox<TestReal, 3>& alignedBox,
		const Sphere<TestReal, 3>& sphere);

	template
	TestReal distance2(
		const AlignedBox<TestReal, 4>& alignedBox,
		const Sphere<TestReal, 4>& sphere);

	template
	TestReal distance2(
		const AlignedBox<TestReal, 5>& alignedBox,
		const Sphere<TestReal, 5>& sphere);

}

#include "pastel/geometry/distance_line_line.h"

namespace Pastel
{

	template TestReal distance2(const Line<TestReal, 1>&,
		const Line<TestReal, 1>&);

	template TestReal distance2(const Line<TestReal, 2>&,
		const Line<TestReal, 2>&);

	template TestReal distance2(const Line<TestReal, 3>&,
		const Line<TestReal, 3>&);

	template TestReal distance2(const Line<TestReal, 4>&,
		const Line<TestReal, 4>&);

	template TestReal distance2(const Line<TestReal, 5>&,
		const Line<TestReal, 5>&);

}

#include "pastel/geometry/distance_line_point.h"

namespace Pastel
{

	template TestReal distance2(const Line<TestReal, 1>&,
		const Point<TestReal, 1>&);

	template TestReal distance2(const Line<TestReal, 2>&,
		const Point<TestReal, 2>&);

	template TestReal distance2(const Line<TestReal, 3>&,
		const Point<TestReal, 3>&);

	template TestReal distance2(const Line<TestReal, 4>&,
		const Point<TestReal, 4>&);

	template TestReal distance2(const Line<TestReal, 5>&,
		const Point<TestReal, 5>&);

}

#include "pastel/geometry/distance_plane_point.h"

namespace Pastel
{

	template
	TestReal distance2(
		const Plane<TestReal, 1>& plane,
		const Point<TestReal, 1>& point);

	template
	TestReal distance2(
		const Plane<TestReal, 2>& plane,
		const Point<TestReal, 2>& point);

	template
	TestReal distance2(
		const Plane<TestReal, 3>& plane,
		const Point<TestReal, 3>& point);

	template
	TestReal distance2(
		const Plane<TestReal, 4>& plane,
		const Point<TestReal, 4>& point);

	template
	TestReal distance2(
		const Plane<TestReal, 5>& plane,
		const Point<TestReal, 5>& point);

}
