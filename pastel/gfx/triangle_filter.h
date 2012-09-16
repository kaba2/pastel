// Description: Triangle_Filter class
// Detail: Triangle reconstruction filter
// Documentation: filters.txt

#ifndef PASTELGFX_TRIANGLE_FILTER_H
#define PASTELGFX_TRIANGLE_FILTER_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/filter.h"

namespace Pastel
{

	class PASTELGFX Triangle_Filter
		: public Filter
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.

		Triangle_Filter();

		virtual ~Triangle_Filter();

		virtual real evaluateInRange(real position) const;

	private:
		Triangle_Filter(const Triangle_Filter& that) PASTEL_DELETE;
		Triangle_Filter& operator=(const Triangle_Filter& that) PASTEL_DELETE;
	};

	typedef std::shared_ptr<Triangle_Filter> TriangleFilterPtr;
	typedef std::shared_ptr<const Triangle_Filter> ConstTriangleFilterPtr;

	inline TriangleFilterPtr triangleFilter()
	{
		return TriangleFilterPtr(new Triangle_Filter);
	}

}

#endif
