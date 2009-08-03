#ifndef PASTEL_SWEEP_HPP
#define PASTEL_SWEEP_HPP

#include "pastel/geometry/sweep.h"
#include "pastel/geometry/antipodal.h"

#include "pastel/math/affinetransformation_tools.h"

namespace Pastel
{

	template <typename Real>
	void orientedSweep(
		const std::vector<Point<Real, 2> >& pen,
		const Segment<2, Real>& segment,
		std::vector<Point<Real, 2> >& result)
	{
		const integer points = pen.size();

		const Vector<Real, 2> delta = segment.end() - segment.start();
		if (delta == Vector<Real, 2>(0))
		{
			std::vector<Point<Real, 2> > sweepResult(pen);
			sweepResult.swap(result);
			return;
		}

		const AffineTransformation2 transformation(
			rotation2<Real>(ccwAngle(delta)));

		std::vector<Point<Real, 2> > orientedPen;
		orientedPen.reserve(points);

		for (integer i = 0;i < points;++i)
		{
			orientedPen.push_back(pen[i] * transformation);
		}

		sweep(orientedPen, segment, result);
	}

	template <typename Real>
	void sweep(
		const std::vector<Point<Real, 2> >& pen,
		const Segment<2, Real>& segment,
		std::vector<Point<Real, 2> >& result)
	{
		const Vector<Real, 2> delta = segment.end() - segment.start();

		if (delta == Vector<Real, 2>(0))
		{
			std::vector<Point<Real, 2> > sweepResult(pen);
			sweepResult.swap(result);
			return;
		}

		const Integer2 breakPoint = antipodal(pen, delta);

		const integer points = pen.size();

		std::vector<Point<Real, 2> > sweepResult;
		sweepResult.reserve(points + 2);

		integer i = breakPoint[0];
		while(i != breakPoint[1])
		{
			sweepResult.push_back(segment.start() + asVector(pen[i]));

			++i;
			if (i == points)
			{
				i = 0;
			}
		}

		sweepResult.push_back(segment.start() + asVector(pen[i]));

		while(i != breakPoint[0])
		{
			sweepResult.push_back(segment.end() + asVector(pen[i]));

			++i;
			if (i == points)
			{
				i = 0;
			}
		}

		sweepResult.push_back(segment.end() + asVector(pen[i]));

		sweepResult.swap(result);
	}

}

#endif
