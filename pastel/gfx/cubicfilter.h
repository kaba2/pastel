#ifndef PASTELGFX_CUBICFILTER_H
#define PASTELGFX_CUBICFILTER_H

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
	MitchellFilter(0, negativeLobeness * (3 / 2)). However,
	this implementation is more efficient. 
	
	The default	value for 'negativeLobeness' comes from the
	experiments of Mitchell and Netravali where they
	concluded that the (subjectively) best filters lie 
	on the line 2C + B = 1.	Here B = 0 and C = negativeLobeness * (3 / 2),
	from which the result follows. Coincidentally, this default 
	parameter gives a filter that is almost identical to LanczosFilter(2).
	*/	
	class PASTELGFX CubicFilter
		: public Filter
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		explicit CubicFilter(real negativeLobeness = (real)1 / 3);
		virtual ~CubicFilter();

		virtual real operator()(real position) const;
		virtual real radius() const;

	private:
		real d_;
	};

	typedef CountedPtr<CubicFilter> CubicFilterRef;
	typedef CountedPtr<const CubicFilter> ConstCubicFilterRef;

	inline CubicFilterRef cubicFilter(real negativeLobeness = (real)1 / 3)
	{
		return CubicFilterRef(new CubicFilter(negativeLobeness));
	}

}

#endif
