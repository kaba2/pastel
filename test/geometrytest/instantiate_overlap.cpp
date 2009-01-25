#include "pastel/sys/mytypes.h"

namespace
{

	typedef Pastel::real TestReal;

}

#include "pastel/geometry/overlaps_alignedbox_alignedbox.h"

namespace Pastel
{

	template
		bool overlaps(
		const AlignedBox<1, TestReal>& a,
		const AlignedBox<1, TestReal>& b);

	template
		bool overlaps(
		const AlignedBox<2, TestReal>& a,
		const AlignedBox<2, TestReal>& b);

	template
		bool overlaps(
		const AlignedBox<3, TestReal>& a,
		const AlignedBox<3, TestReal>& b);

	template
		bool overlaps(
		const AlignedBox<4, TestReal>& a,
		const AlignedBox<4, TestReal>& b);

	template
		bool overlaps(
		const AlignedBox<5, TestReal>& a,
		const AlignedBox<5, TestReal>& b);

}

#include "pastel/geometry/overlaps_alignedbox_box.h"

namespace Pastel
{

	/*
	template
	bool overlaps(
		const AlignedBox<1, TestReal>& a,
		const Box<1, TestReal>& b);
	*/

	template
	bool overlaps(
		const AlignedBox<2, TestReal>& a,
		const Box<2, TestReal>& b);

	template
	bool overlaps(
		const AlignedBox<3, TestReal>& a,
		const Box<3, TestReal>& b);

	/*
	template
	bool overlaps(
		const AlignedBox<4, TestReal>& a,
		const Box<4, TestReal>& b);

	template
	bool overlaps(
		const AlignedBox<5, TestReal>& a,
		const Box<5, TestReal>& b);
	*/

}

#include "pastel/geometry/overlaps_alignedbox_plane.h"

namespace Pastel
{

	template
		bool overlaps(
			const AlignedBox<1, TestReal>& alignedBox,
			const Plane<1, TestReal>& plane);

	template
		bool overlaps(
			const AlignedBox<2, TestReal>& alignedBox,
			const Plane<2, TestReal>& plane);

	template
		bool overlaps(
			const AlignedBox<3, TestReal>& alignedBox,
			const Plane<3, TestReal>& plane);

	template
		bool overlaps(
			const AlignedBox<4, TestReal>& alignedBox,
			const Plane<4, TestReal>& plane);

	template
		bool overlaps(
			const AlignedBox<5, TestReal>& alignedBox,
			const Plane<5, TestReal>& plane);

}

#include "pastel/geometry/overlaps_alignedbox_point.h"

namespace Pastel
{

	template
		bool overlaps(
			const AlignedBox<1, TestReal>& alignedBox,
			const Point<1, TestReal>& point);

	template
		bool overlaps(
			const AlignedBox<2, TestReal>& alignedBox,
			const Point<2, TestReal>& point);

	template
		bool overlaps(
			const AlignedBox<3, TestReal>& alignedBox,
			const Point<3, TestReal>& point);

	template
		bool overlaps(
			const AlignedBox<4, TestReal>& alignedBox,
			const Point<4, TestReal>& point);

	template
		bool overlaps(
			const AlignedBox<5, TestReal>& alignedBox,
			const Point<5, TestReal>& point);

}

#include "pastel/geometry/overlaps_alignedbox_sphere.h"

namespace Pastel
{

	template
	bool overlaps(
		const AlignedBox<1, TestReal>& alignedBox,
		const Sphere<1, TestReal>& sphere);

	template
	bool overlaps(
		const AlignedBox<2, TestReal>& alignedBox,
		const Sphere<2, TestReal>& sphere);

	template
	bool overlaps(
		const AlignedBox<3, TestReal>& alignedBox,
		const Sphere<3, TestReal>& sphere);

	template
	bool overlaps(
		const AlignedBox<4, TestReal>& alignedBox,
		const Sphere<4, TestReal>& sphere);

	template
	bool overlaps(
		const AlignedBox<5, TestReal>& alignedBox,
		const Sphere<5, TestReal>& sphere);

}

#include "pastel/geometry/overlaps_alignedbox_triangle.h"

namespace Pastel
{

	/*
	template
	bool overlaps(
		const AlignedBox<1, TestReal>& alignedBox,
		const Triangle<1, TestReal>& triangle);
	*/

	template
	bool overlaps(
		const AlignedBox<2, TestReal>& alignedBox,
		const Triangle<2, TestReal>& triangle);

	template
	bool overlaps(
		const AlignedBox<3, TestReal>& alignedBox,
		const Triangle<3, TestReal>& triangle);

	/*
	template
	bool overlaps(
		const AlignedBox<4, TestReal>& alignedBox,
		const Triangle<4, TestReal>& triangle);

	template
	bool overlaps(
		const AlignedBox<5, TestReal>& alignedBox,
		const Triangle<5, TestReal>& triangle);
	*/

}

#include "pastel/geometry/overlaps_alignedplane_alignedbox.h"

namespace Pastel
{

	template
		bool overlaps(AlignedPlane<1, TestReal> const &alignedPlane,
			AlignedBox<1, TestReal> const &alignedBox);

	template
		bool overlaps(AlignedPlane<1, TestReal> const &alignedPlane,
			AlignedBox<1, TestReal> const &alignedBox,
			bool &greater);

	template
		bool overlaps(AlignedPlane<2, TestReal> const &alignedPlane,
			AlignedBox<2, TestReal> const &alignedBox);

	template
		bool overlaps(AlignedPlane<2, TestReal> const &alignedPlane,
			AlignedBox<2, TestReal> const &alignedBox,
			bool &greater);

	template
		bool overlaps(AlignedPlane<3, TestReal> const &alignedPlane,
			AlignedBox<3, TestReal> const &alignedBox);

	template
		bool overlaps(AlignedPlane<3, TestReal> const &alignedPlane,
			AlignedBox<3, TestReal> const &alignedBox,
			bool &greater);

	template
		bool overlaps(AlignedPlane<4, TestReal> const &alignedPlane,
			AlignedBox<4, TestReal> const &alignedBox);

	template
		bool overlaps(AlignedPlane<4, TestReal> const &alignedPlane,
			AlignedBox<4, TestReal> const &alignedBox,
			bool &greater);

	template
		bool overlaps(AlignedPlane<5, TestReal> const &alignedPlane,
			AlignedBox<5, TestReal> const &alignedBox);

	template
		bool overlaps(AlignedPlane<5, TestReal> const &alignedPlane,
			AlignedBox<5, TestReal> const &alignedBox,
			bool &greater);

}

#include "pastel/geometry/overlaps_alignedplane_box.h"

namespace Pastel
{

	template
	bool overlaps(AlignedPlane<1, TestReal> const &alignedPlane,
		Box<1, TestReal> const &box);

	template
		bool overlaps(AlignedPlane<1, TestReal> const &alignedPlane,
			Box<1, TestReal> const &box,
			bool &);

	template
	bool overlaps(AlignedPlane<2, TestReal> const &alignedPlane,
		Box<2, TestReal> const &box);

	template
		bool overlaps(AlignedPlane<2, TestReal> const &alignedPlane,
			Box<2, TestReal> const &box,
			bool &);

	template
	bool overlaps(AlignedPlane<3, TestReal> const &alignedPlane,
		Box<3, TestReal> const &box);

	template
		bool overlaps(AlignedPlane<3, TestReal> const &alignedPlane,
			Box<3, TestReal> const &box,
			bool &);

	template
	bool overlaps(AlignedPlane<4, TestReal> const &alignedPlane,
		Box<4, TestReal> const &box);

	template
		bool overlaps(AlignedPlane<4, TestReal> const &alignedPlane,
			Box<4, TestReal> const &box,
			bool &);

	template
	bool overlaps(AlignedPlane<5, TestReal> const &alignedPlane,
		Box<5, TestReal> const &box);

	template
		bool overlaps(AlignedPlane<5, TestReal> const &alignedPlane,
			Box<5, TestReal> const &box,
			bool &);

}

#include "pastel/geometry/overlaps_alignedplane_sphere.h"

namespace Pastel
{

	template
	bool overlaps(AlignedPlane<1, TestReal> const &alignedPlane,
		Sphere<1, TestReal> const &sphere);

	template
	bool overlaps(AlignedPlane<1, TestReal> const &alignedPlane,
		Sphere<1, TestReal> const &sphere,
		bool &alignedPlaneOnPositiveSide);

	template
	bool overlaps(AlignedPlane<2, TestReal> const &alignedPlane,
		Sphere<2, TestReal> const &sphere);

	template
	bool overlaps(AlignedPlane<2, TestReal> const &alignedPlane,
		Sphere<2, TestReal> const &sphere,
		bool &alignedPlaneOnPositiveSide);

	template
	bool overlaps(AlignedPlane<3, TestReal> const &alignedPlane,
		Sphere<3, TestReal> const &sphere);

	template
	bool overlaps(AlignedPlane<3, TestReal> const &alignedPlane,
		Sphere<3, TestReal> const &sphere,
		bool &alignedPlaneOnPositiveSide);

	template
	bool overlaps(AlignedPlane<4, TestReal> const &alignedPlane,
		Sphere<4, TestReal> const &sphere);

	template
	bool overlaps(AlignedPlane<4, TestReal> const &alignedPlane,
		Sphere<4, TestReal> const &sphere,
		bool &alignedPlaneOnPositiveSide);

	template
	bool overlaps(AlignedPlane<5, TestReal> const &alignedPlane,
		Sphere<5, TestReal> const &sphere);

	template
	bool overlaps(AlignedPlane<5, TestReal> const &alignedPlane,
		Sphere<5, TestReal> const &sphere,
		bool &alignedPlaneOnPositiveSide);

}

#include "pastel/geometry/overlaps_alignedplane_triangle.h"

namespace Pastel
{

	template
		bool overlaps(const AlignedPlane<1, TestReal>& alignedPlane,
			const Triangle<1, TestReal>& triangle);

	template
		bool overlaps(const AlignedPlane<1, TestReal>& alignedPlane,
			const Triangle<1, TestReal>& triangle,
			bool& alignedPlaneOnPositiveSide);

	template
		bool overlaps(const AlignedPlane<2, TestReal>& alignedPlane,
			const Triangle<2, TestReal>& triangle);

	template
		bool overlaps(const AlignedPlane<2, TestReal>& alignedPlane,
			const Triangle<2, TestReal>& triangle,
			bool& alignedPlaneOnPositiveSide);

	template
		bool overlaps(const AlignedPlane<3, TestReal>& alignedPlane,
			const Triangle<3, TestReal>& triangle);

	template
		bool overlaps(const AlignedPlane<3, TestReal>& alignedPlane,
			const Triangle<3, TestReal>& triangle,
			bool& alignedPlaneOnPositiveSide);

	template
		bool overlaps(const AlignedPlane<4, TestReal>& alignedPlane,
			const Triangle<4, TestReal>& triangle);

	template
		bool overlaps(const AlignedPlane<4, TestReal>& alignedPlane,
			const Triangle<4, TestReal>& triangle,
			bool& alignedPlaneOnPositiveSide);

	template
		bool overlaps(const AlignedPlane<5, TestReal>& alignedPlane,
			const Triangle<5, TestReal>& triangle);

	template
		bool overlaps(const AlignedPlane<5, TestReal>& alignedPlane,
			const Triangle<5, TestReal>& triangle,
			bool& alignedPlaneOnPositiveSide);

}

#include "pastel/geometry/overlaps_box_box.h"

namespace Pastel
{

	/*
	template
	bool overlaps(Box<1, TestReal> const&,
		Box<1, TestReal> const &nb);
	*/

	template
	bool overlaps(Box<2, TestReal> const&,
		Box<2, TestReal> const &nb);

	template
	bool overlaps(Box<3, TestReal> const&,
		Box<3, TestReal> const &nb);

	/*
	template
	bool overlaps(Box<4, TestReal> const&,
		Box<4, TestReal> const &nb);

	template
	bool overlaps(Box<5, TestReal> const&,
		Box<5, TestReal> const &nb);
	*/

}

#include "pastel/geometry/overlaps_box_point.h"

namespace Pastel
{

	template
	bool overlaps(
		const Box<1, TestReal>& a,
		const Point<1, TestReal>& nb);

	template
	bool overlaps(
		const Box<2, TestReal>& a,
		const Point<2, TestReal>& nb);

	template
	bool overlaps(
		const Box<3, TestReal>& a,
		const Point<3, TestReal>& nb);

	template
	bool overlaps(
		const Box<4, TestReal>& a,
		const Point<4, TestReal>& nb);

	template
	bool overlaps(
		const Box<5, TestReal>& a,
		const Point<5, TestReal>& nb);

}

#include "pastel/geometry/overlaps_box_sphere.h"

namespace Pastel
{

	template
	bool overlaps(
		const Box<1, TestReal>& a,
		const Sphere<1, TestReal>& nb);

	template
	bool overlaps(
		const Box<2, TestReal>& a,
		const Sphere<2, TestReal>& nb);

	template
	bool overlaps(
		const Box<3, TestReal>& a,
		const Sphere<3, TestReal>& nb);

	template
	bool overlaps(
		const Box<4, TestReal>& a,
		const Sphere<4, TestReal>& nb);

	template
	bool overlaps(
		const Box<5, TestReal>& a,
		const Sphere<5, TestReal>& nb);

}

#include "pastel/geometry/overlaps_box_triangle.h"

namespace Pastel
{

	/*
	template
		bool overlaps(
			const Box<1, TestReal>& a,
			const Triangle<1, TestReal>& nb);
	*/

	template
		bool overlaps(
			const Box<2, TestReal>& a,
			const Triangle<2, TestReal>& nb);

	template
		bool overlaps(
			const Box<3, TestReal>& a,
			const Triangle<3, TestReal>& nb);

}

#include "pastel/geometry/overlaps_plane_box.h"

namespace Pastel
{

	template
	bool overlaps(
		const Plane<1, TestReal>& plane,
		const Box<1, TestReal>& box,
		bool &boxOnPositiveSide);

	template
	bool overlaps(
		const Plane<2, TestReal>& plane,
		const Box<2, TestReal>& box,
		bool &boxOnPositiveSide);

	template
	bool overlaps(
		const Plane<3, TestReal>& plane,
		const Box<3, TestReal>& box,
		bool &boxOnPositiveSide);

	template
	bool overlaps(
		const Plane<4, TestReal>& plane,
		const Box<4, TestReal>& box,
		bool &boxOnPositiveSide);

	template
	bool overlaps(
		const Plane<5, TestReal>& plane,
		const Box<5, TestReal>& box,
		bool &boxOnPositiveSide);

}

#include "pastel/geometry/overlaps_plane_sphere.h"

namespace Pastel
{

	template
		bool overlaps(
			const Plane<1, TestReal>& plane,
			const Sphere<1, TestReal>& sphere);

	template
		bool overlaps(
			const Plane<1, TestReal>& plane,
			const Sphere<1, TestReal>& sphere,
			bool& sphereOnPositiveSide);

	template
		bool overlaps(
			const Plane<2, TestReal>& plane,
			const Sphere<2, TestReal>& sphere);

	template
		bool overlaps(
			const Plane<2, TestReal>& plane,
			const Sphere<2, TestReal>& sphere,
			bool& sphereOnPositiveSide);

	template
		bool overlaps(
			const Plane<3, TestReal>& plane,
			const Sphere<3, TestReal>& sphere);

	template
		bool overlaps(
			const Plane<3, TestReal>& plane,
			const Sphere<3, TestReal>& sphere,
			bool& sphereOnPositiveSide);

	template
		bool overlaps(
			const Plane<4, TestReal>& plane,
			const Sphere<4, TestReal>& sphere);

	template
		bool overlaps(
			const Plane<4, TestReal>& plane,
			const Sphere<4, TestReal>& sphere,
			bool& sphereOnPositiveSide);

	template
		bool overlaps(
			const Plane<5, TestReal>& plane,
			const Sphere<5, TestReal>& sphere);

	template
		bool overlaps(
			const Plane<5, TestReal>& plane,
			const Sphere<5, TestReal>& sphere,
			bool& sphereOnPositiveSide);

}

#include "pastel/geometry/overlaps_plane_triangle.h"

namespace Pastel
{

	template
		bool overlaps(
			const Plane<1, TestReal>& plane,
			const Triangle<1, TestReal>& triangle);

	template
		bool overlaps(
			const Plane<2, TestReal>& plane,
			const Triangle<2, TestReal>& triangle);

	template
		bool overlaps(
			const Plane<3, TestReal>& plane,
			const Triangle<3, TestReal>& triangle);

	template
		bool overlaps(
			const Plane<4, TestReal>& plane,
			const Triangle<4, TestReal>& triangle);

	template
		bool overlaps(
			const Plane<5, TestReal>& plane,
			const Triangle<5, TestReal>& triangle);

}

#include "pastel/geometry/overlaps_segment_segment.h"

namespace Pastel
{

	template
	bool overlaps(
		const Segment<1, TestReal>& aSegment,
		const Segment<1, TestReal>& bSegment);

	template
	bool overlaps(
		const Segment<2, TestReal>& aSegment,
		const Segment<2, TestReal>& bSegment);

}

#include "pastel/geometry/overlaps_sphere_point.h"

namespace Pastel
{

	template
	bool overlaps(const Sphere<1, TestReal>& sphere,
		const Point<1, TestReal>& point);

	template
	bool overlaps(const Sphere<2, TestReal>& sphere,
		const Point<2, TestReal>& point);

	template
	bool overlaps(const Sphere<3, TestReal>& sphere,
		const Point<3, TestReal>& point);

	template
	bool overlaps(const Sphere<4, TestReal>& sphere,
		const Point<4, TestReal>& point);

	template
	bool overlaps(const Sphere<5, TestReal>& sphere,
		const Point<5, TestReal>& point);

}

#include "pastel/geometry/overlaps_sphere_sphere.h"

namespace Pastel
{

	template
		bool overlaps(
			const Sphere<1, TestReal>& aSphere,
			const Sphere<1, TestReal>& bSphere);

	template
		bool overlaps(
			const Sphere<2, TestReal>& aSphere,
			const Sphere<2, TestReal>& bSphere);

	template
		bool overlaps(
			const Sphere<3, TestReal>& aSphere,
			const Sphere<3, TestReal>& bSphere);

	template
		bool overlaps(
			const Sphere<4, TestReal>& aSphere,
			const Sphere<4, TestReal>& bSphere);

	template
		bool overlaps(
			const Sphere<5, TestReal>& aSphere,
			const Sphere<5, TestReal>& bSphere);

}

#include "pastel/geometry/overlaps_triangle_triangle.h"

namespace Pastel
{

	template
	bool overlaps(
		const Triangle<1, TestReal>& aTriangle,
		const Triangle<1, TestReal>& bTriangle);

	template
	bool overlaps(
		const Triangle<2, TestReal>& aTriangle,
		const Triangle<2, TestReal>& bTriangle);

	template
	bool overlaps(
		const Triangle<3, TestReal>& aTriangle,
		const Triangle<3, TestReal>& bTriangle);

}

#include "pastel/geometry/overlaps_flat_plane.h"

namespace Pastel
{

	template
		bool overlaps(
		const Flat<1, TestReal, 2>& aPlane,
		const Plane<1, TestReal>& bPlane);

	template
		bool overlaps(
		const Flat<2, TestReal, 2>& aPlane,
		const Plane<2, TestReal>& bPlane);

	template
		bool overlaps(
		const Flat<3, TestReal, 2>& aPlane,
		const Plane<3, TestReal>& bPlane);

	template
		bool overlaps(
		const Flat<4, TestReal, 2>& aPlane,
		const Plane<4, TestReal>& bPlane);

	template
		bool overlaps(
		const Flat<5, TestReal, 2>& aPlane,
		const Plane<5, TestReal>& bPlane);
}
