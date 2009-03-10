#ifndef PASTELGFX_TRIANGLE_FILTER_H
#define PASTELGFX_TRIANGLE_FILTER_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/filter.h"

namespace Pastel
{

	class PASTELGFX TriangleFilter
		: public Filter
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.

		virtual ~TriangleFilter();

		virtual real operator()(real position) const;
		virtual real radius() const;
		virtual std::string name() const;
	};

	typedef CountedPtr<TriangleFilter> TriangleFilterRef;
	typedef CountedPtr<const TriangleFilter> ConstTriangleFilterRef;

	inline TriangleFilterRef triangleFilter()
	{
		return TriangleFilterRef(new TriangleFilter);
	}

}

#endif
