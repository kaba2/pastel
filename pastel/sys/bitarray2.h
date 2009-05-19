/*!
\file
\brief A class for a 2-dimensional bit array.
*/

#ifndef PASTEL_BITARRAY2_H
#define PASTEL_BITARRAY2_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/syslibrary.h"

namespace Pastel
{

	//! A 2-dimensional bit array.

	/*!
	This is a 2-dimensional array container
	for booleans. The motivation for using this
	container is memory efficiency: the bool
	type itself always has sizeof(bool) >= 1.
	It packs the bits into and unpacks them from
	larger size native type variables so that
	the size of the bit really is one bit.
	*/

	template <template <typename> class Array2>
	class BitArray2
	{
	public:

		typedef bool Element;

		// Using default copy constructor.
		// Using default destructor.

		//! Constructs an array of zero size.
		/*!
		Time complexity: constant
		Exception safety: strong
		*/
		BitArray2();

		//! Constructs an array with the given dimensions and value.
		/*!
		Preconditions:
		width >= 0
		height >= 0

		Time complexity: linear
		Exception safety: strong
		*/
		BitArray2(integer width, integer height,
			bool defaultData = false);

		//! Constructs an array with the given dimensions and values.
		/*!
		Preconditions:
		width >= 0
		height >= 0

		Time complexity: linear
		Exception safety: strong

		The given array 'that' is placed at the lower left corner
		of the array.
		*/
		BitArray2(const BitArray2& that,
			integer width, integer height,
			bool defaultData = false);

		//! Sets the width and height of the array to zero.
		/*!
		Time complexity: constant
		Exception safety: nothrow

		This is equal to calling 'setExtent(0, 0)'.
		*/
		void clear();

		//! Swaps two arrays.
		/*
		Time complexity: constant
		Exception safety: nothrow
		*/
		void swap(BitArray2& that);

		//! Sets the width and height of the array.
		/*!
		Preconditions:
		width >= 0
		height >= 0

		Time complexity: linear
		Exception safety: strong

		If width or height is zero, the effect is identical to
		calling "clear()".
		'defaultData' will be used to copy construct the new
		objects. If 'defaultData' is not given, it is defaulted
		false.
		*/
		void setExtent(integer width,
			integer height, bool defaultData = false);

		//! Returns the width of the array.
		/*
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer width() const;

		//! Returns the height of the array.
		/*
		Time complexity: constant
		Exception safety: nothrow
		*/
		integer height() const;

		//! Assigns another array.
		/*
		Time complexity: linear
		Exception safety: strong
		*/
		BitArray2<Array2>& operator=(const BitArray2& that);

		//! Sets the element at (x, y) to 'value'.
		/*!
		Preconditions:
		x >= 0 && x < width()
		y >= 0 && y < height()

		Time complexity: constant
		Exception safety: nothrow
		*/
		void set(integer x, integer y, bool value);

		//! Returns the value of the element at (x, y).
		/*!
		Preconditions:
		x >= 0 && x < width()
		y >= 0 && y < height()

		Time complexity: constant
		Exception safety: nothrow
		*/
		bool operator()(integer x, integer y) const;

		//! Returns the width in the number of 32-bits blocks.
		/*!
		Time complexity: constant
		Exception safety: nothrow

		The returned value is equal to 'ceil(width() / 32)'.
		*/
		integer blockWidth() const;

		//! Returns the height of the array.
		/*!
		Time complexity: constant
		Exception safety: nothrow

		The returned value is equal to 'height()'.
		This function is provided for symmetry, when
		the 'blockWidth()' function is used.
		*/
		integer blockHeight() const;

		//! Returns the 'x':th 32-bit block at the row 'y'.
		/*!
		Preconditions:
		x >= 0 && x < blockWidth()
		y >= 0 && y < blockHeight()

		Time complexity: constant
		Exception safety: nothrow
		*/
		uint32& block(integer x, integer y);

		//! Returns the 'x':th 32-bit block at the row 'y'.
		/*!
		Preconditions:
		x >= 0 && x < blockWidth()
		y >= 0 && y < blockHeight()

		Time complexity: constant
		Exception safety: nothrow
		*/
		const uint32& block(integer x, integer y) const;

	private:
		integer width_;
		integer height_;
		Array2<uint32> data_;
	};

}

#include "pastel/sys/bitarray2.hpp"

#endif
