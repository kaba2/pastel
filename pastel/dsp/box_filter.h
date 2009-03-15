#ifndef PASTELDSP_BOX_FILTER_H
#define PASTELDSP_BOX_FILTER_H

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

	typedef CountedPtr<BoxFilter> BoxFilterRef;
	typedef CountedPtr<const BoxFilter> ConstBoxFilterRef;

	inline BoxFilterRef boxFilter()
	{
		return BoxFilterRef(new BoxFilter);
	}

}

#endif
