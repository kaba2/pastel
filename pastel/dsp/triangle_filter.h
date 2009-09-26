// Description: Triangle_Filter class
// Detail: Triangle reconstruction filter
// Documentation: filter.txt

#ifndef PASTEL_TRIANGLE_FILTER_H
#define PASTEL_TRIANGLE_FILTER_H

#include "pastel/dsp/dsplibrary.h"
#include "pastel/dsp/filter.h"

namespace Pastel
{

	class PASTELDSP Triangle_Filter
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
		// Prohibited
		Triangle_Filter(const Triangle_Filter& that);
		// Prohibited
		Triangle_Filter& operator=(const Triangle_Filter& that);
	};

	typedef CountedPtr<Triangle_Filter> TriangleFilterPtr;
	typedef CountedPtr<const Triangle_Filter> ConstTriangleFilterPtr;

	inline TriangleFilterPtr triangleFilter()
	{
		return TriangleFilterPtr(new Triangle_Filter);
	}

}

#endif
