#ifndef PASTEL_MITCHELL_FILTER_H
#define PASTEL_MITCHELL_FILTER_H

#include "pastel/dsp/dsplibrary.h"
#include "pastel/dsp/filter.h"

namespace Pastel
{

	//! A Mitchell-Netravali cubic spline reconstruction filter.
	/*
	For information on this filter, see:
	"Reconstruction Filters in Computer Graphics",
	Don P. Mitchell, Arun N. Netravali, 1988.

	This filter is interpolatory if and only if B = 0.
	However, if you want interpolation, then consider using
	CubicFilter which is specialized for this case and
	thus more efficient.

	The default	values comes from the
	experiments of Mitchell and Netravali where they
	concluded that the (subjectively) best filters lie
	on the line 2C + B = 1. They further deemed
	(B = 1/3, C = 1/3) as the best from this line.
	*/
	class PASTELDSP MitchellFilter
		: public Filter
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		explicit MitchellFilter(
			real b = (real)1 / 3,
			real c = (real)1 / 3);
		virtual ~MitchellFilter();

		virtual real evaluateInRange(real x) const;

	private:
		// Prohibited
		MitchellFilter(const MitchellFilter& that);
		// Prohibited
		MitchellFilter& operator=(const MitchellFilter& that);

		real b_;
		real c_;
	};

	typedef CountedPtr<MitchellFilter> MitchellFilterPtr;
	typedef CountedPtr<const MitchellFilter> ConstMitchellFilterPtr;

	inline MitchellFilterPtr mitchellFilter(
		real b = (real)1 / 3,
		real c = (real)1 / 3)
	{
		return MitchellFilterPtr(new MitchellFilter(b, c));
	}

}

#endif
