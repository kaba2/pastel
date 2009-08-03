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

	template <int N, typename Real>
	class AlignedBox
		: public AlignedBoxBase<N, Real>
	{
	private:
		typedef AlignedBoxBase<N, Real> Base;

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
	class AlignedBox<1, Real>
		: public AlignedBoxBase<1, Real>
	{
	private:
		enum
		{
			N = 1
		};

		typedef AlignedBoxBase<N, Real> Base;

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
	class AlignedBox<2, Real>
		: public AlignedBoxBase<2, Real>
	{
	private:
		enum
		{
			N = 2
		};

		typedef AlignedBoxBase<N, Real> Base;

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
	class AlignedBox<3, Real>
		: public AlignedBoxBase<3, Real>
	{
	private:
		enum
		{
			N = 3
		};

		typedef AlignedBoxBase<N, Real> Base;

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
	class AlignedBox<4, Real>
		: public AlignedBoxBase<4, Real>
	{
	private:
		enum
		{
			N = 4
		};

		typedef AlignedBoxBase<N, Real> Base;

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
	class AlignedBox<Dynamic, Real>
		: public AlignedBoxBase<Dynamic, Real>
	{
	private:
		enum
		{
			N = Dynamic
		};

		typedef AlignedBoxBase<N, Real> Base;

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

	template <int N, typename Real>
	void swap(AlignedBox<N, Real>& left,
		AlignedBox<N, Real>& right);

	typedef AlignedBox<1, real> AlignedBox1;
	typedef AlignedBox<2, real> AlignedBox2;
	typedef AlignedBox<3, real> AlignedBox3;
	typedef AlignedBox<4, real> AlignedBox4;
	typedef AlignedBox<Dynamic, real> AlignedBoxD;

}

#include "pastel/geometry/alignedbox.hpp"

#endif
