#ifndef PASTELGFX_TABLE_FILTER_HPP
#define PASTELGFX_TABLE_FILTER_HPP

#include "pastel/gfx/filter/table_filter.h"

#include "pastel/sys/view/subview.h"
#include "pastel/sys/view/view_tools.h"
#include "pastel/sys/view/arrayview.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	inline Table_Filter::Table_Filter(
		const Array<dreal, 1>& data,
		dreal radius)
		: Filter(radius, "table")
		, data_()
		, center_(0)
		, scaling_(0)
	{
		integer Padding = 1;
		integer width = data.width();

		ENSURE(odd(width));

		const integer paddedWidth = width + 2 * Padding;
		
		data_.setExtent(paddedWidth, 0);

		copy(constArrayView(data),
			subView(arrayView(data_), 
			AlignedBox<integer, 1>(Padding, paddedWidth - Padding)));

		center_ = paddedWidth / 2;
		scaling_ = (width / 2) / Filter::radius();
	}

	inline Table_Filter::~Table_Filter()
	{
	}

	inline dreal Table_Filter::evaluateInRange(dreal x) const
	{
		dreal filterPos =
			center_ + x * scaling_;
		
		integer filterIndex =
			(integer)filterPos;

		return linear(
			data_(filterIndex),
			data_(filterIndex + 1), 
			filterPos - filterIndex);
	}

	inline void Table_Filter::onSetRadius()
	{
		scaling_ = (data_.width() / 2) / radius();
	}

	inline TableFilterPtr tableFilter(
		const ConstFilterPtr& filter,
		integer samplesPerRadius)
	{
		ENSURE_OP(samplesPerRadius, >=, 0);

		dreal filterRadius = filter->radius();
		integer filterTableRadius =
			std::ceil(samplesPerRadius * filterRadius);
		integer filterTableSize =
			filterTableRadius * 2 + 1;
		Array<dreal, 1> filterTable(filterTableSize);
		
		for (integer i = 0;i < filterTableSize;++i)
		{
			dreal filterPosition =
				(2 * dequantizeUnsignedMatchEnds(i, filterTableSize) - 1) * filterRadius;
			
			filterTable(i) = filter->evaluateInRange(filterPosition);
		}

		return std::make_shared<Table_Filter>(filterTable, filterRadius);
	}

}

#endif
