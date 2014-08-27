// Description: Box_Filter class
// Detail: Box reconstruction filter
// Documentation: filters.txt

#ifndef PASTELGFX_BOX_FILTER_H
#define PASTELGFX_BOX_FILTER_H

#include "pastel/gfx/filter.h"

namespace Pastel
{

	class Box_Filter
		: public Filter
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.

		Box_Filter()
		: Filter(0.5, "box")
		{
		}

		virtual ~Box_Filter()
		{

		}

		virtual real evaluateInRange(real x) const
		{
			return 1;
		}

	private:
		Box_Filter(const Box_Filter& that) = delete;
		Box_Filter& operator=(const Box_Filter& that) = delete;
	};

	using BoxFilterPtr = std::shared_ptr<Box_Filter>;
	using ConstBoxFilterPtr = std::shared_ptr<const Box_Filter>;

	inline BoxFilterPtr boxFilter()
	{
		return std::make_shared<Box_Filter>();
	}

}

#endif
