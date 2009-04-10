#ifndef PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_HPP
#define PASTELGEOMETRY_BOUNDING_ALIGNEDBOX_HPP

#include "pastel/geometry/bounding_alignedbox.h"

#include "pastel/sys/point.h"
#include "pastel/sys/point_tools.h"

#include "pastel/sys/constants.h"

namespace Pastel
{

	template <int N, typename Real, typename InputIterator>
	AlignedBox<N, Real> boundingAlignedBox(
		const InputIterator& from,
		const InputIterator& to)
	{
		Point<N, Real> currentMin(infinity<Real>());
		Point<N, Real> currentMax(-infinity<Real>());

		InputIterator iter(from);
		while (iter != to)
		{
			currentMin = asPoint(min(asVector(*iter), asVector(currentMin)));
			currentMax = asPoint(max(asVector(*iter), asVector(currentMax)));

			++iter;
		}

		return AlignedBox<N, Real>(currentMin, currentMax);
	}

	template <int N, typename Real>
	AlignedBox<N, Real> boundingAlignedBox(
		const AlignedBox<N, Real>& aAlignedBox, const AlignedBox<N, Real>& bAlignedBox)
	{
		return AlignedBox<N, Real>(
			min(aAlignedBox.min(), bAlignedBox.min()),
			max(aAlignedBox.max(), bAlignedBox.max()));
	}

	template <int N, typename Real>
	AlignedBox<N, Real> boundingAlignedBox(
		const Sphere<N, Real>& sphere)
	{
		return AlignedBox<N, Real>(
			sphere.position() - Vector<N, Real>(sphere.radius()),
			sphere.position() + Vector<N, Real>(sphere.radius()));
	}

	template <int N, typename Real>
	AlignedBox<N, Real> boundingAlignedBox(
		const Box<N, Real>& box)
	{
		const Vector<N, Real>& width = box.width();
		Vector<N, Real> radius;

		for (integer i = 0;i < N;++i)
		{
			Real axisRadius = 0;
			for (integer k = 0;k < N;++k)
			{
				// Actually, this is
				// std::abs(dot(box.rotation()[k] * width[k], e_i))
				// where e_i is the i:th standard basis vector.
				axisRadius += std::abs(box.rotation()(k, i) * width[k]);
			}

			radius[i] = axisRadius;
		}

		return AlignedBox<N, Real>(
			box.position() - radius, box.position() + radius);
	}

	template <int N, typename Real>
	AlignedBox<N, Real> boundingAlignedBox(
		const Segment<N, Real>& segment)
	{
		return AlignedBox<N, Real>(
			min(segment.start(), segment.end()),
			max(segment.start(), segment.end()));
	}

	template <int N, typename Real, int M>
	AlignedBox<N, Real> boundingAlignedBox(
		const Simplex<N, Real, M>& simplex)
	{
		return Pastel::boundingAlignedBox<N, Real>(
			simplex.begin(), simplex.end());
	}

	template <int N, typename Real>
	AlignedBox<N, Real> boundingAlignedCube(
		const AlignedBox<N, Real>& box)
	{
		const Real maxRadius = max(box.extent()) * 0.5;
		const Point<N, Real> center = linear(box.min(), box.max(), 0.5);
		return AlignedBox<N, Real>(center - maxRadius, center + maxRadius);
	}

}

#endif
