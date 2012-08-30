// Description: Lanczos_Filter class
// Detail: Lanczos reconstruction filter
// Documentation: filters.txt

#ifndef PASTELGFX_LANCZOS_FILTER_H
#define PASTELGFX_LANCZOS_FILTER_H

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

	class PASTELGFX Lanczos_Filter
		: public Filter
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		explicit Lanczos_Filter(real radius = 2);
		virtual ~Lanczos_Filter();

		virtual real evaluateInRange(real x) const;

	private:
		Lanczos_Filter(const Lanczos_Filter& that) PASTEL_DELETE;
		Lanczos_Filter& operator=(const Lanczos_Filter& that) PASTEL_DELETE;

		virtual void onSetRadius();

		real invRadius_;
	};

	typedef CountedPtr<Lanczos_Filter> LanczosFilterPtr;
	typedef CountedPtr<const Lanczos_Filter> ConstLanczosFilterPtr;

	inline LanczosFilterPtr lanczosFilter(real radius = 2)
	{
		return LanczosFilterPtr(new Lanczos_Filter(radius));
	}

}

#endif
