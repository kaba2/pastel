// Description: Box_Filter class
// Detail: Box reconstruction filter
// Documentation: filters.txt

#ifndef PASTELDSP_BOX_FILTER_H
#define PASTELDSP_BOX_FILTER_H

#include "pastel/dsp/dsplibrary.h"
#include "pastel/dsp/filter.h"

namespace Pastel
{

	class PASTELDSP Box_Filter
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
		Box_Filter(const Box_Filter& that) PASTEL_DELETE;
		Box_Filter& operator=(const Box_Filter& that) PASTEL_DELETE;
	};

	typedef CountedPtr<Box_Filter> BoxFilterPtr;
	typedef CountedPtr<const Box_Filter> ConstBoxFilterPtr;

	inline BoxFilterPtr boxFilter()
	{
		return BoxFilterPtr(new Box_Filter);
	}

}

#endif
