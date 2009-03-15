#ifndef PASTELDSP_GAUSSIAN_FILTER_H
#define PASTELDSP_GAUSSIAN_FILTER_H

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

	typedef CountedPtr<GaussianFilter> GaussianFilterRef;
	typedef CountedPtr<const GaussianFilter> ConstGaussianFilterRef;

	inline GaussianFilterRef gaussianFilter(real radius = 2)
	{
		return GaussianFilterRef(new GaussianFilter(radius));
	}

}

#endif
