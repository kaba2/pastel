/*!
\file
\brief A class for an axis-aligned bounding box.
*/

#ifndef PASTELGEOMETRY_ALIGNEDBOX_H
#define PASTELGEOMETRY_ALIGNEDBOX_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/point.h"
#include "pastel/sys/vector.h"

#include "pastel/geometry/alignedboxbase.h"

namespace Pastel
{

	template <int N, typename Real>
	class AlignedBox
		: public AlignedBoxBase<N, Real, AlignedBox<N, Real> >
	{
	public:
		typedef AlignedBoxBase<N, Real, AlignedBox<N, Real> > Base;

		AlignedBox()
			: Base()
		{
		}

		explicit AlignedBox(const Point<N, Real>& that)
			: Base(that)
		{
		}

		AlignedBox(const Point<N, Real>& min,
			const Point<N, Real>& max)
			: Base(min, max)
		{
		}
	};

	template <typename Real>
	class AlignedBox<1, Real>
		: public AlignedBoxBase<1, Real, AlignedBox<1, Real> >
	{
	private:
		enum
		{
			N = 1
		};

	public:
		typedef AlignedBoxBase<N, Real, AlignedBox<N, Real> > Base;

		AlignedBox()
			: Base()
		{
		}

		explicit AlignedBox(const Point<N, Real>& that)
			: Base(that)
		{
		}

		AlignedBox(const Real& xMin, const Real& xMax)
			: Base(Point<N, Real>(xMin),
			Point<N, Real>(xMax))
		{
		}

		AlignedBox(const Point<N, Real>& min,
			const Point<N, Real>& max)
			: Base(min, max)
		{
		}

		using Base::set;

		void set(const Real& xMin, const Real& xMax)
		{
			set(Point1(xMin), Point1(xMax));
		}
	};

	template <typename Real>
	class AlignedBox<2, Real>
		: public AlignedBoxBase<2, Real, AlignedBox<2, Real> >
	{
	private:
		enum
		{
			N = 2
		};

	public:
		typedef AlignedBoxBase<N, Real, AlignedBox<N, Real> > Base;

		AlignedBox()
			: Base()
		{
		}

		explicit AlignedBox(const Point<N, Real>& that)
			: Base(that)
		{
		}

		AlignedBox(const Real& xMin, const Real& yMin, const Real& xMax, const Real& yMax)
			: Base(Point<N, Real>(xMin, yMin),
			Point<N, Real>(xMax, yMax))
		{
		}

		AlignedBox(const Point<N, Real>& min,
			const Point<N, Real>& max)
			: Base(min, max)
		{
		}

		using Base::set;

		void set(const Real& xMin, const Real& yMin,
			const Real& xMax, const Real& yMax)
		{
			set(Point2(xMin, yMin),
				Point2(xMax, yMax));
		}
	};

	template <typename Real>
	class AlignedBox<3, Real>
		: public AlignedBoxBase<3, Real, AlignedBox<3, Real> >
	{
	private:
		enum
		{
			N = 3
		};

	public:
		typedef AlignedBoxBase<N, Real, AlignedBox<N, Real> > Base;

		AlignedBox()
			: Base()
		{
		}

		explicit AlignedBox(const Point<N, Real>& that)
			: Base(that)
		{
		}

		AlignedBox(const Real& xMin, const Real& yMin, const Real& zMin,
			const Real& xMax, const Real& yMax, const Real& zMax)
			: Base(Point<N, Real>(xMin, yMin, zMin),
			Point<N, Real>(xMax, yMax, zMax))
		{
		}

		AlignedBox(const Point<N, Real>& min,
			const Point<N, Real>& max)
			: Base(min, max)
		{
		}

		using Base::set;

		void set(const Real& xMin, const Real& yMin, const Real& zMin,
			const Real& xMax, const Real& yMax, const Real& zMax)
		{
			set(Point3(xMin, yMin, zMin),
				Point3(xMax, yMax, zMax));
		}
	};

	template <int N, typename Real>
	void swap(AlignedBox<N, Real>& left,
		AlignedBox<N, Real>& right);

	typedef AlignedBox<1, real> AlignedBox1;
	typedef AlignedBox<2, real> AlignedBox2;
	typedef AlignedBox<3, real> AlignedBox3;
	typedef AlignedBox<4, real> AlignedBox4;

}

#include "pastel/geometry/alignedbox.hpp"

#endif
