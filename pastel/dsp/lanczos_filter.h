#ifndef PASTELDSP_LANCZOS_FILTER_H
#define PASTELDSP_LANCZOS_FILTER_H

#include "pastel/dsp/dsplibrary.h"
#include "pastel/dsp/filter.h"

namespace Pastel
{

	//! Lanczos reconstruction filter.
	/*!
	This filter is interpolatory. It is given by
	f(x) = sinc(|x|) * sinc(|x| / radius), |x| < radius
										0, otherwise
	*/

	class PASTELDSP LanczosFilter
		: public Filter
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		explicit LanczosFilter(real radius = 2);
		virtual ~LanczosFilter();

		virtual real evaluateInRange(real x) const;

	private:
		// Prohibited
		LanczosFilter(const LanczosFilter& that);
		// Prohibited
		LanczosFilter& operator=(const LanczosFilter& that);

		virtual void onSetRadius();

		real invRadius_;
	};

	typedef CountedPtr<LanczosFilter> LanczosFilterPtr;
	typedef CountedPtr<const LanczosFilter> ConstLanczosFilterPtr;

	inline LanczosFilterPtr lanczosFilter(real radius = 2)
	{
		return LanczosFilterPtr(new LanczosFilter(radius));
	}

}

#endif
