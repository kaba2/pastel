#ifndef PASTELGEOMETRY_GJKAFFINETRANSFORMATION_H
#define PASTELGEOMETRY_GJKAFFINETRANSFORMATION_H

#include "pastel/geometry/gjkshape.h"

#include "pastel/math/affinetransformation.h"

namespace Pastel
{

	template <int N, typename Real>
	class GjkAffineTransformation
		: public GjkShape<N, Real>
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		GjkAffineTransformation()
			: GjkShape<N, Real>()
			, shape_()
			, transformation_()
		{
		}

		explicit GjkAffineTransformation(
			const GjkShape<N, Real>::Ptr& shape,
			const AffineTransformation<N, Real>& transformation)
			: GjkShape<N, Real>()
			, shape_(shape)
			, transformation_(transformation)
		{
		}

		virtual ~GjkAffineTransformation()
		{
		}

		virtual Point<N, Real> support(const Vector<N, Real>& direction) const
		{
			return shape_->support(transformation_.transformation() * direction) * transformation_;
		}

	private:
		GjkShape<N, Real>::Ptr shape_;
		AffineTransformation<N, Real> transformation_;
	};

}

#endif
