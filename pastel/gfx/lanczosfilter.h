#ifndef PASTELGFX_LANCZOSFILTER_H
#define PASTELGFX_LANCZOSFILTER_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/filter.h"

namespace Pastel
{

	//! Lanczos reconstruction filter.
	/*!
	This filter is interpolatory. It is given by
	f(x) = sinc(|x|) * sinc(|x| / radius), |x| < radius
	                                    0, otherwise
	*/

    class PASTELGFX LanczosFilter
		: public Filter
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		explicit LanczosFilter(real radius = 2);
		virtual ~LanczosFilter();

		virtual real operator()(real position) const;

		void setRadius(real radius);
		virtual real radius() const;

	private:
		real radius_;
		real invRadius_;
	};

	typedef CountedPtr<LanczosFilter> LanczosFilterRef;
	typedef CountedPtr<const LanczosFilter> ConstLanczosFilterRef;

	inline LanczosFilterRef lanczosFilter(real radius = 2)
	{
		return LanczosFilterRef(new LanczosFilter(radius));
	}

}

#endif
