// Description: AlignedBox class
// Detail: Allows to work with axis-aligned boxes

#ifndef PASTELSYS_ALIGNEDBOX_H
#define PASTELSYS_ALIGNEDBOX_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include "pastel/sys/alignedboxbase.h"

namespace Pastel
{

	template <typename Real, int N = Dynamic>
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

		explicit AlignedBox(const Vector<Real, N>& that)
			: Base(that)
		{
		}

		AlignedBox(const Vector<Real, N>& min,
			const Vector<Real, N>& max)
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

		explicit AlignedBox(const Vector<Real, N>& that)
			: Base(that)
		{
		}

		AlignedBox(const Real& xMin, const Real& xMax)
			: Base(Vector<Real, N>(xMin),
			Vector<Real, N>(xMax))
		{
		}

		AlignedBox(const Vector<Real, N>& min,
			const Vector<Real, N>& max)
			: Base(min, max)
		{
		}

		using Base::set;

		void set(const Real& xMin, const Real& xMax)
		{
			set(Vector<Real, N>(xMin), Vector<Real, N>(xMax));
		}

		Real width() const
		{
			return Base::extent()[0];
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

		explicit AlignedBox(const Vector<Real, N>& that)
			: Base(that)
		{
		}

		AlignedBox(const Real& xMin, const Real& yMin, const Real& xMax, const Real& yMax)
			: Base(Vector<Real, N>(xMin, yMin),
			Vector<Real, N>(xMax, yMax))
		{
		}

		AlignedBox(const Vector<Real, N>& min,
			const Vector<Real, N>& max)
			: Base(min, max)
		{
		}

		using Base::set;

		void set(const Real& xMin, const Real& yMin,
			const Real& xMax, const Real& yMax)
		{
			set(Vector<Real, N>(xMin, yMin),
				Vector<Real, N>(xMax, yMax));
		}

		Real width() const
		{
			return Base::extent()[0];
		}

		Real height() const
		{
			return Base::extent()[1];
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

		explicit AlignedBox(const Vector<Real, N>& that)
			: Base(that)
		{
		}

		AlignedBox(const Real& xMin, const Real& yMin, const Real& zMin,
			const Real& xMax, const Real& yMax, const Real& zMax)
			: Base(Vector<Real, N>(xMin, yMin, zMin),
			Vector<Real, N>(xMax, yMax, zMax))
		{
		}

		AlignedBox(const Vector<Real, N>& min,
			const Vector<Real, N>& max)
			: Base(min, max)
		{
		}

		using Base::set;

		void set(const Real& xMin, const Real& yMin, const Real& zMin,
			const Real& xMax, const Real& yMax, const Real& zMax)
		{
			set(Vector<Real, N>(xMin, yMin, zMin),
				Vector<Real, N>(xMax, yMax, zMax));
		}

		Real width() const
		{
			return Base::extent()[0];
		}

		Real height() const
		{
			return Base::extent()[1];
		}

		Real depth() const
		{
			return Base::extent()[2];
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

		explicit AlignedBox(const Vector<Real, N>& that)
			: Base(that)
		{
		}

		AlignedBox(const Real& xMin, const Real& yMin, const Real& zMin, const Real& wMin,
			const Real& xMax, const Real& yMax, const Real& zMax, const Real& wMax)
			: Base(Vector<Real, N>(xMin, yMin, zMin, wMin),
			Vector<Real, N>(xMax, yMax, zMax, wMax))
		{
		}

		AlignedBox(const Vector<Real, N>& min,
			const Vector<Real, N>& max)
			: Base(min, max)
		{
		}

		using Base::set;

		void set(const Real& xMin, const Real& yMin, const Real& zMin, const Real& wMin,
			const Real& xMax, const Real& yMax, const Real& zMax, const Real& wMax)
		{
			set(Vector<Real, N>(xMin, yMin, zMin, wMin),
				Vector<Real, N>(xMax, yMax, zMax, wMax));
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
		AlignedBox() PASTEL_DELETE;
	public:

		explicit AlignedBox(integer dimension)
			: Base(dimension)
		{
		}

		explicit AlignedBox(const Vector<Real, N>& that)
			: Base(that)
		{
		}

		AlignedBox(const Vector<Real, N>& min,
			const Vector<Real, N>& max)
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

	typedef AlignedBox<integer, 1> AlignedBox1i;
	typedef AlignedBox<integer, 2> AlignedBox2i;
	typedef AlignedBox<integer, 3> AlignedBox3i;
	typedef AlignedBox<integer, 4> AlignedBox4i;
	typedef AlignedBox<integer, Dynamic> AlignedBoxDi;

}

#include "pastel/sys/alignedbox.hpp"

#endif
