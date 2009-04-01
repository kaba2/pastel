#ifndef PASTELDSP_TRIANGLE_FILTER_H
#define PASTELDSP_TRIANGLE_FILTER_H

#include "pastel/dsp/dsplibrary.h"
#include "pastel/dsp/filter.h"

namespace Pastel
{

	class PASTELDSP TriangleFilter
		: public Filter
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.

		TriangleFilter();

		virtual ~TriangleFilter();

		virtual real evaluateInRange(real position) const;

	private:
		// Prohibited
		TriangleFilter(const TriangleFilter& that);
		// Prohibited
		TriangleFilter& operator=(const TriangleFilter& that);
	};

	typedef CountedPtr<TriangleFilter> TriangleFilterPtr;
	typedef CountedPtr<const TriangleFilter> ConstTriangleFilterPtr;

	inline TriangleFilterPtr triangleFilter()
	{
		return TriangleFilterPtr(new TriangleFilter);
	}

}

#endif
