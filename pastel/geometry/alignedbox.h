// Description: AlignedBox class
// Detail: Allows to work with axis aligned boxes
// Documentation: shapes.txt

#ifndef PASTEL_ALIGNEDBOX_H
#define PASTEL_ALIGNEDBOX_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/point.h"
#include "pastel/sys/vector.h"

#include "pastel/geometry/alignedboxbase.h"

namespace Pastel
{

	template <typename Real, int N>
	class AlignedBox
		: public AlignedBoxBase<Real, N>
	{
	private:
		typedef AlignedBoxBase<Real, N> Base;

	public:
		AlignedBox()
			: Base()
		{
		}

		explicit AlignedBox(integer dimension)
			: Base(dimension)
		{
		}

		explicit AlignedBox(const Point<Real, N>& that)
			: Base(that)
		{
		}

		AlignedBox(const Point<Real, N>& min,
			const Point<Real, N>& max)
			: Base(min, max)
		{
		}
	};

	template <typename Real>
	class AlignedBox<Real, 1>
		: public AlignedBoxBase<Real, 1>
	{
	private:
		enum
		{
			N = 1
		};

		typedef AlignedBoxBase<Real, N> Base;

	public:
		AlignedBox()
			: Base()
		{
		}

		explicit AlignedBox(integer dimension)
			: Base(dimension)
		{
		}

		explicit AlignedBox(const Point<Real, N>& that)
			: Base(that)
		{
		}

		AlignedBox(const Real& xMin, const Real& xMax)
			: Base(Point<Real, N>(xMin),
			Point<Real, N>(xMax))
		{
		}

		AlignedBox(const Point<Real, N>& min,
			const Point<Real, N>& max)
			: Base(min, max)
		{
		}

		using Base::set;

		void set(const Real& xMin, const Real& xMax)
		{
			set(Point<Real, N>(xMin), Point<Real, N>(xMax));
		}
	};

	template <typename Real>
	class AlignedBox<Real, 2>
		: public AlignedBoxBase<Real, 2>
	{
	private:
		enum
		{
			N = 2
		};

		typedef AlignedBoxBase<Real, N> Base;

	public:
		AlignedBox()
			: Base()
		{
		}

		explicit AlignedBox(integer dimension)
			: Base(dimension)
		{
		}

		explicit AlignedBox(const Point<Real, N>& that)
			: Base(that)
		{
		}

		AlignedBox(const Real& xMin, const Real& yMin, const Real& xMax, const Real& yMax)
			: Base(Point<Real, N>(xMin, yMin),
			Point<Real, N>(xMax, yMax))
		{
		}

		AlignedBox(const Point<Real, N>& min,
			const Point<Real, N>& max)
			: Base(min, max)
		{
		}

		using Base::set;

		void set(const Real& xMin, const Real& yMin,
			const Real& xMax, const Real& yMax)
		{
			set(Point<Real, N>(xMin, yMin),
				Point<Real, N>(xMax, yMax));
		}
	};

	template <typename Real>
	class AlignedBox<Real, 3>
		: public AlignedBoxBase<Real, 3>
	{
	private:
		enum
		{
			N = 3
		};

		typedef AlignedBoxBase<Real, N> Base;

	public:
		AlignedBox()
			: Base()
		{
		}

		explicit AlignedBox(integer dimension)
			: Base(dimension)
		{
		}

		explicit AlignedBox(const Point<Real, N>& that)
			: Base(that)
		{
		}

		AlignedBox(const Real& xMin, const Real& yMin, const Real& zMin,
			const Real& xMax, const Real& yMax, const Real& zMax)
			: Base(Point<Real, N>(xMin, yMin, zMin),
			Point<Real, N>(xMax, yMax, zMax))
		{
		}

		AlignedBox(const Point<Real, N>& min,
			const Point<Real, N>& max)
			: Base(min, max)
		{
		}

		using Base::set;

		void set(const Real& xMin, const Real& yMin, const Real& zMin,
			const Real& xMax, const Real& yMax, const Real& zMax)
		{
			set(Point<Real, N>(xMin, yMin, zMin),
				Point<Real, N>(xMax, yMax, zMax));
		}
	};

	template <typename Real>
	class AlignedBox<Real, 4>
		: public AlignedBoxBase<Real, 4>
	{
	private:
		enum
		{
			N = 4
		};

		typedef AlignedBoxBase<Real, N> Base;

	public:
		AlignedBox()
			: Base()
		{
		}

		explicit AlignedBox(integer dimension)
			: Base(dimension)
		{
		}

		explicit AlignedBox(const Point<Real, N>& that)
			: Base(that)
		{
		}

		AlignedBox(const Real& xMin, const Real& yMin, const Real& zMin, const Real& wMin,
			const Real& xMax, const Real& yMax, const Real& zMax, const Real& wMax)
			: Base(Point<Real, N>(xMin, yMin, zMin, wMin),
			Point<Real, N>(xMax, yMax, zMax, wMax))
		{
		}

		AlignedBox(const Point<Real, N>& min,
			const Point<Real, N>& max)
			: Base(min, max)
		{
		}

		using Base::set;

		void set(const Real& xMin, const Real& yMin, const Real& zMin, const Real& wMin,
			const Real& xMax, const Real& yMax, const Real& zMax, const Real& wMax)
		{
			set(Point<Real, N>(xMin, yMin, zMin, wMin),
				Point<Real, N>(xMax, yMax, zMax, wMax));
		}
	};

	template <typename Real>
	class AlignedBox<Real, Dynamic>
		: public AlignedBoxBase<Real, Dynamic>
	{
	private:
		enum
		{
			N = Dynamic
		};

		typedef AlignedBoxBase<Real, N> Base;

		// We require the dimension to be
		// specified at construction for
		// unbounded vectors.
		// Prohibited
		AlignedBox();
	public:

		explicit AlignedBox(integer dimension)
			: Base(dimension)
		{
		}

		explicit AlignedBox(const Point<Real, N>& that)
			: Base(that)
		{
		}

		AlignedBox(const Point<Real, N>& min,
			const Point<Real, N>& max)
			: Base(min, max)
		{
		}
	};

	template <typename Real, int N>
	void swap(AlignedBox<Real, N>& left,
		AlignedBox<Real, N>& right);

	typedef AlignedBox<real, 1> AlignedBox1;
	typedef AlignedBox<real, 2> AlignedBox2;
	typedef AlignedBox<real, 3> AlignedBox3;
	typedef AlignedBox<real, 4> AlignedBox4;
	typedef AlignedBox<real, Dynamic> AlignedBoxD;

}

#include "pastel/geometry/alignedbox.hpp"

#endif
