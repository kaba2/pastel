// Description: GaussianFilter class
// Detail: Clamped gaussian reconstruction filter

#ifndef PASTEL_GAUSSIAN_FILTER_H
#define PASTEL_GAUSSIAN_FILTER_H

#include "pastel/dsp/dsplibrary.h"
#include "pastel/dsp/filter.h"

namespace Pastel
{

	class PASTELDSP GaussianFilter
		: public Filter
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		explicit GaussianFilter(real radius = 2);

		virtual ~GaussianFilter();

		virtual real evaluateInRange(real x) const;

	private:
		// Prohibited
		GaussianFilter(const GaussianFilter& that);
		// Prohibited
		GaussianFilter& operator=(const GaussianFilter& that);
	};

	typedef CountedPtr<GaussianFilter> GaussianFilterPtr;
	typedef CountedPtr<const GaussianFilter> ConstGaussianFilterPtr;

	inline GaussianFilterPtr gaussianFilter(real radius = 2)
	{
		return GaussianFilterPtr(new GaussianFilter(radius));
	}

}

#endif
