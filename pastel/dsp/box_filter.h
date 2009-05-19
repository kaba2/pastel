#ifndef PASTEL_BOX_FILTER_H
#define PASTEL_BOX_FILTER_H

#include "pastel/dsp/dsplibrary.h"
#include "pastel/dsp/filter.h"

namespace Pastel
{

	class PASTELDSP BoxFilter
		: public Filter
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.

		BoxFilter();

		virtual ~BoxFilter();

		virtual real evaluateInRange(real x) const;

	private:
		// Prohibited
		BoxFilter(const BoxFilter& that);
		// Prohibited
		BoxFilter& operator=(const BoxFilter& that);
	};

	typedef CountedPtr<BoxFilter> BoxFilterPtr;
	typedef CountedPtr<const BoxFilter> ConstBoxFilterPtr;

	inline BoxFilterPtr boxFilter()
	{
		return BoxFilterPtr(new BoxFilter);
	}

}

#endif
