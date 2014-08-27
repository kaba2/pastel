// Description: Lanczos_Filter class
// Detail: Lanczos reconstruction filter
// Documentation: filters.txt

#ifndef PASTELGFX_LANCZOS_FILTER_H
#define PASTELGFX_LANCZOS_FILTER_H

#include "pastel/gfx/filter.h"

#include "pastel/sys/sinc.h"

#include <cmath>

namespace Pastel
{

	//! Lanczos reconstruction filter.
	/*!
	This filter is interpolatory. It is given by
	f(x) = sinc(|x|) * sinc(|x| / radius), |x| < radius
										0, otherwise
	*/
	class Lanczos_Filter
		: public Filter
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		explicit Lanczos_Filter(real radius = 2)
		: Filter(radius, "lanczos")
		, invRadius_(inverse(radius))
		{
		}
		
		virtual ~Lanczos_Filter()
		{
		}

		virtual real evaluateInRange(real x) const
		{
			return sinc<real>(x) * sinc<real>(x * invRadius_);
		}


	private:
		Lanczos_Filter(const Lanczos_Filter& that) = delete;
		Lanczos_Filter& operator=(const Lanczos_Filter& that) = delete;

		virtual void onSetRadius()
		{
			invRadius_ = inverse(radius());
		}

		real invRadius_;
	};

	using LanczosFilterPtr = std::shared_ptr<Lanczos_Filter>;
	using ConstLanczosFilterPtr = std::shared_ptr<const Lanczos_Filter>;

	inline LanczosFilterPtr lanczosFilter(real radius = 2)
	{
		return std::make_shared<Lanczos_Filter>(radius);
	}

}

#endif
