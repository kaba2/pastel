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
		integer dimension,
		const InputIterator& from,
		const InputIterator& to)
	{
		PENSURE1((N == Dynamic && dimension > 0) ||
			(N != Dynamic && dimension == N), dimension);

		AlignedBox<N, Real> result(dimension);

		if (from != to)
		{
			PENSURE2(dimension == from->dimension(),
				dimension, from->dimension());

			InputIterator iter(from);
			while (iter != to)
			{
				extendToCover(*iter, result);

				++iter;
			}
		}

		return result;
	}

	template <int N, typename Real>
	AlignedBox<N, Real> boundingAlignedBox(
		const AlignedBox<N, Real>& aAlignedBox, 
		const AlignedBox<N, Real>& bAlignedBox)
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
		const integer dimension = box.dimension();

		const Vector<N, Real>& width = box.width();
		Vector<N, Real> radius(ofDimension(dimension));

		for (integer i = 0;i < dimension;++i)
		{
			Real axisRadius = 0;
			for (integer k = 0;k < dimension;++k)
			{
				// Actually, this is
				// mabs(dot(box.rotation()[k] * width[k], e_i))
				// where e_i is the i:th standard basis vector.
				axisRadius += mabs(box.rotation()(k, i) * width[k]);
			}

			radius[i] = axisRadius;
		}

		return AlignedBox<N, Real>(
			box.position() - radius, 
			box.position() + radius);
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
		Point<N, Real> minPoint = center - maxRadius;
		Point<N, Real> maxPoint = center + maxRadius;

		return AlignedBox<N, Real>(
			min(minPoint, box.min()),
			max(maxPoint, box.max()));
	}

	template <int N, typename Real>
	void extendToCover(
		const AlignedBox<N, Real>& boxToCover,
		AlignedBox<N, Real>& boxToExtend)
	{
		Pastel::extendToCover(boxToCover.min(),
			boxToExtend);
		Pastel::extendToCover(boxToCover.max(),
			boxToExtend);
	}

	template <int N, typename Real>
	void extendToCover(
		const Point<N, Real>& pointToCover,
		AlignedBox<N, Real>& boxToExtend)
	{
		const integer dimension = pointToCover.size();
		PENSURE2(dimension == boxToExtend.dimension(),
			dimension, boxToExtend.dimension());

		Point<N, Real>& min = boxToExtend.min();
		Point<N, Real>& max = boxToExtend.max();

		for (integer i = 0;i < dimension;++i)
		{
			if (pointToCover[i] < min[i])
			{
				min[i] = pointToCover[i];
			}
			else if (pointToCover[i] > max[i])
			{
				max[i] = pointToCover[i];
			}
		}
	}

}

#endif
