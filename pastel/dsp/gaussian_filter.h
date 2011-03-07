// Description: Gaussian_Filter class
// Detail: Clamped gaussian reconstruction filter
// Documentation: filters.txt

#ifndef PASTEL_GAUSSIAN_FILTER_H
#define PASTEL_GAUSSIAN_FILTER_H

#include "pastel/dsp/dsplibrary.h"
#include "pastel/dsp/filter.h"

namespace Pastel
{

	class PASTELDSP Gaussian_Filter
		: public Filter
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		explicit Gaussian_Filter(real radius = 2);

		virtual ~Gaussian_Filter();

		virtual real evaluateInRange(real x) const;

	private:
		// Prohibited
		Gaussian_Filter(const Gaussian_Filter& that);
		// Prohibited
		Gaussian_Filter& operator=(const Gaussian_Filter& that);
	};

	typedef CountedPtr<Gaussian_Filter> GaussianFilterPtr;
	typedef CountedPtr<const Gaussian_Filter> ConstGaussianFilterPtr;

	inline GaussianFilterPtr gaussianFilter(real radius = 2)
	{
		return GaussianFilterPtr(new Gaussian_Filter(radius));
	}

}

#endif
