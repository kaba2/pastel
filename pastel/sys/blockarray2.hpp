#ifndef PASTELSYS_BLOCKARRAY2_HPP
#define PASTELSYS_BLOCKARRAY2_HPP

#include "pastel/sys/blockarray2.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/syscommon.h"
#include "pastel/sys/destruct.h"

#include <algorithm>

namespace Pastel
{

	namespace BlockArray2_Detail
	{

		enum
		{
			XBlockShift = 3,
			YBlockShift = 3,
			XBlockMask = (1 << XBlockShift) - 1,
			YBlockMask = (1 << YBlockShift) - 1,
			BlockWidth = (1 << XBlockShift),
			BlockHeight = (1 << YBlockShift),
			BlockSize = BlockWidth * BlockHeight
		};

	}

	template <typename Type>
	BlockArray2<Type>::BlockArray2()
		: width_(0)
		, height_(0)
		, data_(0)
		, realWidth_(0)
		, realHeight_(0)
	{
	}

	template <typename Type>
	BlockArray2<Type>::BlockArray2(
		integer width, integer height,
		const Type& defaultData)
		: width_(0)
		, height_(0)
		, data_(0)
		, realWidth_(0)
		, realHeight_(0)
	{
		construct(width, height, defaultData);
	}

	template <typename Type>
	BlockArray2<Type>::BlockArray2(const BlockArray2<Type>& that)
		: width_(0)
		, height_(0)
		, data_(0)
		, realWidth_(0)
		, realHeight_(0)
	{
		construct(that.width_, that.height_, Type());
		copy(that, width_, height_, Type());
	}

	template <typename Type>
	BlockArray2<Type>::BlockArray2(
		const BlockArray2<Type>& that,
		integer width, integer height,
		const Type& defaultData)
		: width_(0)
		, height_(0)
		, data_(0)
		, realWidth_(0)
		, realHeight_(0)
	{
		construct(width, height, defaultData);
		copy(that, width_, height_, defaultData);
	}

	template <typename Type>
	BlockArray2<Type>::~BlockArray2()
	{
		clear();
	}

	template <typename Type>
	void BlockArray2<Type>::clear()
	{
		if (data_)
		{
			destruct(data_, data_ + realWidth_ * realHeight_);
			delete[] ((int8*)data_);
			data_ = 0;
		}

		width_ = 0;
		height_ = 0;

		realWidth_ = 0;
		realHeight_ = 0;
	}

	template <typename Type>
	void BlockArray2<Type>::swap(BlockArray2<Type>& that)
	{
		std::swap(width_, that.width_);
		std::swap(height_, that.height_);
		std::swap(data_, that.data_);
		std::swap(realWidth_, that.realWidth_);
		std::swap(realHeight_, that.realHeight_);
	}

	template <typename Type>
	void BlockArray2<Type>::setExtent(
		integer width, integer height,
		const Type& defaultData)
	{
		ENSURE1(width >= 0, width);
		ENSURE1(height >= 0, height);

		if (width != width_ || height != height_)
		{
			BlockArray2<Type> copy(*this, width,
				height, defaultData);
			swap(copy);
		}
	}

	template <typename Type>
	integer BlockArray2<Type>::width() const
	{
		return width_;
	}

	template <typename Type>
	integer BlockArray2<Type>::height() const
	{
		return height_;
	}

	template <typename Type>
	bool BlockArray2<Type>::empty() const
	{
		return width_ == 0 || height_ == 0;
	}

	template <typename Type>
	BlockArray2<Type>& BlockArray2<Type>::operator=(
		const BlockArray2<Type>& that)
	{
		BlockArray2<Type> copy(that);
		swap(copy);

		return *this;
	}

	template <typename Type>
	Type& BlockArray2<Type>::operator()(integer x, integer y)
	{
		const BlockArray2& self = *this;
		return const_cast<Type&>(self(x, y));
	}

	template <typename Type>
	const Type& BlockArray2<Type>::operator()(integer x, integer y) const
	{
		PENSURE2(x >= 0 && x < width_, x, width_);
		PENSURE2(y >= 0 && y < height_, y, height_);

		const integer xBlock = x >> BlockArray2_Detail::XBlockShift;
		//integer yBlock = y >> BlockArray2_Detail::YBlockShift;
		//integer xRounded = x & (~BlockArray2_Detail::XBlockMask);
		const integer yRounded = y & (~BlockArray2_Detail::YBlockMask);
		const integer xInBlock = x & BlockArray2_Detail::XBlockMask;
		const integer yInBlock = y & BlockArray2_Detail::YBlockMask;

		const Type* blockAddress =
			data_ +
			yRounded * realWidth_ +
			xBlock * BlockArray2_Detail::BlockSize;

		const Type* address =
			blockAddress +
			yInBlock * BlockArray2_Detail::BlockWidth +
			xInBlock;

		return *address;
	}

	template <typename Type>
	Type& BlockArray2<Type>::operator()(const Integer2& index)
	{
		const BlockArray2& self = *this;
		return const_cast<Type&>(self(index[0], index[1]));
	}

	template <typename Type>
	const Type& BlockArray2<Type>::operator()(const Integer2& index) const
	{
		const BlockArray2& self = *this;
		return self(index[0], index[1]);
	}

	// Private

	template <typename Type>
	void BlockArray2<Type>::construct(
		integer width, integer height,
		const Type& defaultData)
	{
		ASSERT1(width >= 0, width);
		ASSERT1(height >= 0, height);
		ASSERT(!data_);

		if (width > 0 && height > 0)
		{
			// Get the real width and height of the array
			// by aligning 'width' and 'height' to the nearest
			// greater block sizes.
			const integer newRealHeight = roundUpToPowerOf2(
				height, BlockArray2_Detail::YBlockShift);
			const integer newRealWidth = roundUpToPowerOf2(
				width, BlockArray2_Detail::XBlockShift);

			// Allocate raw memory.
			const integer units = newRealWidth * newRealHeight;
			int8* newData = new int8[units * sizeof(Type)];

			try
			{
				// Construct the objects in the array.
				std::uninitialized_fill_n((Type*)newData,
					units, defaultData);
			}
			catch(...)
			{
				delete[] newData;
				newData = 0;
				throw;
			}

			// Commit

			data_ = (Type*)newData;
			realWidth_ = newRealWidth;
			realHeight_ = newRealHeight;
			width_ = width;
			height_ = height;
		}
	}

	template <typename Type>
	void BlockArray2<Type>::copy(
		const BlockArray2<Type>& that,
		integer width, integer height,
		const Type& defaultData)
	{
		ASSERT2(0 <= width && width <= width_,
			width, width_);
		ASSERT2(0 <= height && height <= height_,
			height, height_);

		BlockArray2& self = *this;

		const integer xMinSize = std::min(width, that.width_);
		const integer yMinSize = std::min(height, that.height_);

		if (xMinSize > 0 && yMinSize > 0)
		{
			for (integer y = 0;y < yMinSize;++y)
			{
				// Copy the start of the row from 'that'.
				for (integer x = 0;x < xMinSize;++x)
				{
					self(x, y) = that(x, y);
				}

				// Fill the rest of the row with 'defaultData'.
				for (integer x = xMinSize;x < width;++x)
				{
					self(x, y) = defaultData;
				}
			}

			// Fill the rest of the rows with 'defaultData'.
			for (integer y = yMinSize;y < height;++y)
			{
				for (integer x = 0;x < width;++x)
				{
					self(x, y) = defaultData;
				}
			}
		}
	}

	template <typename Type>
	typename BlockArray2<Type>::Cursor BlockArray2<Type>::cursor(integer x, integer y)
	{
		return Cursor(x, y, this);
	}

	template <typename Type>
	typename BlockArray2<Type>::ConstCursor BlockArray2<Type>::constCursor(integer x, integer y) const
	{
		return ConstCursor(x, y, this);
	}

	template <typename Type>
	void swap(BlockArray2<Type>& left,
		BlockArray2<Type>& right)
	{
		left.swap(right);
	}

}

#endif
