// Description: Gaussian_Filter class
// Detail: Clamped gaussian reconstruction filter
// Documentation: filters.txt

#ifndef PASTELGFX_GAUSSIAN_FILTER_H
#define PASTELGFX_GAUSSIAN_FILTER_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/filter.h"

namespace Pastel
{

	class PASTELGFX Gaussian_Filter
		: public Filter
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		explicit Gaussian_Filter(real radius = 2);

		virtual ~Gaussian_Filter();

		virtual real evaluateInRange(real x) const;

	private:
		Gaussian_Filter(const Gaussian_Filter& that) PASTEL_DELETE;
		Gaussian_Filter& operator=(const Gaussian_Filter& that) PASTEL_DELETE;
	};

	typedef std::shared_ptr<Gaussian_Filter> GaussianFilterPtr;
	typedef std::shared_ptr<const Gaussian_Filter> ConstGaussianFilterPtr;

	inline GaussianFilterPtr gaussianFilter(real radius = 2)
	{
		return GaussianFilterPtr(new Gaussian_Filter(radius));
	}

}

#endif
