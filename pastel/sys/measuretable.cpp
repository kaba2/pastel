#include "pastel/sys/measuretable.h"

namespace Pastel
{

	void MeasureTable::setCaption(const std::string& caption)
	{
		caption_ = caption;
	}

	const std::string& MeasureTable::caption() const
	{
		return caption_;
	}

	void MeasureTable::setSize(integer width, integer height)
	{
		data_.setExtent(width, height);
	}

	integer MeasureTable::width() const
	{
		return data_.width();
	}

	integer MeasureTable::height() const
	{
		return data_.height();
	}

	MeasureTable::Entry& MeasureTable::operator()(integer x, integer y)
	{
		return data_(x, y);
	}

	const MeasureTable::Entry& MeasureTable::operator()(integer x, integer y) const
	{
		return data_(x, y);
	}

	void MeasureTable::addHorizontalSeparator(integer y)
	{
		hSeparatorSet_.insert(y);
	}

	void MeasureTable::addVerticalSeparator(integer x)
	{
		vSeparatorSet_.insert(x);
	}

	bool MeasureTable::hasHorizontalSeparator(integer y) const
	{
		return hSeparatorSet_.find(y) < hSeparatorSet_.size();
	}

	bool MeasureTable::hasVerticalSeparator(integer x) const
	{
		return vSeparatorSet_.find(x) < vSeparatorSet_.size();
	}

}
