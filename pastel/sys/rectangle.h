/*!
\file
\brief A class for an axis-aligned bounding box.
*/

#ifndef PASTEL_RECTANGLE_H
#define PASTEL_RECTANGLE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/point.h"
#include "pastel/sys/vector.h"

#include "pastel/sys/rectanglebase.h"

namespace Pastel
{

	template <int N>
	class Rectangle
		: public RectangleBase<N, Rectangle<N> >
	{
	public:
		typedef RectangleBase<N, Rectangle<N> > Base;

		Rectangle()
			: Base()
		{
		}

		explicit Rectangle(const Point<N, integer>& that)
			: Base(that)
		{
		}

		Rectangle(const Point<N, integer>& min,
			const Point<N, integer>& max)
			: Base(min, max)
		{
		}
	};

	template <>
	class Rectangle<1>
		: public RectangleBase<1, Rectangle<1> >
	{
	private:
		enum
		{
			N = 1
		};

	public:
		typedef RectangleBase<N, Rectangle<N> > Base;

		Rectangle()
			: Base()
		{
		}

		explicit Rectangle(const Point<N, integer>& that)
			: Base(that)
		{
		}

		Rectangle(integer xMin, integer xMax)
			: Base(Point<N, integer>(xMin),
			Point<N, integer>(xMax))
		{
		}

		Rectangle(const Point<N, integer>& min,
			const Point<N, integer>& max)
			: Base(min, max)
		{
		}

		using Base::set;
		using Base::min;
		using Base::max;

		void set(integer xMin, integer xMax)
		{
			set(Point1i(xMin), Point1i(xMax));
		}

		integer& xMin()
		{
			return min()[0];
		}

		integer& xMax()
		{
			return max()[0];
		}

		integer xMin() const
		{
			return min()[0];
		}

		integer xMax() const
		{
			return max()[0];
		}

		integer width() const
		{
			return max()[0] - min()[0];
		}
	};

	template <>
	class Rectangle<2>
		: public RectangleBase<2, Rectangle<2> >
	{
	private:
		enum
		{
			N = 2
		};

	public:
		typedef RectangleBase<N, Rectangle<N> > Base;

		Rectangle()
			: Base()
		{
		}

		explicit Rectangle(const Point<N, integer>& that)
			: Base(that)
		{
		}

		Rectangle(integer xMin, integer yMin, integer xMax, integer yMax)
			: Base(Point<N, integer>(xMin, yMin),
			Point<N, integer>(xMax, yMax))
		{
		}

		Rectangle(const Point<N, integer>& min,
			const Point<N, integer>& max)
			: Base(min, max)
		{
		}

		using Base::set;
		using Base::min;
		using Base::max;

		void set(integer xMin, integer yMin,
			integer xMax, integer yMax)
		{
			set(Point2i(xMin, yMin),
				Point2i(xMax, yMax));
		}

		integer& xMin()
		{
			return min()[0];
		}

		integer& yMin()
		{
			return min()[1];
		}

		integer& xMax()
		{
			return max()[0];
		}

		integer& yMax()
		{
			return max()[1];
		}

		integer xMin() const
		{
			return min()[0];
		}

		integer yMin() const
		{
			return min()[1];
		}

		integer xMax() const
		{
			return max()[0];
		}

		integer yMax() const
		{
			return max()[1];
		}

		integer width() const
		{
			return max()[0] - min()[0];
		}

		integer height() const
		{
			return max()[1] - min()[1];
		}
	};

	template <>
	class Rectangle<3>
		: public RectangleBase<3, Rectangle<3> >
	{
	private:
		enum
		{
			N = 3
		};

	public:
		typedef RectangleBase<N, Rectangle<N> > Base;

		Rectangle()
			: Base()
		{
		}

		explicit Rectangle(const Point<N, integer>& that)
			: Base(that)
		{
		}

		Rectangle(integer xMin, integer yMin, integer zMin,
			integer xMax, integer yMax, integer zMax)
			: Base(Point<N, integer>(xMin, yMin, zMin),
			Point<N, integer>(xMax, yMax, zMax))
		{
		}

		Rectangle(const Point<N, integer>& min,
			const Point<N, integer>& max)
			: Base(min, max)
		{
		}

		using Base::set;
		using Base::min;
		using Base::max;

		void set(integer xMin, integer yMin, integer zMin,
			integer xMax, integer yMax, integer zMax)
		{
			set(Point3i(xMin, yMin, zMin),
				Point3i(xMax, yMax, zMax));
		}

		integer& xMin()
		{
			return min()[0];
		}

		integer& yMin()
		{
			return min()[1];
		}

		integer& zMin()
		{
			return min()[2];
		}

		integer& xMax()
		{
			return max()[0];
		}

		integer& yMax()
		{
			return max()[1];
		}

		integer& zMax()
		{
			return max()[2];
		}

		integer xMin() const
		{
			return min()[0];
		}

		integer yMin() const
		{
			return min()[1];
		}

		integer zMin() const
		{
			return min()[2];
		}

		integer xMax() const
		{
			return max()[0];
		}

		integer yMax() const
		{
			return max()[1];
		}

		integer zMax() const
		{
			return max()[2];
		}

		integer width() const
		{
			return max()[0] - min()[0];
		}

		integer height() const
		{
			return max()[1] - min()[1];
		}

		integer depth() const
		{
			return max()[2] - min()[2];
		}
	};

	template <int N>
	void swap(Rectangle<N>& left,
		Rectangle<N>& right);

	typedef Rectangle<1> Rectangle1;
	typedef Rectangle<2> Rectangle2;
	typedef Rectangle<3> Rectangle3;
	typedef Rectangle<4> Rectangle4;

}

#include "pastel/sys/rectangle.hpp"

#endif
