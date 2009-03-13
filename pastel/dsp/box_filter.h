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

		virtual ~BoxFilter();

		virtual real operator()(real position) const;
		virtual real radius() const;
		virtual real max() const;
		virtual std::string name() const;
	};

	typedef CountedPtr<BoxFilter> BoxFilterRef;
	typedef CountedPtr<const BoxFilter> ConstBoxFilterRef;

	inline BoxFilterRef boxFilter()
	{
		return BoxFilterRef(new BoxFilter);
	}

}

#endif
