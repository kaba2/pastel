#ifndef PASTELGEOMETRY_GJKMINKOWSKISUM_H
#define PASTELGEOMETRY_GJKMINKOWSKISUM_H

#include "pastel/geometry/gjkshape.h"

namespace Pastel
{

	template <int N, typename Real>
	class GjkMinkowskiSum
		: public GjkShape<N, Real>
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		GjkMinkowskiSum()
			: GjkShape<N, Real>()
			, aShape_()
			, bShape_()
		{
		}

		GjkMinkowskiSum(
			const Ref& aShape,
			const Ref& bShape)
			: GjkShape<N, Real>()
			, aShape_(aShape)
			, bShape_(bShape)
		{
		}

		virtual ~GjkMinkowskiSum()
		{
		}

		virtual Point<N, Real> support(const Vector<N, Real>& direction) const
		{
			PENSURE(!aShape_->empty() && !bShape_->empty());

			return aShape_->support(direction) +
				asVector(bShape_->support(direction));
		}

	private:
		GjkShape<N, Real>::Ref aShape_;
		GjkShape<N, Real>::Ref bShape_;
	};

}

#endif
