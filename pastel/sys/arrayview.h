#ifndef PASTELSYS_ARRAYVIEW_H
#define PASTELSYS_ARRAYVIEW_H

#include "pastel/sys/view.h"

namespace Pastel
{

	template <int N, typename Contained_Array>
	class ConstArrayView
	{
	public:
		enum
		{
			Dimension = N
		};

		typedef typename Contained_Array::Element Element;
		typedef typename Contained_Array::ConstCursor ConstCursor;
		typedef typename Contained_Array::ConstReference ConstReference;

		ConstArrayView(const Contained_Array& array)
			: array_(&array)
		{
		}

		const Vector<N, integer>& extent() const
		{
			return array_->extent();
		}

		const Element& operator()(const Point<N, integer>& position) const
		{
			return (*array_)(position);
		}
		
		ConstCursor constCursor(const Point<N, integer>& position) const
		{
			return const_cast<Contained_Array*>(array_)->constCursor(position);
		}

	protected:
		const Contained_Array* array_;
	};

	template <int N, typename Contained_Array>
	class ArrayView
		: public ConstArrayView<N, Contained_Array>
	{
	private:
		typedef ConstArrayView<N, Contained_Array> Base;
		using Base::array_;

	public:
		//using Base::Dimension;
		using typename Base::Element;
		using typename Base::ConstReference;
		using typename Base::ConstCursor;
		using Base::constCursor;
		using Base::extent;

		typedef typename Contained_Array::Cursor Cursor;
		typedef typename Contained_Array::Reference Reference;

		ArrayView(Contained_Array& array)
			: Base(array)
		{
		}

		Element& operator()(const Point<N, integer>& position) const
		{
			return (*array_)(position);
		}
		
		Cursor cursor(const Point<N, integer>& position) const
		{
			return const_cast<Contained_Array*>(array_)->cursor(position);
		}
	};

	template <typename Array>
	View<Array::Dimension, typename Array::Element, ArrayView<Array::Dimension, Array> > 
		arrayView(Array& that)
	{
		return wrapView(ArrayView<Array::Dimension, Array>(that));
	}

	template <typename Array>
	ConstView<Array::Dimension, typename Array::Element, ConstArrayView<Array::Dimension, Array> > 
		constArrayView(const Array& that)
	{
		return wrapConstView(ConstArrayView<Array::Dimension, Array>(that));
	}

}

#endif
