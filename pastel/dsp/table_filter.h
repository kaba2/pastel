// Description: TableFilter class
// Detail: Piecewise-linear modifiable reconstruction filter

#ifndef PASTEL_TABLE_FILTER_H
#define PASTEL_TABLE_FILTER_H

#include "pastel/dsp/dsplibrary.h"
#include "pastel/dsp/filter.h"

#include "pastel/sys/array.h"

namespace Pastel
{

	//! Piecewise-linear reconstruction filter.
	/*!
	This class represents a piecewise-linear
	filter whose knots are uniformly distributed
	on the range [-radius, radius].
	The first knot is located at x = -radius and
	the last knot is located at x = radius.

	The purpose of this class is to act
	as an efficient approximation for the
	other filters.
	*/

	class PASTELDSP TableFilter
		: public Filter
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.

		//! Construct with the given data.
		/*!
		Preconditions:
		radius > 0
		odd(data.width())
		*/
		TableFilter(
			const Array<real, 1>& data,
			real radius);

		virtual ~TableFilter();

		virtual real evaluateInRange(real position) const;

	private:
		// Prohibited
		TableFilter(const TableFilter& that);
		// Prohibited
		TableFilter& operator=(const TableFilter& that);

		virtual void onSetRadius();

		Array<real, 1> data_;
		integer center_;
		real scaling_;
	};

	typedef CountedPtr<TableFilter> TableFilterPtr;
	typedef CountedPtr<const TableFilter> ConstTableFilterPtr;

	PASTELDSP TableFilterPtr tableFilter(const ConstFilterPtr& filter, integer samplesPerRadius = 16);

}

#endif
