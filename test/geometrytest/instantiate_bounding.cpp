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
		AlignedBox<1, TestReal> boundingAlignedBox(
		const AlignedBox<1, TestReal>& aAlignedBox,
		const AlignedBox<1, TestReal>& bAlignedBox);

	template
		AlignedBox<2, TestReal> boundingAlignedBox(
		const AlignedBox<2, TestReal>& aAlignedBox,
		const AlignedBox<2, TestReal>& bAlignedBox);

	template
		AlignedBox<3, TestReal> boundingAlignedBox(
		const AlignedBox<3, TestReal>& aAlignedBox,
		const AlignedBox<3, TestReal>& bAlignedBox);

	template
		AlignedBox<4, TestReal> boundingAlignedBox(
		const AlignedBox<4, TestReal>& aAlignedBox,
		const AlignedBox<4, TestReal>& bAlignedBox);

	template
		AlignedBox<5, TestReal> boundingAlignedBox(
		const AlignedBox<5, TestReal>& aAlignedBox,
		const AlignedBox<5, TestReal>& bAlignedBox);

}

namespace Pastel
{

    template
    AlignedBox<1, TestReal> boundingAlignedBox(
        const Sphere<1, TestReal>& sphere);

    template
    AlignedBox<2, TestReal> boundingAlignedBox(
        const Sphere<2, TestReal>& sphere);

    template
    AlignedBox<3, TestReal> boundingAlignedBox(
        const Sphere<3, TestReal>& sphere);

    template
    AlignedBox<4, TestReal> boundingAlignedBox(
        const Sphere<4, TestReal>& sphere);

    template
    AlignedBox<5, TestReal> boundingAlignedBox(
        const Sphere<5, TestReal>& sphere);

}

namespace Pastel
{

    template 
    AlignedBox<1, TestReal> boundingAlignedBox(
        const Box<1, TestReal>& box);

    template 
    AlignedBox<2, TestReal> boundingAlignedBox(
        const Box<2, TestReal>& box);

    template 
    AlignedBox<3, TestReal> boundingAlignedBox(
        const Box<3, TestReal>& box);

    template 
    AlignedBox<4, TestReal> boundingAlignedBox(
        const Box<4, TestReal>& box);

    template 
    AlignedBox<5, TestReal> boundingAlignedBox(
        const Box<5, TestReal>& box);

}

namespace Pastel
{

    template
    AlignedBox<1, TestReal> boundingAlignedBox(
        const Segment<1, TestReal>& segment);

    template
    AlignedBox<2, TestReal> boundingAlignedBox(
        const Segment<2, TestReal>& segment);

    template
    AlignedBox<3, TestReal> boundingAlignedBox(
        const Segment<3, TestReal>& segment);

    template
    AlignedBox<4, TestReal> boundingAlignedBox(
        const Segment<4, TestReal>& segment);

    template
    AlignedBox<5, TestReal> boundingAlignedBox(
        const Segment<5, TestReal>& segment);

}

#include "pastel/geometry/bounding_sphere.h"

namespace Pastel
{

	template
		Sphere<1, TestReal> boundingSphere(
		const std::vector<Point<1, TestReal> >::iterator& from,
		const std::vector<Point<1, TestReal> >::iterator& to);

	template
		Sphere<2, TestReal> boundingSphere(
		const std::vector<Point<2, TestReal> >::iterator& from,
		const std::vector<Point<2, TestReal> >::iterator& to);

	template
		Sphere<3, TestReal> boundingSphere(
		const std::vector<Point<3, TestReal> >::iterator& from,
		const std::vector<Point<3, TestReal> >::iterator& to);

	template
		Sphere<4, TestReal> boundingSphere(
		const std::vector<Point<4, TestReal> >::iterator& from,
		const std::vector<Point<4, TestReal> >::iterator& to);

	template
		Sphere<5, TestReal> boundingSphere(
		const std::vector<Point<5, TestReal> >::iterator& from,
		const std::vector<Point<5, TestReal> >::iterator& to);

}
