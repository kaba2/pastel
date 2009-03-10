#ifndef PASTELGFX_GAUSSIAN_FILTER_H
#define PASTELGFX_GAUSSIAN_FILTER_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/filter.h"

namespace Pastel
{

	class PASTELGFX GaussianFilter
		: public Filter
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		explicit GaussianFilter(real radius = 2);

		virtual ~GaussianFilter();

		virtual real operator()(real position) const;

		void setRadius(real radius);
		virtual real radius() const;
		virtual std::string name() const;

	private:
		real radius_;
	};

	typedef CountedPtr<GaussianFilter> GaussianFilterRef;
	typedef CountedPtr<const GaussianFilter> ConstGaussianFilterRef;

	inline GaussianFilterRef gaussianFilter(real radius = 2)
	{
		return GaussianFilterRef(new GaussianFilter(radius));
	}

}

#endif
