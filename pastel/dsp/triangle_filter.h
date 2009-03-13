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
