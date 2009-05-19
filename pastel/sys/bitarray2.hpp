#ifndef PASTEL_BITARRAY2_HPP
#define PASTEL_BITARRAY2_HPP

#include "pastel/sys/bitarray2.h"

#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <template <typename> class Array2>
	BitArray2<Array2>::BitArray2()
		: width_(0)
		, height_(0)
		, data_()
	{
	}

	template <template <typename> class Array2>
	BitArray2<Array2>::BitArray2(
		integer width, integer height,
		bool defaultData)
		: width_(width)
		, height_(height)
		, data_((width + 31) >> 5, height,
		defaultData ? 0xFFFFFFFF : 0)
	{
	}

	template <template <typename> class Array2>
	BitArray2<Array2>::BitArray2(
		const BitArray2& that,
		integer width, integer height,
		bool defaultData)
		: width_(width)
		, height_(height)
		, data_(that.data_,
		(width + 31) >> 5, height,
		defaultData ? 0xFFFFFFFF : 0)
	{
	}

	template <template <typename> class Array2>
	void BitArray2<Array2>::clear()
	{
		width_ = 0;
		height_ = 0;
		data_.clear();
	}

	template <template <typename> class Array2>
	void BitArray2<Array2>::swap(BitArray2& that)
	{
		std::swap(width_, that.width_);
		std::swap(height_, that.height_);
		data_.swap(that.data_);
	}

	template <template <typename> class Array2>
	void BitArray2<Array2>::setExtent(
		integer width, integer height,
		bool defaultData)
	{
		ENSURE1(width >= 0, width);
		ENSURE1(height >= 0, height);

		if (width != width_ || height != height_)
		{
			BitArray2 copy(*this, width,
				height, defaultData);
			swap(copy);
		}
	}

	template <template <typename> class Array2>
	integer BitArray2<Array2>::width() const
	{
		return width_;
	}

	template <template <typename> class Array2>
	integer BitArray2<Array2>::height() const
	{
		return height_;
	}

	template <template <typename> class Array2>
	BitArray2<Array2>& BitArray2<Array2>::operator=(
		const BitArray2& that)
	{
		BitArray2 copy(that);
		swap(copy);

		return *this;
	}

	template <template <typename> class Array2>
	void BitArray2<Array2>::set(integer x, integer y, bool value)
	{
		PENSURE2(x >= 0 && x < width_, x, width_);
		PENSURE2(y >= 0 && y < height_, y, height_);

		const integer xIndex = x >> 5;
		const integer xBit = x & 0x1F;
		const uint32 bitMask = 1 << xBit;

		data_(xIndex, y) &= ~bitMask;
		if (value)
		{
			data_(xIndex, y) |= bitMask;
		}
	}

	template <template <typename> class Array2>
	bool BitArray2<Array2>::operator()(integer x, integer y) const
	{
		PENSURE2(x >= 0 && x < width_, x, width_);
		PENSURE2(y >= 0 && y < height_, y, height_);

		const integer xIndex = x >> 5;
		const integer xBit = x & 0x1F;
		const uint32 bitMask = 1 << xBit;

		return (data_(xIndex, y) & bitMask) != 0;
	}

	template <template <typename> class Array2>
	integer BitArray2<Array2>::blockWidth() const
	{
		return data_.width();
	}

	template <template <typename> class Array2>
	integer BitArray2<Array2>::blockHeight() const
	{
		return data_.height();
	}

	template <template <typename> class Array2>
	uint32& BitArray2<Array2>::block(integer x, integer y)
	{
		return (uint32&)(((const BitArray2&)*this).block(x, y));
	}

	template <template <typename> class Array2>
	const uint32& BitArray2<Array2>::block(integer x, integer y) const
	{
		PENSURE2(x >= 0 && x < data_.width(), x, data_.width());
		PENSURE2(y >= 0 && y < data_.height(), y, data_.height());

		return data_(x, y);
	}

}

#endif
