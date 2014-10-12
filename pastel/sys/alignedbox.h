// Description: Axis-aligned box

#ifndef PASTELSYS_ALIGNEDBOX_H
#define PASTELSYS_ALIGNEDBOX_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/constants.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/topology.h"

#include <boost/operators.hpp>

namespace Pastel
{

	//! An axis-aligned box
	template <typename Real, int N = Dynamic>
	class AlignedBox
		: boost::multipliable<AlignedBox<Real, N>, Real
		, boost::dividable<AlignedBox<Real, N>, Real
		, boost::addable<AlignedBox<Real, N>, Vector<Real, N>
		, boost::subtractable<AlignedBox<Real, N>, Vector<Real, N>
		> > > >
	{
	private:
		PASTEL_STATIC_ASSERT(N == Dynamic || N > 0);

	public:
		using Real_ = Real;

		static PASTEL_CONSTEXPR int N_ = N;

		// Using default copy constructor.
		// Using default assignment.

		//! Constructs a unit box centered at the origin.
		explicit AlignedBox(integer dimension = N)
			: min_(Dimension(dimension), infinity<Real>())
			, max_(Dimension(dimension), -infinity<Real>())
			, minTopology_(ofDimension(dimension), Topology::Closed)
			, maxTopology_(ofDimension(dimension), Topology::Open)
		{
			PENSURE2((N == Dynamic && dimension > 0) || 
				(N != Dynamic && dimension == N), dimension, N);
		}

		//! Constructs a singular box (min = max = that).
		explicit AlignedBox(const Vector<Real, N>& that)
			: min_(that)
			, max_(that)
			, minTopology_(ofDimension(that.n()), Topology::Closed)
			, maxTopology_(ofDimension(that.n()), Topology::Open)
		{
		}

		//! Constructs a box using the given points.
		AlignedBox(
			const Vector<Real, N>& min,
			const Vector<Real, N>& max)
			: min_(min)
			, max_(max)
			, minTopology_(ofDimension(min.n()), Topology::Closed)
			, maxTopology_(ofDimension(max.n()), Topology::Open)
		{
		}

		template <
			int N_ = N,
			typename = EnableIfC<(N_ == 1), void>
			>
		AlignedBox(
			const Real& xMin, 
			const Real& xMax)
			: min_(xMin)
			, max_(xMax)
			, minTopology_(ofDimension(min_.n()), Topology::Closed)
			, maxTopology_(ofDimension(max_.n()), Topology::Open)
		{
		}

		template <
			int N_ = N,
			typename = EnableIfC<(N_ == 2), void>
			>
		AlignedBox(
			const Real& xMin, const Real& yMin,
			const Real& xMax, const Real& yMax)
			: min_(xMin, yMin)
			, max_(xMax, yMax)
			, minTopology_(ofDimension(min_.n()), Topology::Closed)
			, maxTopology_(ofDimension(max_.n()), Topology::Open)
		{
		}

		template <
			int N_ = N,
			typename = EnableIfC<(N_ == 3), void>
			>
		AlignedBox(
			const Real& xMin, const Real& yMin, const Real& zMin,
			const Real& xMax, const Real& yMax, const Real& zMax)
			: min_(xMin, yMin, zMin)
			, max_(xMax, yMax, zMax)
			, minTopology_(ofDimension(min_.n()), Topology::Closed)
			, maxTopology_(ofDimension(max_.n()), Topology::Open)
		{
		}

		template <
			int N_ = N,
			typename = EnableIfC<(N_ == 4), void>>
		AlignedBox(
			const Real& xMin, const Real& yMin, const Real& zMin, const Real& wMin,
			const Real& xMax, const Real& yMax, const Real& zMax, const Real& wMax)
			: min_(xMin, yMin, zMin, wMin)
			, max_(xMax, yMax, zMax, wMax)
			, minTopology_(ofDimension(min_.n()), Topology::Closed)
			, maxTopology_(ofDimension(max_.n()), Topology::Open)
		{
		}

		//! Returns whether the i:th projection contains any points.
		bool empty(integer i) const
		{
			PENSURE_OP(i, >=, 0);
			PENSURE_OP(i, <, n());

			if (min_[i] >= max_[i])
			{
				if (min_[i] > max_[i] ||
					minTopology_[i] == Topology::Open ||
					maxTopology_[i] == Topology::Open)
				{
					return true;
				}
			}

			return false;
		}

		//! Returns whether the box contains any points.
		bool empty() const
		{
			integer d = n();
			for (integer i = 0;i < d;++i)
			{
				if (empty(i))
				{
					return true;
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

		integer n() const
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
		Tuple<Topology, N>& minTopology()
		{
			return minTopology_;
		}

		//! Returns the topology of the minimums.
		const Tuple<Topology, N>& minTopology() const
		{
			return minTopology_;
		}

		//! Returns the topology of the maximums.
		Tuple<Topology, N>& maxTopology()
		{
			return maxTopology_;
		}

		//! Returns the topology of the maximums.
		const Tuple<Topology, N>& maxTopology() const
		{
			return maxTopology_;
		}

		//! Returns max() - min().
		VectorSubtraction<Real, N, 
			Vector<Real, N>, 
			Vector<Real, N> >
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

			return *this;
		}

		//! Translates the box backwards by the given vector.
		AlignedBox<Real, N>& operator-=(
			const Vector<Real, N>& right)
		{
			min_ -= right;
			max_ -= right;

			return *this;
		}

		//! Scales up the box without affecting position.
		AlignedBox<Real, N>& operator*=(
			const Real& that)
		{
			Vector<Real, N> translation =
				(max_ - min_) * ((that - 1) / 2);
		
			min_ -= translation;
			max_ += translation;

			return *this;
		}

		//! Scales down the box without affecting position.
		AlignedBox<Real, N>& operator/=(
			const Real& that)
		{
			return (*this *= inverse(that));
		}


		template <int N_ = N>
		EnableIfC<(N_ == 1), void> set(
			const Real& xMin,
			const Real& xMax)
		{
			set(Vector<Real, N>(xMin),
				Vector<Real, N>(xMax));
		}

		template <int N_ = N>
		EnableIfC<(N_ == 2), void> set(
			const Real& xMin, const Real& yMin,
			const Real& xMax, const Real& yMax)
		{
			set(Vector<Real, N>(xMin, yMin),
				Vector<Real, N>(xMax, yMax));
		}

		template <int N_ = N>
		EnableIfC<(N_ == 3), void> set(
			const Real& xMin, const Real& yMin, const Real& zMin,
			const Real& xMax, const Real& yMax, const Real& zMax)
		{
			set(Vector<Real, N>(xMin, yMin, zMin),
				Vector<Real, N>(xMax, yMax, zMax));
		}

		template <int N_ = N>
		EnableIfC<(N_ == 4), void> set(
			const Real& xMin, const Real& yMin, const Real& zMin, const Real& wMin,
			const Real& xMax, const Real& yMax, const Real& zMax, const Real& wMax)
		{
			set(Vector<Real, N>(xMin, yMin, zMin, wMin),
				Vector<Real, N>(xMax, yMax, zMax, wMax));
		}

		template <int N_ = N>
		EnableIfC<(N_ >= 1), Real> width() const
		{
			return extent()[0];
		}

		template <int N_ = N>
		EnableIfC<(N_ >= 2), Real> height() const
		{
			return extent()[1];
		}

		template <int N_ = N>
		EnableIfC<(N_ >= 3), Real> depth() const
		{
			return extent()[2];
		}

	private:
		Vector<Real, N> min_;
		Vector<Real, N> max_;
		Tuple<Topology, N> minTopology_;
		Tuple<Topology, N> maxTopology_;
	};

	template <typename Real, int N>
	void swap(AlignedBox<Real, N>& left,
		AlignedBox<Real, N>& right);

	using AlignedBox1 = AlignedBox<real, 1>;
	using AlignedBox2 = AlignedBox<real, 2>;
	using AlignedBox3 = AlignedBox<real, 3>;
	using AlignedBox4 = AlignedBox<real, 4>;
	using AlignedBoxD = AlignedBox<real, Dynamic>;

	using AlignedBox1i = AlignedBox<integer, 1>;
	using AlignedBox2i = AlignedBox<integer, 2>;
	using AlignedBox3i = AlignedBox<integer, 3>;
	using AlignedBox4i = AlignedBox<integer, 4>;
	using AlignedBoxDi = AlignedBox<integer, Dynamic>;

}

#include "pastel/sys/alignedbox.hpp"
#include "pastel/sys/alignedbox_tools.h"

#endif
