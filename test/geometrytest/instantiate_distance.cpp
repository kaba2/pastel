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
			const AlignedBox<1, TestReal>& aBox,
			const AlignedBox<1, TestReal>& bBox);

	template
		TestReal distance2(
			const AlignedBox<2, TestReal>& aBox,
			const AlignedBox<2, TestReal>& bBox);

	template
		TestReal distance2(
			const AlignedBox<3, TestReal>& aBox,
			const AlignedBox<3, TestReal>& bBox);

	template
		TestReal distance2(
			const AlignedBox<4, TestReal>& aBox,
			const AlignedBox<4, TestReal>& bBox);

	template
		TestReal distance2(
			const AlignedBox<5, TestReal>& aBox,
			const AlignedBox<5, TestReal>& bBox);

}

#include "pastel/geometry/distance_alignedbox_point.h"

namespace Pastel
{

	template
		TestReal distance2(
			const AlignedBox<1, TestReal>& alignedBox,
			const Point<TestReal, 1>& point);

	template
		TestReal distance2(
			const AlignedBox<2, TestReal>& alignedBox,
			const Point<TestReal, 2>& point);

	template
		TestReal distance2(
			const AlignedBox<3, TestReal>& alignedBox,
			const Point<TestReal, 3>& point);

	template
		TestReal distance2(
			const AlignedBox<4, TestReal>& alignedBox,
			const Point<TestReal, 4>& point);

	template
		TestReal distance2(
			const AlignedBox<5, TestReal>& alignedBox,
			const Point<TestReal, 5>& point);

}

#include "pastel/geometry/distance_alignedbox_sphere.h"

namespace Pastel
{

	template
	TestReal distance2(
		const AlignedBox<1, TestReal>& alignedBox,
		const Sphere<1, TestReal>& sphere);

	template
	TestReal distance2(
		const AlignedBox<2, TestReal>& alignedBox,
		const Sphere<2, TestReal>& sphere);

	template
	TestReal distance2(
		const AlignedBox<3, TestReal>& alignedBox,
		const Sphere<3, TestReal>& sphere);

	template
	TestReal distance2(
		const AlignedBox<4, TestReal>& alignedBox,
		const Sphere<4, TestReal>& sphere);

	template
	TestReal distance2(
		const AlignedBox<5, TestReal>& alignedBox,
		const Sphere<5, TestReal>& sphere);

}

#include "pastel/geometry/distance_line_line.h"

namespace Pastel
{

	template TestReal distance2(const Line<1, TestReal>&,
		const Line<1, TestReal>&);

	template TestReal distance2(const Line<2, TestReal>&,
		const Line<2, TestReal>&);

	template TestReal distance2(const Line<3, TestReal>&,
		const Line<3, TestReal>&);

	template TestReal distance2(const Line<4, TestReal>&,
		const Line<4, TestReal>&);

	template TestReal distance2(const Line<5, TestReal>&,
		const Line<5, TestReal>&);

}

#include "pastel/geometry/distance_line_point.h"

namespace Pastel
{

	template TestReal distance2(const Line<1, TestReal>&,
		const Point<TestReal, 1>&);

	template TestReal distance2(const Line<2, TestReal>&,
		const Point<TestReal, 2>&);

	template TestReal distance2(const Line<3, TestReal>&,
		const Point<TestReal, 3>&);

	template TestReal distance2(const Line<4, TestReal>&,
		const Point<TestReal, 4>&);

	template TestReal distance2(const Line<5, TestReal>&,
		const Point<TestReal, 5>&);

}

#include "pastel/geometry/distance_plane_point.h"

namespace Pastel
{

	template
	TestReal distance2(
		const Plane<1, TestReal>& plane,
		const Point<TestReal, 1>& point);

	template
	TestReal distance2(
		const Plane<2, TestReal>& plane,
		const Point<TestReal, 2>& point);

	template
	TestReal distance2(
		const Plane<3, TestReal>& plane,
		const Point<TestReal, 3>& point);

	template
	TestReal distance2(
		const Plane<4, TestReal>& plane,
		const Point<TestReal, 4>& point);

	template
	TestReal distance2(
		const Plane<5, TestReal>& plane,
		const Point<TestReal, 5>& point);

}
