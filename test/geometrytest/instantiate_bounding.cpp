#include "pastel/sys/mytypes.h"

#include <vector>

namespace
{

	typedef Pastel::real TestReal;

}

#include "pastel/geometry/bounding_alignedbox.h"

namespace Pastel
{

	template
		AlignedBox<TestReal, 1> boundingAlignedBox(
		const AlignedBox<TestReal, 1>& aAlignedBox,
		const AlignedBox<TestReal, 1>& bAlignedBox);

	template
		AlignedBox<TestReal, 2> boundingAlignedBox(
		const AlignedBox<TestReal, 2>& aAlignedBox,
		const AlignedBox<TestReal, 2>& bAlignedBox);

	template
		AlignedBox<TestReal, 3> boundingAlignedBox(
		const AlignedBox<TestReal, 3>& aAlignedBox,
		const AlignedBox<TestReal, 3>& bAlignedBox);

	template
		AlignedBox<TestReal, 4> boundingAlignedBox(
		const AlignedBox<TestReal, 4>& aAlignedBox,
		const AlignedBox<TestReal, 4>& bAlignedBox);

	template
		AlignedBox<TestReal, 5> boundingAlignedBox(
		const AlignedBox<TestReal, 5>& aAlignedBox,
		const AlignedBox<TestReal, 5>& bAlignedBox);

}

namespace Pastel
{

	template
	AlignedBox<TestReal, 1> boundingAlignedBox(
		const Sphere<TestReal, 1>& sphere);

	template
	AlignedBox<TestReal, 2> boundingAlignedBox(
		const Sphere<TestReal, 2>& sphere);

	template
	AlignedBox<TestReal, 3> boundingAlignedBox(
		const Sphere<TestReal, 3>& sphere);

	template
	AlignedBox<TestReal, 4> boundingAlignedBox(
		const Sphere<TestReal, 4>& sphere);

	template
	AlignedBox<TestReal, 5> boundingAlignedBox(
		const Sphere<TestReal, 5>& sphere);

}

namespace Pastel
{

	template
	AlignedBox<TestReal, 1> boundingAlignedBox(
		const Box<TestReal, 1>& box);

	template
	AlignedBox<TestReal, 2> boundingAlignedBox(
		const Box<TestReal, 2>& box);

	template
	AlignedBox<TestReal, 3> boundingAlignedBox(
		const Box<TestReal, 3>& box);

	template
	AlignedBox<TestReal, 4> boundingAlignedBox(
		const Box<TestReal, 4>& box);

	template
	AlignedBox<TestReal, 5> boundingAlignedBox(
		const Box<TestReal, 5>& box);

}

namespace Pastel
{

	template
	AlignedBox<TestReal, 1> boundingAlignedBox(
		const Segment<TestReal, 1>& segment);

	template
	AlignedBox<TestReal, 2> boundingAlignedBox(
		const Segment<TestReal, 2>& segment);

	template
	AlignedBox<TestReal, 3> boundingAlignedBox(
		const Segment<TestReal, 3>& segment);

	template
	AlignedBox<TestReal, 4> boundingAlignedBox(
		const Segment<TestReal, 4>& segment);

	template
	AlignedBox<TestReal, 5> boundingAlignedBox(
		const Segment<TestReal, 5>& segment);

}

#include "pastel/geometry/bounding_sphere.h"

namespace Pastel
{

	template
		Sphere<TestReal, 1> boundingSphere(
		const std::vector<Vector<TestReal, 1> >::iterator& from,
		const std::vector<Vector<TestReal, 1> >::iterator& to);

	template
		Sphere<TestReal, 2> boundingSphere(
		const std::vector<Vector<TestReal, 2> >::iterator& from,
		const std::vector<Vector<TestReal, 2> >::iterator& to);

	template
		Sphere<TestReal, 3> boundingSphere(
		const std::vector<Vector<TestReal, 3> >::iterator& from,
		const std::vector<Vector<TestReal, 3> >::iterator& to);

	template
		Sphere<TestReal, 4> boundingSphere(
		const std::vector<Vector<TestReal, 4> >::iterator& from,
		const std::vector<Vector<TestReal, 4> >::iterator& to);

	template
		Sphere<TestReal, 5> boundingSphere(
		const std::vector<Vector<TestReal, 5> >::iterator& from,
		const std::vector<Vector<TestReal, 5> >::iterator& to);

}
