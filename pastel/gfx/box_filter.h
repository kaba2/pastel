#ifndef PASTELGFX_BOX_FILTER_H
#define PASTELGFX_BOX_FILTER_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/filter.h"

namespace Pastel
{

	class PASTELGFX BoxFilter
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
