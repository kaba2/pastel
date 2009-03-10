#ifndef PASTELGFX_MITCHELL_FILTER_H
#define PASTELGFX_MITCHELL_FILTER_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/filter.h"

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
	class PASTELGFX MitchellFilter
		: public Filter
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		explicit MitchellFilter(
			real b = (real)1 / 3,
			real c = (real)1 / 3);
		virtual ~MitchellFilter();

		virtual real operator()(real position) const;
		virtual real radius() const;
		virtual std::string name() const;

	private:
		real b_;
		real c_;
	};

	typedef CountedPtr<MitchellFilter> MitchellFilterRef;
	typedef CountedPtr<const MitchellFilter> ConstMitchellFilterRef;

	inline MitchellFilterRef mitchellFilter(
		real b = (real)1 / 3,
		real c = (real)1 / 3)
	{
		return MitchellFilterRef(new MitchellFilter(b, c));
	}

}

#endif
