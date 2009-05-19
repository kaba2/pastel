#ifndef PASTEL_GJKALIGNEDBOX_H
#define PASTEL_GJKALIGNEDBOX_H

#include "pastel/geometry/gjkshape.h"

namespace Pastel
{

	template <int N, typename Real>
	class GjkAlignedBox
		: public GjkShape<N, Real>
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		GjkAlignedBox()
			: GjkShape<N, Real>()
			, alignedBox_()
		{
		}

		explicit GjkAlignedBox(
			const AlignedBox<N, Real>& alignedBox)
			: GjkShape<N, Real>()
			, alignedBox_(alignedBox)
		{
		}

		virtual ~GjkAlignedBox()
		{
		}

		virtual Point<N, Real> support(const Vector<N, Real>& direction) const
		{
			Vector<N, Real> result = alignedBox_.min();

			for (integer i = 0;i < N;++i)
			{
				if (direction[i] >= 0)
				{
					result[i] = alignedBox_.max()[i];
				}
			}

			return result;
		}

	private:
		AlignedBox<N, Real> alignedBox_;
	};

}

#endif
