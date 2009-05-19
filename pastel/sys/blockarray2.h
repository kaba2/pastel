/*!
\file
\brief A class for a 2-dimensional blocked array.
*/

#ifndef PASTEL_BLOCKARRAY2_H
#define PASTEL_BLOCKARRAY2_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/tuple.h"

#include <boost/operators.hpp>

namespace Pastel
{

	//! A 2-dimensional blocked array of objects.

	/*!
	Traditionally a 2-dimensional array is stored
	either as 'row-major' or 'column-major'. Row-major
	means to store the first row in memory before
	the second row. Column-major means to store the
	first column in memory before the second column.
	These data layouts are often not optimal, because
	small distances in the coordinates might mean
	either a long or a short distance in memory,
	depending on the direction and the data storage
	convention. For example, in row-major convention,
	one step in x direction corresponds to one
	step in memory, but one step in y direction corresponds
	to a whole row of steps in memory. This can
	cause a lot of cache misses if the memory is
	not accessed this way.
	This class uses a block data layout. The 2 dimensional
	array is divided into fixed size rectangular blocks
	which are themselves stored sequentially row-by-row,
	column-by-column (row-major).
	This results in better cache behaviour when accessing
	the array, since now small geometric distances
	correspond (most of the time) to small distances
	in memory.
	*/

	template <typename Type>
	class BlockArray2
	{
	public:
		typedef Type Element;

		class Cursor;
		class ConstCursor;

		//! Constructs an empty array.
		/*!
		Time complexity: constant
		Exception safety: strong
		*/
		BlockArray2();

		//! Constructs an array with the given dimensions and data.
		/*!
		width, height:
		The dimensions of the array to create.

		defaultData:
		The value to assign to all of the elements.
		If this parameter is not given, the Type's
		default constructor	will be used to create it.

		Preconditions:
		width >= 0
		height >= 0

		Time complexity: linear
		Exception safety: strong
		*/
		BlockArray2(integer width, integer height,
			const Type& defaultData = Type());

		//! Constructs a copy of the given array.
		/*!
		that:
		An array to copy data from.

		Time complexity: linear
		Exception safety: strong
		*/
		BlockArray2(const BlockArray2<Type>& that);

		//! Constructs a copy of the given array with given dimensions.
		/*!
		that:
		An array to copy data from.

		width, height:
		The dimensions of the array to create.

		defaultData:
		The value to assign to all those elements that
		are not copied from 'that'. If this parameter is
		not given, the Type's default constructor will be
		used to create it.

		Preconditions:
		width >= 0
		height >= 0

		Time complexity: linear
		Exception safety: strong
		*/
		BlockArray2(const BlockArray2<Type>& that,
			integer width, integer height,
			const Type& defaultData = Type());

		//! Destructs the array.
		~BlockArray2();

		//! Sets the array to zero size.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		void clear();

		//! Swaps two arrays.
		/*!
		that:
		Another array to swap with.

		Time complexity: constant
		Exception safety: nothrow
		*/
		void swap(BlockArray2<Type>& that);

		//! Sets the width and height of the array.
		/*!
		width, height:
		The new dimensions of the array.
		If width or height is zero, the effect is identical to
		calling "clear()".

		defaultData:
		The value to assign to all new elements.
		If this parameter is not given, the
		Type's default constructor will be
		used to create it.

		Preconditions:
		width >= 0
		height >= 0

		Time complexity: linear
		Exception safety: strong
		*/
		void setExtent(integer width,
			integer height, const Type& defaultData = Type());

		//! Returns the width of the array.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer width() const;

		//! Returns the height of the array.
		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer height() const;

		/*!
		Time complexity: constant
		Exception safety: nothrow
		*/
		bool empty() const;

		/*!
		'that':
		Another array to copy data from.

		Time complexity: linear
		Exception safety: strong
		*/
		BlockArray2<Type>& operator=(const BlockArray2<Type>& that);

		//! Returns a reference to the element (x, y)
		/*!
		x, y:
		The coordinates of the element to access.

		Preconditions:
		0 <= x < width()
		0 <= y < height()

		Time complexity: constant
		Exception safety: nothrow
		*/
		Type& operator()(integer x, integer y);
		const Type& operator()(integer x, integer y) const;
		Type& operator()(const Integer2& index);
		const Type& operator()(const Integer2& index) const;

		typename BlockArray2<Type>::Cursor cursor(integer x, integer y);
		typename BlockArray2<Type>::ConstCursor constCursor(integer x, integer y) const;

	private:
		/*
		Time complexity: linear
		Exception safety: strong
		*/
		void construct(
			integer width, integer height,
			const Type& defaultData);
		/*
		Time complexity: linear
		Exception safety: basic
		*/
		void copy(const BlockArray2<Type>& that,
			integer width, integer height,
			const Type& defaultData);

		integer width_;
		integer height_;
		Type* data_;
		integer realWidth_;
		integer realHeight_;
	};

	template <typename Type>
	class BlockArray2<Type>::Cursor
	{
	public:
		typedef Type Element;

		friend class BlockArray2<Type>;
		friend class BlockArray2<Type>::ConstCursor;

		Cursor()
			: x_(0)
			, y_(0)
			, data_(0)
		{
		}

		void swap(Cursor& that)
		{
			std::swap(x_, that.x_);
			std::swap(y_, that.y_);
			std::swap(data_, that.data_);
		}

		Type* operator->() const
		{
			return &(*data_)(x_, y_);
		}

		Type& operator*() const
		{
			return (*data_)(x_, y_);
		}

		void xIncrement()
		{
			++x_;
		}

		void xDecrement()
		{
			--x_;
		}

		void xMove(integer amount)
		{
			x_ += amount;
		}

		void yIncrement()
		{
			++y_;
		}

		void yDecrement()
		{
			--y_;
		}

		void yMove(integer amount)
		{
			y_ += amount;
		}

		void move(integer xAmount, integer yAmount)
		{
			x_ += xAmount;
			y_ += yAmount;
		}

	private:
		Cursor(integer x, integer y, BlockArray2<Type>* data)
			: x_(x)
			, y_(y)
			, data_(data)
		{
		}

		integer x_;
		integer y_;
		BlockArray2<Type>* data_;
	};

	template <typename Type>
	class BlockArray2<Type>::ConstCursor
	{
	public:
		typedef Type Element;

		friend class BlockArray2<Type>;

		ConstCursor()
			: x_(0)
			, y_(0)
			, data_(0)
		{
		}

		ConstCursor(const typename BlockArray2<Type>::Cursor& that)
			: x_(that.x_)
			, y_(that.y_)
			, data_(that.data_)
		{
		}

		void swap(ConstCursor& that)
		{
			std::swap(x_, that.x_);
			std::swap(y_, that.y_);
			std::swap(data_, that.data_);
		}

		const Type* operator->() const
		{
			return &(*data_)(x_, y_);
		}

		const Type& operator*() const
		{
			return (*data_)(x_, y_);
		}

		void xIncrement()
		{
			++x_;
		}

		void xDecrement()
		{
			--x_;
		}

		void xMove(integer amount)
		{
			x_ += amount;
		}

		void yIncrement()
		{
			++y_;
		}

		void yDecrement()
		{
			--y_;
		}

		void yMove(integer amount)
		{
			y_ += amount;
		}

		void move(integer xAmount, integer yAmount)
		{
			x_ += xAmount;
			y_ += yAmount;
		}

	private:
		ConstCursor(integer x, integer y, const BlockArray2<Type>* data)
			: x_(x)
			, y_(y)
			, data_(data)
		{
		}

		integer x_;
		integer y_;
		const BlockArray2<Type>* data_;
	};

	template <typename Type>
	void swap(BlockArray2<Type>& left,
		BlockArray2<Type>& right);

}

#include "pastel/sys/blockarray2.hpp"

#endif
