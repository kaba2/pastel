// Description: Array class
// Detail: An n-dimensional array

#ifndef PASTEL_ARRAY_H
#define PASTEL_ARRAY_H

#include "pastel/sys/arraybase.h"

namespace Pastel
{

	//! A multi-dimensional array.
	template <typename Type, int N>
	class Array
		: public Detail_Array::ArrayBase<Type, N>
	{
	private:
		typedef Detail_Array::ArrayBase<Type, N> Base;

	public:
		// Using default assignment.
		// Using default destructor.

		//using Base::Dimension;
		using typename Base::Element;
		using typename Base::Reference;
		using typename Base::ConstReference;
		using typename Base::Cursor;
		using typename Base::ConstCursor;
		using Base::operator=;

		Array()
			: Base()
		{
		}

		Array(const Array& that)
			: Base(that)
		{
		}

		Array(
			const Vector<integer, N>& extent,
			const Alias<Type*>& dataAlias)
			: Base(extent, dataAlias)
		{
		}

		explicit Array(
			const Vector<integer, N>& extent,
			const Type& defaultData = Type())
			: Base(extent, defaultData)
		{
		}

		Array(
			const Array<Type, N>& that,
			const Vector<integer, N>& extent,
			const Type& defaultData = Type())
			: Base(that, extent, defaultData)
		{
		}
	};

	template <typename Type>
	class Array<Type, 1>
		: public Detail_Array::ArrayBase<Type, 1>
	{
	private:
		enum
		{
			N = 1
		};

		typedef Detail_Array::ArrayBase<Type, N> Base;

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
		using Base::operator=;
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
			const Vector<integer, N>& extent,
			const Alias<Type*>& dataAlias)
			: Base(extent, dataAlias)
		{
		}

		explicit Array(
			const Vector<integer, N>& extent,
			const Type& defaultData = Type())
			: Base(extent, defaultData)
		{
		}

		/*
		Array(
			integer width,
			const Type& defaultData = Type())
			: Base(Vector<integer, N>(width), defaultData)
		{
		}
		*/

		Array(
			const Array<Type, N>& that,
			const Vector<integer, N>& extent,
			const Type& defaultData = Type())
			: Base(that, extent, defaultData)
		{
		}

		Array(
			const Array<Type, N>& that,
			integer width,
			const Type& defaultData = Type())
			: Base(that, Vector<integer, N>(width), defaultData)
		{
		}

		Array(
			integer width,
			const Alias<Type*> dataAlias)
			: Base(Vector<integer, N>(width), dataAlias)
		{
		}

		void setExtent(integer width,
			const Type& defaultData = Type())
		{
			Base::setExtent(Vector<integer, N>(width), defaultData);
		}

		integer width() const
		{
			return Base::extent()[0];
		}

		void reshape(integer width)
		{
			Base::reshape(Vector<integer, N>(width));
		}

		Type& operator()(integer x)
		{
			return Base::operator()(Vector<integer, N>(x));
		}

		const Type& operator()(integer x) const
		{
			return Base::operator()(Vector<integer, N>(x));
		}

		Cursor cursor(integer x)
		{
			return Base::cursor(Vector<integer, N>(x));
		}

		ConstCursor constCursor(integer x) const
		{
			return Base::constCursor(Vector<integer, N>(x));
		}
	};

	template <typename Type>
	class Array<Type, 2>
		: public Detail_Array::ArrayBase<Type, 2>
	{
	private:
		enum
		{
			N = 2
		};

		typedef Detail_Array::ArrayBase<Type, N> Base;

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
		using Base::operator=;
		using Base::setExtent;
		using Base::reshape;
		using typename Base::RowIterator;
		using typename Base::ConstRowIterator;

		Array()
			: Base()
		{
		}

		Array(const Array& that)
			: Base(that)
		{
		}

		Array(
			const Vector<integer, N>& extent,
			const Alias<Type*>& dataAlias)
			: Base(extent, dataAlias)
		{
		}

		explicit Array(
			const Vector<integer, N>& extent,
			const Type& defaultData = Type())
			: Base(extent, defaultData)
		{
		}

		Array(
			integer width, integer height,
			const Type& defaultData = Type())
			: Base(Vector<integer, N>(width, height), defaultData)
		{
		}

		Array(
			integer width, integer height,
			const Alias<Type*> dataAlias)
			: Base(Vector<integer, N>(width, height), dataAlias)
		{
		}

		Array(
			const Array<Type, N>& that,
			const Vector<integer, N>& extent,
			const Type& defaultData = Type())
			: Base(that, extent, defaultData)
		{
		}

		Array(
			const Array<Type, N>& that,
			integer width, integer height,
			const Type& defaultData = Type())
			: Base(that, Vector<integer, N>(width, height), defaultData)
		{
		}

		void setExtent(integer width, integer height,
			const Type& defaultData = Type())
		{
			Base::setExtent(Vector<integer, N>(width, height), defaultData);
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
			Base::reshape(Vector<integer, N>(width, height));
		}

		Type& operator()(integer x, integer y)
		{
			return Base::operator()(Vector<integer, N>(x, y));
		}

		const Type& operator()(integer x, integer y) const
		{
			return Base::operator()(Vector<integer, N>(x, y));
		}

		Cursor cursor(integer x, integer y)
		{
			return Base::cursor(Vector<integer, N>(x, y));
		}

		ConstCursor constCursor(integer x, integer y) const
		{
			return Base::constCursor(Vector<integer, N>(x, y));
		}

		RowIterator rowBegin(integer y)
		{
			return Base::rowBegin(Vector<integer, N>(0, y), 0);
		}

		ConstRowIterator rowBegin(integer y) const
		{
			return Base::rowBegin(Vector<integer, N>(0, y), 0);
		}

		RowIterator rowEnd(integer y)
		{
			return Base::rowEnd(Vector<integer, N>(0, y), 0);
		}

		ConstRowIterator rowEnd(integer y) const
		{
			return Base::rowEnd(Vector<integer, N>(0, y), 0);
		}

		RowIterator columnBegin(integer x)
		{
			return Base::rowBegin(Vector<integer, N>(x, 0), 1);
		}

		ConstRowIterator columnBegin(integer x) const
		{
			return Base::rowBegin(Vector<integer, N>(x, 0), 1);
		}

		RowIterator columnEnd(integer x)
		{
			return Base::rowEnd(Vector<integer, N>(x, 0), 1);
		}

		ConstRowIterator columnEnd(integer x) const
		{
			return Base::rowEnd(Vector<integer, N>(x, 0), 1);
		}
	};

	template <typename Type>
	class Array<Type, 3>
		: public Detail_Array::ArrayBase<Type, 3>
	{
	private:
		enum
		{
			N = 3
		};

		typedef Detail_Array::ArrayBase<Type, N> Base;

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
		using Base::operator=;
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
			const Vector<integer, N>& extent,
			const Alias<Type*>& dataAlias)
			: Base(extent, dataAlias)
		{
		}

		explicit Array(
			const Vector<integer, N>& extent,
			const Type& defaultData = Type())
			: Base(extent, defaultData)
		{
		}

		Array(
			integer width, integer height, integer depth,
			const Type& defaultData = Type())
			: Base(Vector<integer, N>(width, height, depth), defaultData)
		{
		}

		Array(
			const Array<Type, N>& that,
			const Vector<integer, N>& extent,
			const Type& defaultData = Type())
			: Base(that, extent, defaultData)
		{
		}

		Array(
			const Array<Type, N>& that,
			integer width, integer height, integer depth,
			const Type& defaultData = Type())
			: Base(that, Vector<integer, N>(width, height, depth), defaultData)
		{
		}

		Array(
			integer width, integer height, integer depth,
			const Alias<Type*> dataAlias)
			: Base(Vector<integer, N>(width, height, depth), dataAlias)
		{
		}

		void setExtent(integer width, integer height, integer depth,
			const Type& defaultData = Type())
		{
			Base::setExtent(Vector<integer, N>(width, height, depth), defaultData);
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
			Base::reshape(Vector<integer, N>(width, height, depth));
		}

		Type& operator()(integer x, integer y, integer z)
		{
			return Base::operator()(Vector<integer, N>(x, y, z));
		}

		const Type& operator()(integer x, integer y, integer z) const
		{
			return Base::operator()(Vector<integer, N>(x, y, z));
		}

		Cursor cursor(integer x, integer y, integer z)
		{
			return Base::cursor(Vector<integer, N>(x, y, z));
		}

		ConstCursor constCursor(integer x, integer y, integer z) const
		{
			return Base::constCursor(Vector<integer, N>(x, y, z));
		}
	};

	template <typename Type, int N>
	void swap(Array<Type, N>& left, Array<Type, N>& right);

}

#include "pastel/sys/array.hpp"

#endif
