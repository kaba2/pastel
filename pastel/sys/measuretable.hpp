#ifndef PASTELSYS_MEASURETABLE_HPP
#define PASTELSYS_MEASURETABLE_HPP

#include "pastel/sys/measuretable.h"

namespace Pastel
{

	inline void MeasureTable::setCaption(const std::string& caption)
	{
		caption_ = caption;
	}

	inline const std::string& MeasureTable::caption() const
	{
		return caption_;
	}

	inline void MeasureTable::setSize(integer width, integer height)
	{
		data_.setExtent(Vector2i(width, height));
	}

	inline integer MeasureTable::width() const
	{
		return data_.width();
	}

	inline integer MeasureTable::height() const
	{
		return data_.height();
	}

	inline MeasureTable::Entry& MeasureTable::operator()(integer x, integer y)
	{
		return data_(x, y);
	}

	inline const MeasureTable::Entry& MeasureTable::operator()(integer x, integer y) const
	{
		return data_(x, y);
	}

	inline void MeasureTable::addHorizontalSeparator(integer y)
	{
		hSeparatorSet_.insert(y);
	}

	inline void MeasureTable::addVerticalSeparator(integer x)
	{
		vSeparatorSet_.insert(x);
	}

	inline bool MeasureTable::hasHorizontalSeparator(integer y) const
	{
		return hSeparatorSet_.count(y);
	}

	inline bool MeasureTable::hasVerticalSeparator(integer x) const
	{
		return vSeparatorSet_.count(x);
	}

}

#endif
