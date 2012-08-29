// Description: Mitchell_Filter class
// Detail: Mitchell-Netravali cubic spline reconstruction filter
// Documentation: filters.txt

#ifndef PASTELDSP_MITCHELL_FILTER_H
#define PASTELDSP_MITCHELL_FILTER_H

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
	Cubic_Filter which is specialized for this case and
	thus more efficient.

	The default	values comes from the
	experiments of Mitchell and Netravali where they
	concluded that the (subjectively) best filters lie
	on the line 2C + B = 1. They further deemed
	(B = 1/3, C = 1/3) as the best from this line.
	*/
	class PASTELDSP Mitchell_Filter
		: public Filter
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		explicit Mitchell_Filter(
			real b = (real)1 / 3,
			real c = (real)1 / 3);
		virtual ~Mitchell_Filter();

		virtual real evaluateInRange(real x) const;

	private:
		Mitchell_Filter(const Mitchell_Filter& that) PASTEL_DELETE;
		Mitchell_Filter& operator=(const Mitchell_Filter& that) PASTEL_DELETE;

		real b_;
		real c_;
	};

	typedef CountedPtr<Mitchell_Filter> MitchellFilterPtr;
	typedef CountedPtr<const Mitchell_Filter> ConstMitchellFilterPtr;

	inline MitchellFilterPtr mitchellFilter(
		real b = (real)1 / 3,
		real c = (real)1 / 3)
	{
		return MitchellFilterPtr(new Mitchell_Filter(b, c));
	}

}

#endif
