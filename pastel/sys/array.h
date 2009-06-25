/*!
\file
\brief A class for an n-dimensional array.
*/

#ifndef PASTEL_ARRAY_H
#define PASTEL_ARRAY_H

#include "pastel/sys/arraybase.h"

namespace Pastel
{

	//! A multi-dimensional array.
	/*!
	Let the dimensionality of the array be n.
	The elements of the array are indexed by a tuple
	(x_1, x_2, ..., x_n) in Z+^n. This tuple
	must satisfy:
	
	for all i in [1, n]: 0 <= x_i < w_i
	
	where (w_1, w_2, ..., w_n) is a tuple containing
	the extents of the array. We guarantee
	that the elements are stored sequantially in
	the following manner (corresponding to C
	array storage convention):

	element index = sum[i = 1..n] x_i a_i

	where

	a_1 = 1
	a_i = a_(i - 1) * w_(i - 1)

	For example, a 2d array stores the whole 
	i:th row before the (i+1):th row
	(called a row-major order).

	You can obtain constant-time subviews to 
	the array by using array views, see
	'pastel/sys/view_all.h'.
	*/

	template <int N, typename Type>
	class Array
		: public Detail_Array::ArrayBase<N, Type>
	{
	private:
		typedef Detail_Array::ArrayBase<N, Type> Base;

	public:
		// Using default assignment.
		// Using default destructor.

		//using Base::Dimension;
		using typename Base::Element;
		using typename Base::Reference;
		using typename Base::ConstReference;
		using typename Base::Cursor;
		using typename Base::ConstCursor;

		Array()
			: Base()
		{
		}

		Array(const Array& that)
			: Base(that)
		{
		}

		Array(
			const Vector<N, integer>& extent,
			const Alias<Type*>& dataAlias)
			: Base(extent, dataAlias)
		{
		}

		explicit Array(
			const Vector<N, integer>& extent,
			const Type& defaultData = Type())
			: Base(extent, defaultData)
		{
		}

		Array(
			const Array<N, Type>& that,
			const Vector<N, integer>& extent,
			const Type& defaultData = Type())
			: Base(that, extent, defaultData)
		{
		}
	};

	template <typename Type>
	class Array<1, Type>
		: public Detail_Array::ArrayBase<1, Type>
	{
	private:
		enum
		{
			N = 1
		};

		typedef Detail_Array::ArrayBase<N, Type> Base;

	public:
		// Using default assignment.
		// Using default destructor.

		//using Base::Dimension;
		using typename Base::Element;
		using typename Base::Reference;
		using typename Base::ConstReference;
		using typename Base::Cursor;
		using typename Base::ConstCursor;
		using Base::cursor;
		using Base::constCursor;
		using Base::operator();
		using Base::setExtent;
		using Base::reshape;

		Array()
			: Base()
		{
		}

		Array(const Array& that)
			: Base(that)
		{
		}

		Array(
			const Vector<N, integer>& extent,
			const Alias<Type*>& dataAlias)
			: Base(extent, dataAlias)
		{
		}

		explicit Array(
			const Vector<N, integer>& extent,
			const Type& defaultData = Type())
			: Base(extent, defaultData)
		{
		}

		/*
		Array(
			integer width,
			const Type& defaultData = Type())
			: Base(Vector<N, integer>(width), defaultData)
		{
		}
		*/

		Array(
			const Array<N, Type>& that,
			const Vector<N, integer>& extent,
			const Type& defaultData = Type())
			: Base(that, extent, defaultData)
		{
		}

		Array(
			const Array<N, Type>& that,
			integer width,
			const Type& defaultData = Type())
			: Base(that, Vector<N, integer>(width), defaultData)
		{
		}

		Array(
			integer width,
			const Alias<Type*> dataAlias)
			: Base(Vector<N, integer>(width), dataAlias)
		{
		}

		void setExtent(integer width,
			const Type& defaultData = Type())
		{
			Base::setExtent(Vector<N, integer>(width), defaultData);
		}

		integer width() const
		{
			return Base::extent()[0];
		}

		void reshape(integer width)
		{
			Base::reshape(Vector<N, integer>(width));
		}

		Type& operator()(integer x)
		{
			return Base::operator()(Point<N, integer>(x));
		}

		const Type& operator()(integer x) const
		{
			return Base::operator()(Point<N, integer>(x));
		}

		Cursor cursor(integer x)
		{
			return Base::cursor(Point<N, integer>(x));
		}

		ConstCursor constCursor(integer x) const
		{
			return Base::constCursor(Point<N, integer>(x));
		}
	};

	template <typename Type>
	class Array<2, Type>
		: public Detail_Array::ArrayBase<2, Type>
	{
	private:
		enum
		{
			N = 2
		};

		typedef Detail_Array::ArrayBase<N, Type> Base;

	public:
		// Using default assignment.
		// Using default destructor.

		//using Base::Dimension;
		using typename Base::Element;
		using typename Base::Reference;
		using typename Base::ConstReference;
		using typename Base::Cursor;
		using typename Base::ConstCursor;
		using Base::cursor;
		using Base::constCursor;
		using Base::operator();
		using Base::setExtent;
		using Base::reshape;

		Array()
			: Base()
		{
		}

		Array(const Array& that)
			: Base(that)
		{
		}

		Array(
			const Vector<N, integer>& extent,
			const Alias<Type*>& dataAlias)
			: Base(extent, dataAlias)
		{
		}

		explicit Array(
			const Vector<N, integer>& extent,
			const Type& defaultData = Type())
			: Base(extent, defaultData)
		{
		}

		Array(
			integer width, integer height,
			const Type& defaultData = Type())
			: Base(Vector<N, integer>(width, height), defaultData)
		{
		}

		Array(
			integer width, integer height,
			const Alias<Type*> dataAlias)
			: Base(Vector<N, integer>(width, height), dataAlias)
		{
		}

		Array(
			const Array<N, Type>& that,
			const Vector<N, integer>& extent,
			const Type& defaultData = Type())
			: Base(that, extent, defaultData)
		{
		}

		Array(
			const Array<N, Type>& that,
			integer width, integer height,
			const Type& defaultData = Type())
			: Base(that, Vector<N, integer>(width, height), defaultData)
		{
		}

		void setExtent(integer width, integer height,
			const Type& defaultData = Type())
		{
			Base::setExtent(Vector<N, integer>(width, height), defaultData);
		}

		integer width() const
		{
			return Base::extent()[0];
		}

		integer height() const
		{
			return Base::extent()[1];
		}

		void reshape(integer width, integer height)
		{
			Base::reshape(Vector<N, integer>(width, height));
		}

		Type& operator()(integer x, integer y)
		{
			return Base::operator()(Point<N, integer>(x, y));
		}

		const Type& operator()(integer x, integer y) const
		{
			return Base::operator()(Point<N, integer>(x, y));
		}

		Cursor cursor(integer x, integer y)
		{
			return Base::cursor(Point<N, integer>(x, y));
		}

		ConstCursor constCursor(integer x, integer y) const
		{
			return Base::constCursor(Point<N, integer>(x, y));
		}
	};

	template <typename Type>
	class Array<3, Type>
		: public Detail_Array::ArrayBase<3, Type>
	{
	private:
		enum
		{
			N = 3
		};

		typedef Detail_Array::ArrayBase<N, Type> Base;

	public:
		// Using default assignment.
		// Using default destructor.

		//using Base::Dimension;
		using typename Base::Element;
		using typename Base::Reference;
		using typename Base::ConstReference;
		using typename Base::Cursor;
		using typename Base::ConstCursor;
		using Base::cursor;
		using Base::constCursor;
		using Base::operator();
		using Base::setExtent;
		using Base::reshape;

		Array()
			: Base()
		{
		}

		Array(const Array& that)
			: Base(that)
		{
		}

		Array(
			const Vector<N, integer>& extent,
			const Alias<Type*>& dataAlias)
			: Base(extent, dataAlias)
		{
		}

		explicit Array(
			const Vector<N, integer>& extent,
			const Type& defaultData = Type())
			: Base(extent, defaultData)
		{
		}

		Array(
			integer width, integer height, integer depth,
			const Type& defaultData = Type())
			: Base(Vector<N, integer>(width, height, depth), defaultData)
		{
		}

		Array(
			const Array<N, Type>& that,
			const Vector<N, integer>& extent,
			const Type& defaultData = Type())
			: Base(that, extent, defaultData)
		{
		}

		Array(
			const Array<N, Type>& that,
			integer width, integer height, integer depth,
			const Type& defaultData = Type())
			: Base(that, Vector<N, integer>(width, height, depth), defaultData)
		{
		}

		Array(
			integer width, integer height, integer depth,
			const Alias<Type*> dataAlias)
			: Base(Vector<N, integer>(width, height, depth), dataAlias)
		{
		}

		void setExtent(integer width, integer height, integer depth,
			const Type& defaultData = Type())
		{
			Base::setExtent(Vector<N, integer>(width, height, depth), defaultData);
		}

		integer width() const
		{
			return Base::extent()[0];
		}

		integer height() const
		{
			return Base::extent()[1];
		}

		integer depth() const
		{
			return Base::extent()[2];
		}

		void reshape(integer width, integer height, integer depth)
		{
			Base::reshape(Vector<N, integer>(width, height, depth));
		}

		Type& operator()(integer x, integer y, integer z)
		{
			return Base::operator()(Point<N, integer>(x, y, z));
		}

		const Type& operator()(integer x, integer y, integer z) const
		{
			return Base::operator()(Point<N, integer>(x, y, z));
		}

		Cursor cursor(integer x, integer y, integer z)
		{
			return Base::cursor(Point<N, integer>(x, y, z));
		}

		ConstCursor constCursor(integer x, integer y, integer z) const
		{
			return Base::constCursor(Point<N, integer>(x, y, z));
		}
	};

	template <int N, typename Type>
	void swap(Array<N, Type>& left, Array<N, Type>& right);

}

#include "pastel/sys/array.hpp"

#endif
