// Description: Triangle_Filter class
// Detail: Triangle reconstruction filter
// Documentation: filters.txt

#ifndef PASTELGFX_TRIANGLE_FILTER_H
#define PASTELGFX_TRIANGLE_FILTER_H

#include "pastel/gfx/filter.h"

#include <cmath>

namespace Pastel
{

	class Triangle_Filter
		: public Filter
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.

		Triangle_Filter()
		: Filter(1, "triangle")
		{
		}

		virtual ~Triangle_Filter()
		{
		}

		virtual real evaluateInRange(real x) const
		{
			return 1 - std::abs(x);
		}

	private:
		Triangle_Filter(const Triangle_Filter& that) = delete;
		Triangle_Filter& operator=(const Triangle_Filter& that) = delete;
	};

	using TriangleFilterPtr = std::shared_ptr<Triangle_Filter>;
	using ConstTriangleFilterPtr = std::shared_ptr<const Triangle_Filter>;

	inline TriangleFilterPtr triangleFilter()
	{
		return std::make_shared<Triangle_Filter>();
	}

}

#endif
