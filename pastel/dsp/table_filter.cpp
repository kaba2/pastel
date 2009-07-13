#include "pastel/dsp/table_filter.h"

#include "pastel/sys/subview.h"
#include "pastel/sys/view_tools.h"
#include "pastel/sys/arrayview.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	TableFilter::TableFilter(
		const Array<1, real>& data,
		real radius)
		: Filter(radius, "table")
		, data_()
		, center_(0)
		, scaling_(0)
	{
		const integer Padding = 1;
		const integer width = data.width();

		ENSURE(odd(width));

		const integer paddedWidth = width + 2 * Padding;
		
		data_.setExtent(paddedWidth, 0);

		copy(constArrayView(data),
			subView(arrayView(data_), 
			Rectangle<1>(Padding, paddedWidth - Padding)));

		center_ = paddedWidth / 2;
		scaling_ = (width / 2) / Filter::radius();
	}

	TableFilter::~TableFilter()
	{
	}

	real TableFilter::evaluateInRange(real x) const
	{
		const real filterPos = 
			center_ + x * scaling_;
		
		const integer filterIndex =
			(integer)filterPos;

		return linear(
			data_(filterIndex),
			data_(filterIndex + 1), 
			filterPos - filterIndex);
	}

	void TableFilter::onSetRadius()
	{
		scaling_ = (data_.width() / 2) / radius();
	}

	PASTELDSP TableFilterPtr tableFilter(
		const ConstFilterPtr& filter,
		integer samplesPerRadius)
	{
		ENSURE_OP(samplesPerRadius, >=, 0);

		const real filterRadius = filter->radius();
		const integer filterTableRadius = 
			std::ceil(samplesPerRadius * filterRadius);
		const integer filterTableSize = 
			filterTableRadius * 2 + 1;
		Array<1, real> filterTable(filterTableSize);
		
		for (integer i = 0;i < filterTableSize;++i)
		{
			const real filterPosition =
				(2 * dequantizeUnsignedMatchEnds(i, filterTableSize) - 1) * filterRadius;
			
			filterTable(i) = filter->evaluateInRange(filterPosition);
		}

		return TableFilterPtr(new TableFilter(filterTable, filterRadius));
	}

}
