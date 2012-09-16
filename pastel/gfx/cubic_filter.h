// Description: Cubic_Filter class
// Detail: Cardinal cubic spline reconstruction filter
// Documentation: filters.txt

#ifndef PASTELGFX_CUBIC_FILTER_H
#define PASTELGFX_CUBIC_FILTER_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/filter.h"

namespace Pastel
{

	//! A cardinal cubic spline reconstruction filter.
	/*!
	Strictly speaking, the filter is a proper reconstruction
	filter only if the values of 'negativeLobeness' are
	in the range [0, 1]. However, we don't restrict the
	usage. This filter is interpolatory. It is equivalent to
	Mitchell_Filter(0, negativeLobeness * (3 / 2)). However,
	this implementation is more efficient.

	The default	value for 'negativeLobeness' comes from the
	experiments of Mitchell and Netravali where they
	concluded that the (subjectively) best filters lie
	on the line 2C + B = 1.	Here B = 0 and C = negativeLobeness * (3 / 2),
	from which the result follows. Coincidentally, this default
	parameter gives a filter that is almost identical to Lanczos_Filter(2).
	*/
	class PASTELGFX Cubic_Filter
		: public Filter
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		explicit Cubic_Filter(real negativeLobeness = (real)1 / 3);
		virtual ~Cubic_Filter();

		virtual real evaluateInRange(real x) const;

	private:
		Cubic_Filter(const Cubic_Filter& that) PASTEL_DELETE;
		Cubic_Filter& operator=(const Cubic_Filter& that) PASTEL_DELETE;

		real d_;
	};

	typedef std::shared_ptr<Cubic_Filter> CubicFilterPtr;
	typedef std::shared_ptr<const Cubic_Filter> ConstCubicFilterPtr;

	inline CubicFilterPtr cubicFilter(real negativeLobeness = (real)1 / 3)
	{
		return CubicFilterPtr(new Cubic_Filter(negativeLobeness));
	}

}

#endif