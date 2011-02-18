// Description: AlignedBoxBase class
// Detail: CRTP base class for AlignedBox

#ifndef PASTEL_ALIGNEDBOXBASE_H
#define PASTEL_ALIGNEDBOXBASE_H

#include "pastel/sys/alignedbox.h"
#include "pastel/sys/constants.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/topology.h"

#include <boost/operators.hpp>
#include <boost/type_traits/is_integral.hpp>

namespace Pastel
{

	template <typename Real, int N>
	class AlignedBox;

	//! An axis-aligned box

	/*!
	An axis-aligned box can be described by giving
	two corner points. The other is called the minimum point
	'min' and the other is called the maximum point 'max'.
	The aligned box is the set of points p that satisfy
	'min <= p <= max' where the inequalities are understood
	as "for all components".
	*/

	template <typename Real, int N>
	class AlignedBoxBase
		: boost::multipliable<AlignedBox<Real, N>, Real
		, boost::dividable<AlignedBox<Real, N>, Real
		, boost::addable<AlignedBox<Real, N>, Vector<Real, N>
		, boost::subtractable<AlignedBox<Real, N>, Vector<Real, N>
		> > > >
	{
	public:
		typedef Real Real_;
		enum
		{
			N_ = N
		};

		static const bool IsInteger =
			boost::is_integral<Real>::value;

		// Using default copy constructor.
		// Using default assignment.

		//! Constructs a unit box centered at the origin.
		explicit AlignedBoxBase(integer dimension = N)
			: min_(Dimension(dimension), infinity<Real>())
			, max_(Dimension(dimension), -infinity<Real>())
			, minTopology_(ofDimension(dimension), Topology::Closed)
			, maxTopology_(ofDimension(dimension), IsInteger ? Topology::Open : Topology::Closed )
		{
			PENSURE2((N == Dynamic && dimension > 0) || 
				(N != Dynamic && dimension == N), dimension, N);
		}

		//! Constructs a singular box (min = max = that).
		explicit AlignedBoxBase(const Vector<Real, N>& that)
			: min_(that)
			, max_(that)
			, minTopology_(ofDimension(that.dimension()), Topology::Closed)
			, maxTopology_(ofDimension(that.dimension()), IsInteger ? Topology::Open : Topology::Closed)
		{
		}

		//! Constructs a box using the given points.
		AlignedBoxBase(
			const Vector<Real, N>& min,
			const Vector<Real, N>& max)
			: min_(min)
			, max_(max)
			, minTopology_(ofDimension(min.dimension()), Topology::Closed)
			, maxTopology_(ofDimension(max.dimension()), IsInteger ? Topology::Open : Topology::Closed )
		{
		}

		// Used for concept checking.
		~AlignedBoxBase()
		{
			PASTEL_STATIC_ASSERT(N == Dynamic || N > 0);
		}

		//! Returns whether the box contains any points.
		bool empty() const
		{
			const integer n = dimension();
			for (integer i = 0;i < n;++i)
			{
				if (min_[i] >= max_[i])
				{
					if (min_[i] > max_[i] ||
						minTopology_[i] == Topology::Open ||
						maxTopology_[i] == Topology::Open)
					{
						return true;
					}
				}
			}

			return false;
		}

		//! Swaps two aligned boxes.
		void swap(AlignedBox<Real, N>& that)
		{
			using std::swap;

			min_.swap(that.min_);
			max_.swap(that.max_);
			minTopology_.swap(that.minTopology_);
			maxTopology_.swap(that.maxTopology_);
		}

		integer dimension() const
		{
			return min_.size();
		}

		//! Sets the corner points of the box.
		void set(
			const Vector<Real, N>& min,
			const Vector<Real, N>& max)
		{
			min_ = min;
			max_ = max;
		}

		//! Sets the minimum point of the box.
		void setMin(const Vector<Real, N>& min)
		{
			min_ = min;
		}

		//! Returns the minimum point of the box.
		Vector<Real, N>& min()
		{
			return min_;
		}

		//! Returns the minimum point of the box.
		const Vector<Real, N>& min() const
		{
			return min_;
		}

		//! Sets the maximum point of the box.
		void setMax(const Vector<Real, N>& max)
		{
			max_ = max;
		}

		//! Returns the maximum point of the box.
		Vector<Real, N>& max()
		{
			return max_;
		}

		//! Returns the maximum point of the box.
		const Vector<Real, N>& max() const
		{
			return max_;
		}

		//! Returns the topology of the minimums.
		Tuple<Topology::Enum, N>& minTopology()
		{
			return minTopology_;
		}

		//! Returns the topology of the minimums.
		const Tuple<Topology::Enum, N>& minTopology() const
		{
			return minTopology_;
		}

		//! Returns the topology of the maximums.
		Tuple<Topology::Enum, N>& maxTopology()
		{
			return maxTopology_;
		}

		//! Returns the topology of the maximums.
		const Tuple<Topology::Enum, N>& maxTopology() const
		{
			return maxTopology_;
		}

		//! Returns max() - min().
		const VectorSubtraction<Real, N, 
			Detail::VectorBase<Real, N>, 
			Detail::VectorBase<Real, N> >
			extent() const
		{
			return max_ - min_;
		}

		Real extent(integer index) const
		{
			return max_[index] - min_[index];
		}

		Vector<Real, N> at(
			const Vector<Real, N>& coordinates) const
		{
			return Vector<Real, N>(
				(1 - coordinates) * min_ +
				coordinates * max_);
		}

		//! Translates the box by the given vector.
		AlignedBox<Real, N>& operator+=(
			const Vector<Real, N>& right)
		{
			min_ += right;
			max_ += right;

			return (AlignedBox<Real, N>&)*this;
		}

		//! Translates the box backwards by the given vector.
		AlignedBox<Real, N>& operator-=(
			const Vector<Real, N>& right)
		{
			min_ -= right;
			max_ -= right;

			return (AlignedBox<Real, N>&)*this;
		}

		//! Scales up the box without affecting position.
		AlignedBox<Real, N>& operator*=(
			const Real& that)
		{
			const Vector<Real, N> translation =
				(max_ - min_) * ((that - 1) / 2);
		
			min_ -= translation;
			max_ += translation;

			return (AlignedBox<Real, N>&)*this;
		}

		//! Scales down the box without affecting position.
		AlignedBox<Real, N>& operator/=(
			const Real& that)
		{
			return (AlignedBox<Real, N>&)(*this *= inverse(that));
		}

	private:
		Vector<Real, N> min_;
		Vector<Real, N> max_;
		Tuple<Topology::Enum, N> minTopology_;
		Tuple<Topology::Enum, N> maxTopology_;
	};

}

#include "pastel/sys/alignedboxbase.hpp"

#endif
