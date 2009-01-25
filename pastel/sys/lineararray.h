/*!
\file
\brief A class for an n-dimensional array.
*/

#ifndef PASTELSYS_LINEARARRAY_H
#define PASTELSYS_LINEARARRAY_H

#include "pastel/sys/lineararraybase.h"

namespace Pastel
{

	template <int N, typename Type>
	class LinearArray
		: public Detail_LinearArray::LinearArrayBase<N, Type, LinearArray<N, Type> >
	{
	private:
		typedef Detail_LinearArray::LinearArrayBase<N, Type, LinearArray<N, Type> > Base;

	public:
		// Using default assignment.
		// Using default destructor.

		//using Base::Dimension;
		using typename Base::Element;
		using typename Base::Reference;
		using typename Base::ConstReference;
		using typename Base::Cursor;
		using typename Base::ConstCursor;

		LinearArray()
			: Base()
		{
		}

		LinearArray(const LinearArray& that)
			: Base(that)
		{
		}

		LinearArray(
			const Vector<N, integer>& extent,
			const Type& defaultData = Type())
			: Base(extent, defaultData)
		{
		}

		LinearArray(
			const LinearArray<N, Type>& that,
			const Vector<N, integer>& extent,
			const Type& defaultData = Type())
			: Base(that, extent, defaultData)
		{
		}
	};

	template <typename Type>
	class LinearArray<1, Type>
		: public Detail_LinearArray::LinearArrayBase<1, Type, LinearArray<1, Type> >
	{
	private:
		enum
		{
			N = 1
		};

		typedef Detail_LinearArray::LinearArrayBase<N, Type, LinearArray<N, Type> > Base;

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

		LinearArray()
			: Base()
		{
		}

		LinearArray(const LinearArray& that)
			: Base(that)
		{
		}

		LinearArray(
			const Vector<N, integer>& extent,
			const Type& defaultData = Type())
			: Base(extent, defaultData)
		{
		}

		LinearArray(
			integer width,
			const Type& defaultData = Type())
			: Base(Vector<N, integer>(width), defaultData)
		{
		}

		LinearArray(
			const LinearArray<N, Type>& that,
			const Vector<N, integer>& extent,
			const Type& defaultData = Type())
			: Base(that, extent, defaultData)
		{
		}

		LinearArray(
			const LinearArray<N, Type>& that,
			integer width, 
			const Type& defaultData = Type())
			: Base(that, Vector<N, integer>(width), defaultData)
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
	class LinearArray<2, Type>
		: public Detail_LinearArray::LinearArrayBase<2, Type, LinearArray<2, Type> >
	{
	private:
		enum
		{
			N = 2
		};

		typedef Detail_LinearArray::LinearArrayBase<N, Type, LinearArray<N, Type> > Base;

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

		LinearArray()
			: Base()
		{
		}

		LinearArray(const LinearArray& that)
			: Base(that)
		{
		}

		LinearArray(
			const Vector<N, integer>& extent,
			const Type& defaultData = Type())
			: Base(extent, defaultData)
		{
		}

		LinearArray(
			integer width, integer height,
			const Type& defaultData = Type())
			: Base(Vector<N, integer>(width, height), defaultData)
		{
		}

		LinearArray(
			const LinearArray<N, Type>& that,
			const Vector<N, integer>& extent,
			const Type& defaultData = Type())
			: Base(that, extent, defaultData)
		{
		}

		LinearArray(
			const LinearArray<N, Type>& that,
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
	class LinearArray<3, Type>
		: public Detail_LinearArray::LinearArrayBase<3, Type, LinearArray<3, Type> >
	{
	private:
		enum
		{
			N = 3
		};

		typedef Detail_LinearArray::LinearArrayBase<N, Type, LinearArray<N, Type> > Base;

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

		LinearArray()
			: Base()
		{
		}

		LinearArray(const LinearArray& that)
			: Base(that)
		{
		}

		LinearArray(
			const Vector<N, integer>& extent,
			const Type& defaultData = Type())
			: Base(extent, defaultData)
		{
		}

		LinearArray(
			integer width, integer height, integer depth,
			const Type& defaultData = Type())
			: Base(Vector<N, integer>(width, height, depth), defaultData)
		{
		}

		LinearArray(
			const LinearArray<N, Type>& that,
			const Vector<N, integer>& extent,
			const Type& defaultData = Type())
			: Base(that, extent, defaultData)
		{
		}

		LinearArray(
			const LinearArray<N, Type>& that,
			integer width, integer height, integer depth,
			const Type& defaultData = Type())
			: Base(that, Vector<N, integer>(width, height, depth), defaultData)
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
	void swap(LinearArray<N, Type>& left, LinearArray<N, Type>& right);

}

#include "pastel/sys/lineararray.hpp"

#endif
