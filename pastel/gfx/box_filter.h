// Description: Box_Filter class
// Detail: Box reconstruction filter
// Documentation: filters.txt

#ifndef PASTELGFX_BOX_FILTER_H
#define PASTELGFX_BOX_FILTER_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/filter.h"

namespace Pastel
{

	class PASTELGFX Box_Filter
		: public Filter
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.

		Box_Filter();

		virtual ~Box_Filter();

		virtual real evaluateInRange(real x) const;

	private:
		Box_Filter(const Box_Filter& that) = delete;
		Box_Filter& operator=(const Box_Filter& that) = delete;
	};

	typedef std::shared_ptr<Box_Filter> BoxFilterPtr;
	typedef std::shared_ptr<const Box_Filter> ConstBoxFilterPtr;

	inline BoxFilterPtr boxFilter()
	{
		return BoxFilterPtr(new Box_Filter);
	}

}

#endif
