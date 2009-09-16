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
		const AlignedBox<TestReal, 1>& a,
		const AlignedBox<TestReal, 1>& b);

	template
		bool overlaps(
		const AlignedBox<TestReal, 2>& a,
		const AlignedBox<TestReal, 2>& b);

	template
		bool overlaps(
		const AlignedBox<TestReal, 3>& a,
		const AlignedBox<TestReal, 3>& b);

	template
		bool overlaps(
		const AlignedBox<TestReal, 4>& a,
		const AlignedBox<TestReal, 4>& b);

	template
		bool overlaps(
		const AlignedBox<TestReal, 5>& a,
		const AlignedBox<TestReal, 5>& b);

}

#include "pastel/geometry/overlaps_alignedbox_box.h"

namespace Pastel
{

	/*
	template
	bool overlaps(
		const AlignedBox<TestReal, 1>& a,
		const Box<TestReal, 1>& b);
	*/

	template
	bool overlaps(
		const AlignedBox<TestReal, 2>& a,
		const Box<TestReal, 2>& b);

	template
	bool overlaps(
		const AlignedBox<TestReal, 3>& a,
		const Box<TestReal, 3>& b);

	/*
	template
	bool overlaps(
		const AlignedBox<TestReal, 4>& a,
		const Box<TestReal, 4>& b);

	template
	bool overlaps(
		const AlignedBox<TestReal, 5>& a,
		const Box<TestReal, 5>& b);
	*/

}

#include "pastel/geometry/overlaps_alignedbox_plane.h"

namespace Pastel
{

	template
		bool overlaps(
			const AlignedBox<TestReal, 1>& alignedBox,
			const Plane<TestReal, 1>& plane);

	template
		bool overlaps(
			const AlignedBox<TestReal, 2>& alignedBox,
			const Plane<TestReal, 2>& plane);

	template
		bool overlaps(
			const AlignedBox<TestReal, 3>& alignedBox,
			const Plane<TestReal, 3>& plane);

	template
		bool overlaps(
			const AlignedBox<TestReal, 4>& alignedBox,
			const Plane<TestReal, 4>& plane);

	template
		bool overlaps(
			const AlignedBox<TestReal, 5>& alignedBox,
			const Plane<TestReal, 5>& plane);

}

#include "pastel/geometry/overlaps_alignedbox_point.h"

namespace Pastel
{

	template
		bool overlaps(
			const AlignedBox<TestReal, 1>& alignedBox,
			const Vector<TestReal, 1>& point);

	template
		bool overlaps(
			const AlignedBox<TestReal, 2>& alignedBox,
			const Vector<TestReal, 2>& point);

	template
		bool overlaps(
			const AlignedBox<TestReal, 3>& alignedBox,
			const Vector<TestReal, 3>& point);

	template
		bool overlaps(
			const AlignedBox<TestReal, 4>& alignedBox,
			const Vector<TestReal, 4>& point);

	template
		bool overlaps(
			const AlignedBox<TestReal, 5>& alignedBox,
			const Vector<TestReal, 5>& point);

}

#include "pastel/geometry/overlaps_alignedbox_sphere.h"

namespace Pastel
{

	template
	bool overlaps(
		const AlignedBox<TestReal, 1>& alignedBox,
		const Sphere<TestReal, 1>& sphere);

	template
	bool overlaps(
		const AlignedBox<TestReal, 2>& alignedBox,
		const Sphere<TestReal, 2>& sphere);

	template
	bool overlaps(
		const AlignedBox<TestReal, 3>& alignedBox,
		const Sphere<TestReal, 3>& sphere);

	template
	bool overlaps(
		const AlignedBox<TestReal, 4>& alignedBox,
		const Sphere<TestReal, 4>& sphere);

	template
	bool overlaps(
		const AlignedBox<TestReal, 5>& alignedBox,
		const Sphere<TestReal, 5>& sphere);

}

#include "pastel/geometry/overlaps_alignedbox_triangle.h"

namespace Pastel
{

	/*
	template
	bool overlaps(
		const AlignedBox<TestReal, 1>& alignedBox,
		const Triangle<TestReal, 1>& triangle);
	*/

	template
	bool overlaps(
		const AlignedBox<TestReal, 2>& alignedBox,
		const Triangle<TestReal, 2>& triangle);

	template
	bool overlaps(
		const AlignedBox<TestReal, 3>& alignedBox,
		const Triangle<TestReal, 3>& triangle);

	/*
	template
	bool overlaps(
		const AlignedBox<TestReal, 4>& alignedBox,
		const Triangle<TestReal, 4>& triangle);

	template
	bool overlaps(
		const AlignedBox<TestReal, 5>& alignedBox,
		const Triangle<TestReal, 5>& triangle);
	*/

}

#include "pastel/geometry/overlaps_alignedplane_alignedbox.h"

namespace Pastel
{

	template
		bool overlaps(AlignedPlane<TestReal, 1> const &alignedPlane,
			AlignedBox<TestReal, 1> const &alignedBox);

	template
		bool overlaps(AlignedPlane<TestReal, 1> const &alignedPlane,
			AlignedBox<TestReal, 1> const &alignedBox,
			bool &greater);

	template
		bool overlaps(AlignedPlane<TestReal, 2> const &alignedPlane,
			AlignedBox<TestReal, 2> const &alignedBox);

	template
		bool overlaps(AlignedPlane<TestReal, 2> const &alignedPlane,
			AlignedBox<TestReal, 2> const &alignedBox,
			bool &greater);

	template
		bool overlaps(AlignedPlane<TestReal, 3> const &alignedPlane,
			AlignedBox<TestReal, 3> const &alignedBox);

	template
		bool overlaps(AlignedPlane<TestReal, 3> const &alignedPlane,
			AlignedBox<TestReal, 3> const &alignedBox,
			bool &greater);

	template
		bool overlaps(AlignedPlane<TestReal, 4> const &alignedPlane,
			AlignedBox<TestReal, 4> const &alignedBox);

	template
		bool overlaps(AlignedPlane<TestReal, 4> const &alignedPlane,
			AlignedBox<TestReal, 4> const &alignedBox,
			bool &greater);

	template
		bool overlaps(AlignedPlane<TestReal, 5> const &alignedPlane,
			AlignedBox<TestReal, 5> const &alignedBox);

	template
		bool overlaps(AlignedPlane<TestReal, 5> const &alignedPlane,
			AlignedBox<TestReal, 5> const &alignedBox,
			bool &greater);

}

#include "pastel/geometry/overlaps_alignedplane_box.h"

namespace Pastel
{

	template
	bool overlaps(AlignedPlane<TestReal, 1> const &alignedPlane,
		Box<TestReal, 1> const &box);

	template
		bool overlaps(AlignedPlane<TestReal, 1> const &alignedPlane,
			Box<TestReal, 1> const &box,
			bool &);

	template
	bool overlaps(AlignedPlane<TestReal, 2> const &alignedPlane,
		Box<TestReal, 2> const &box);

	template
		bool overlaps(AlignedPlane<TestReal, 2> const &alignedPlane,
			Box<TestReal, 2> const &box,
			bool &);

	template
	bool overlaps(AlignedPlane<TestReal, 3> const &alignedPlane,
		Box<TestReal, 3> const &box);

	template
		bool overlaps(AlignedPlane<TestReal, 3> const &alignedPlane,
			Box<TestReal, 3> const &box,
			bool &);

	template
	bool overlaps(AlignedPlane<TestReal, 4> const &alignedPlane,
		Box<TestReal, 4> const &box);

	template
		bool overlaps(AlignedPlane<TestReal, 4> const &alignedPlane,
			Box<TestReal, 4> const &box,
			bool &);

	template
	bool overlaps(AlignedPlane<TestReal, 5> const &alignedPlane,
		Box<TestReal, 5> const &box);

	template
		bool overlaps(AlignedPlane<TestReal, 5> const &alignedPlane,
			Box<TestReal, 5> const &box,
			bool &);

}

#include "pastel/geometry/overlaps_alignedplane_sphere.h"

namespace Pastel
{

	template
	bool overlaps(AlignedPlane<TestReal, 1> const &alignedPlane,
		Sphere<TestReal, 1> const &sphere);

	template
	bool overlaps(AlignedPlane<TestReal, 1> const &alignedPlane,
		Sphere<TestReal, 1> const &sphere,
		bool &alignedPlaneOnPositiveSide);

	template
	bool overlaps(AlignedPlane<TestReal, 2> const &alignedPlane,
		Sphere<TestReal, 2> const &sphere);

	template
	bool overlaps(AlignedPlane<TestReal, 2> const &alignedPlane,
		Sphere<TestReal, 2> const &sphere,
		bool &alignedPlaneOnPositiveSide);

	template
	bool overlaps(AlignedPlane<TestReal, 3> const &alignedPlane,
		Sphere<TestReal, 3> const &sphere);

	template
	bool overlaps(AlignedPlane<TestReal, 3> const &alignedPlane,
		Sphere<TestReal, 3> const &sphere,
		bool &alignedPlaneOnPositiveSide);

	template
	bool overlaps(AlignedPlane<TestReal, 4> const &alignedPlane,
		Sphere<TestReal, 4> const &sphere);

	template
	bool overlaps(AlignedPlane<TestReal, 4> const &alignedPlane,
		Sphere<TestReal, 4> const &sphere,
		bool &alignedPlaneOnPositiveSide);

	template
	bool overlaps(AlignedPlane<TestReal, 5> const &alignedPlane,
		Sphere<TestReal, 5> const &sphere);

	template
	bool overlaps(AlignedPlane<TestReal, 5> const &alignedPlane,
		Sphere<TestReal, 5> const &sphere,
		bool &alignedPlaneOnPositiveSide);

}

#include "pastel/geometry/overlaps_alignedplane_triangle.h"

namespace Pastel
{

	template
		bool overlaps(const AlignedPlane<TestReal, 1>& alignedPlane,
			const Triangle<TestReal, 1>& triangle);

	template
		bool overlaps(const AlignedPlane<TestReal, 1>& alignedPlane,
			const Triangle<TestReal, 1>& triangle,
			bool& alignedPlaneOnPositiveSide);

	template
		bool overlaps(const AlignedPlane<TestReal, 2>& alignedPlane,
			const Triangle<TestReal, 2>& triangle);

	template
		bool overlaps(const AlignedPlane<TestReal, 2>& alignedPlane,
			const Triangle<TestReal, 2>& triangle,
			bool& alignedPlaneOnPositiveSide);

	template
		bool overlaps(const AlignedPlane<TestReal, 3>& alignedPlane,
			const Triangle<TestReal, 3>& triangle);

	template
		bool overlaps(const AlignedPlane<TestReal, 3>& alignedPlane,
			const Triangle<TestReal, 3>& triangle,
			bool& alignedPlaneOnPositiveSide);

	template
		bool overlaps(const AlignedPlane<TestReal, 4>& alignedPlane,
			const Triangle<TestReal, 4>& triangle);

	template
		bool overlaps(const AlignedPlane<TestReal, 4>& alignedPlane,
			const Triangle<TestReal, 4>& triangle,
			bool& alignedPlaneOnPositiveSide);

	template
		bool overlaps(const AlignedPlane<TestReal, 5>& alignedPlane,
			const Triangle<TestReal, 5>& triangle);

	template
		bool overlaps(const AlignedPlane<TestReal, 5>& alignedPlane,
			const Triangle<TestReal, 5>& triangle,
			bool& alignedPlaneOnPositiveSide);

}

#include "pastel/geometry/overlaps_box_box.h"

namespace Pastel
{

	/*
	template
	bool overlaps(Box<TestReal, 1> const&,
		Box<TestReal, 1> const &nb);
	*/

	template
	bool overlaps(Box<TestReal, 2> const&,
		Box<TestReal, 2> const &nb);

	template
	bool overlaps(Box<TestReal, 3> const&,
		Box<TestReal, 3> const &nb);

	/*
	template
	bool overlaps(Box<TestReal, 4> const&,
		Box<TestReal, 4> const &nb);

	template
	bool overlaps(Box<TestReal, 5> const&,
		Box<TestReal, 5> const &nb);
	*/

}

#include "pastel/geometry/overlaps_box_point.h"

namespace Pastel
{

	template
	bool overlaps(
		const Box<TestReal, 1>& a,
		const Vector<TestReal, 1>& nb);

	template
	bool overlaps(
		const Box<TestReal, 2>& a,
		const Vector<TestReal, 2>& nb);

	template
	bool overlaps(
		const Box<TestReal, 3>& a,
		const Vector<TestReal, 3>& nb);

	template
	bool overlaps(
		const Box<TestReal, 4>& a,
		const Vector<TestReal, 4>& nb);

	template
	bool overlaps(
		const Box<TestReal, 5>& a,
		const Vector<TestReal, 5>& nb);

}

#include "pastel/geometry/overlaps_box_sphere.h"

namespace Pastel
{

	template
	bool overlaps(
		const Box<TestReal, 1>& a,
		const Sphere<TestReal, 1>& nb);

	template
	bool overlaps(
		const Box<TestReal, 2>& a,
		const Sphere<TestReal, 2>& nb);

	template
	bool overlaps(
		const Box<TestReal, 3>& a,
		const Sphere<TestReal, 3>& nb);

	template
	bool overlaps(
		const Box<TestReal, 4>& a,
		const Sphere<TestReal, 4>& nb);

	template
	bool overlaps(
		const Box<TestReal, 5>& a,
		const Sphere<TestReal, 5>& nb);

}

#include "pastel/geometry/overlaps_box_triangle.h"

namespace Pastel
{

	/*
	template
		bool overlaps(
			const Box<TestReal, 1>& a,
			const Triangle<TestReal, 1>& nb);
	*/

	template
		bool overlaps(
			const Box<TestReal, 2>& a,
			const Triangle<TestReal, 2>& nb);

	template
		bool overlaps(
			const Box<TestReal, 3>& a,
			const Triangle<TestReal, 3>& nb);

}

#include "pastel/geometry/overlaps_plane_box.h"

namespace Pastel
{

	template
	bool overlaps(
		const Plane<TestReal, 1>& plane,
		const Box<TestReal, 1>& box,
		bool &boxOnPositiveSide);

	template
	bool overlaps(
		const Plane<TestReal, 2>& plane,
		const Box<TestReal, 2>& box,
		bool &boxOnPositiveSide);

	template
	bool overlaps(
		const Plane<TestReal, 3>& plane,
		const Box<TestReal, 3>& box,
		bool &boxOnPositiveSide);

	template
	bool overlaps(
		const Plane<TestReal, 4>& plane,
		const Box<TestReal, 4>& box,
		bool &boxOnPositiveSide);

	template
	bool overlaps(
		const Plane<TestReal, 5>& plane,
		const Box<TestReal, 5>& box,
		bool &boxOnPositiveSide);

}

#include "pastel/geometry/overlaps_plane_sphere.h"

namespace Pastel
{

	template
		bool overlaps(
			const Plane<TestReal, 1>& plane,
			const Sphere<TestReal, 1>& sphere);

	template
		bool overlaps(
			const Plane<TestReal, 1>& plane,
			const Sphere<TestReal, 1>& sphere,
			bool& sphereOnPositiveSide);

	template
		bool overlaps(
			const Plane<TestReal, 2>& plane,
			const Sphere<TestReal, 2>& sphere);

	template
		bool overlaps(
			const Plane<TestReal, 2>& plane,
			const Sphere<TestReal, 2>& sphere,
			bool& sphereOnPositiveSide);

	template
		bool overlaps(
			const Plane<TestReal, 3>& plane,
			const Sphere<TestReal, 3>& sphere);

	template
		bool overlaps(
			const Plane<TestReal, 3>& plane,
			const Sphere<TestReal, 3>& sphere,
			bool& sphereOnPositiveSide);

	template
		bool overlaps(
			const Plane<TestReal, 4>& plane,
			const Sphere<TestReal, 4>& sphere);

	template
		bool overlaps(
			const Plane<TestReal, 4>& plane,
			const Sphere<TestReal, 4>& sphere,
			bool& sphereOnPositiveSide);

	template
		bool overlaps(
			const Plane<TestReal, 5>& plane,
			const Sphere<TestReal, 5>& sphere);

	template
		bool overlaps(
			const Plane<TestReal, 5>& plane,
			const Sphere<TestReal, 5>& sphere,
			bool& sphereOnPositiveSide);

}

#include "pastel/geometry/overlaps_plane_triangle.h"

namespace Pastel
{

	template
		bool overlaps(
			const Plane<TestReal, 1>& plane,
			const Triangle<TestReal, 1>& triangle);

	template
		bool overlaps(
			const Plane<TestReal, 2>& plane,
			const Triangle<TestReal, 2>& triangle);

	template
		bool overlaps(
			const Plane<TestReal, 3>& plane,
			const Triangle<TestReal, 3>& triangle);

	template
		bool overlaps(
			const Plane<TestReal, 4>& plane,
			const Triangle<TestReal, 4>& triangle);

	template
		bool overlaps(
			const Plane<TestReal, 5>& plane,
			const Triangle<TestReal, 5>& triangle);

}

#include "pastel/geometry/overlaps_segment_segment.h"

namespace Pastel
{

	template
	bool overlaps(
		const Segment<TestReal, 1>& aSegment,
		const Segment<TestReal, 1>& bSegment);

	template
	bool overlaps(
		const Segment<TestReal, 2>& aSegment,
		const Segment<TestReal, 2>& bSegment);

}

#include "pastel/geometry/overlaps_sphere_point.h"

namespace Pastel
{

	template
	bool overlaps(const Sphere<TestReal, 1>& sphere,
		const Vector<TestReal, 1>& point);

	template
	bool overlaps(const Sphere<TestReal, 2>& sphere,
		const Vector<TestReal, 2>& point);

	template
	bool overlaps(const Sphere<TestReal, 3>& sphere,
		const Vector<TestReal, 3>& point);

	template
	bool overlaps(const Sphere<TestReal, 4>& sphere,
		const Vector<TestReal, 4>& point);

	template
	bool overlaps(const Sphere<TestReal, 5>& sphere,
		const Vector<TestReal, 5>& point);

}

#include "pastel/geometry/overlaps_sphere_sphere.h"

namespace Pastel
{

	template
		bool overlaps(
			const Sphere<TestReal, 1>& aSphere,
			const Sphere<TestReal, 1>& bSphere);

	template
		bool overlaps(
			const Sphere<TestReal, 2>& aSphere,
			const Sphere<TestReal, 2>& bSphere);

	template
		bool overlaps(
			const Sphere<TestReal, 3>& aSphere,
			const Sphere<TestReal, 3>& bSphere);

	template
		bool overlaps(
			const Sphere<TestReal, 4>& aSphere,
			const Sphere<TestReal, 4>& bSphere);

	template
		bool overlaps(
			const Sphere<TestReal, 5>& aSphere,
			const Sphere<TestReal, 5>& bSphere);

}

#include "pastel/geometry/overlaps_triangle_triangle.h"

namespace Pastel
{

	template
	bool overlaps(
		const Triangle<TestReal, 1>& aTriangle,
		const Triangle<TestReal, 1>& bTriangle);

	template
	bool overlaps(
		const Triangle<TestReal, 2>& aTriangle,
		const Triangle<TestReal, 2>& bTriangle);

	template
	bool overlaps(
		const Triangle<TestReal, 3>& aTriangle,
		const Triangle<TestReal, 3>& bTriangle);

}

#include "pastel/geometry/overlaps_flat_plane.h"

namespace Pastel
{

	template
		bool overlaps(
		const Flat<TestReal, 1, 2>& aPlane,
		const Plane<TestReal, 1>& bPlane);

	template
		bool overlaps(
		const Flat<TestReal, 2, 2>& aPlane,
		const Plane<TestReal, 2>& bPlane);

	template
		bool overlaps(
		const Flat<TestReal, 3, 2>& aPlane,
		const Plane<TestReal, 3>& bPlane);

	template
		bool overlaps(
		const Flat<TestReal, 4, 2>& aPlane,
		const Plane<TestReal, 4>& bPlane);

	template
		bool overlaps(
		const Flat<TestReal, 5, 2>& aPlane,
		const Plane<TestReal, 5>& bPlane);
}
