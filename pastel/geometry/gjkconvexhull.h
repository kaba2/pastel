#ifndef PASTELGEOMETRY_GJKCONVEXHULL_H
#define PASTELGEOMETRY_GJKCONVEXHULL_H

#include "pastel/geometry/gjkshape.h"

namespace Pastel
{

	template <int N, typename Real>
	class GjkConvexHull
		: public GjkShape<N, Real>
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		GjkConvexHull()
			: GjkShape<N, Real>()
			, aShape_()
			, bShape_()
		{
		}

		GjkConvexHull(
			const Ref& aShape,
			const Ref& bShape)
			: GjkShape<N, Real>()
			, aShape_(aShape)
			, bShape_(bShape)
		{
		}

		virtual ~GjkConvexHull()
		{
		}

		virtual Point<N, Real> support(const Vector<N, Real>& direction) const
		{
			PENSURE(!aShape_->empty() && !bShape_->empty());

			const Point<N, Real> aSupport = aShape_->support(direction);
			const Point<N, Real> bSupport = bShape_->support(direction);

			if (dot(asVector(aSupport)) < dot(asVector(bSupport)))
			{
				return bSupport;
			}

			return aSupport;
		}

	private:
		GjkShape<N, Real>::Ref aShape_;
		GjkShape<N, Real>::Ref bShape_;
	};

}

#endif
