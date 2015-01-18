// Description: Gaussian_Filter class
// Detail: Clamped gaussian reconstruction filter
// Documentation: filters.txt

#ifndef PASTELGFX_GAUSSIAN_FILTER_H
#define PASTELGFX_GAUSSIAN_FILTER_H

#include "pastel/gfx/filter.h"

#include <cmath>

namespace Pastel
{

	class Gaussian_Filter
		: public Filter
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		explicit Gaussian_Filter(real radius = 2)
		: Filter(radius, "gaussian")
		{
		}

		virtual ~Gaussian_Filter()
		{
		}

		virtual real evaluateInRange(real x) const
		{
			// I chose the factor -2 because
			// it fitted well with the lobe of
			// the Lanczos filter. No deeper
			// science here.

			return std::exp(-2 * x * x);
		}

	private:
		Gaussian_Filter(const Gaussian_Filter& that) = delete;
		Gaussian_Filter& operator=(const Gaussian_Filter& that) = delete;
	};

	using GaussianFilterPtr = std::shared_ptr<Gaussian_Filter>;
	using ConstGaussianFilterPtr = std::shared_ptr<const Gaussian_Filter>;

	inline GaussianFilterPtr gaussianFilter(real radius = 2)
	{
		return GaussianFilterPtr(new Gaussian_Filter(radius));
	}

}

#endif
