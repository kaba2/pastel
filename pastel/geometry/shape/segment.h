// Description: Line segment

#ifndef PASTELGEOMETRY_SEGMENT_H
#define PASTELGEOMETRY_SEGMENT_H

#include "pastel/sys/vector.h"

#include "pastel/geometry/topology.h"

#include <boost/operators.hpp>

namespace Pastel
{

	//! A line segment.
	/*!
	A line segment is described by two endpoints:
	a 'starting point' P and an 'ending point' Q.
	The line segment is then the set of points
	p = P + t * (Q - P)
	with 0 <= t <= 1
	*/
	template <typename Real, int N = Dynamic>
	class Segment
		: boost::multipliable<Segment<Real, N>, Real
		, boost::dividable<Segment<Real, N>, Real
		, boost::addable<Segment<Real, N>, Vector<Real, N>
		, boost::subtractable<Segment<Real, N>, Vector<Real, N>
		> > > >
	{
	public:
		using Real_ = Real;
		static constexpr int N_ = N;

		// Using default copy constructor.
		// Using default assignment.

		//! Constructs a singular segment at origin.
		Segment();

		//! Constructs a segment using the given points.
		Segment(
			const Vector<Real, N>& start,
			const Vector<Real, N>& end);

		//! Constructs a segment using the given points.
		Segment(
			const Vector<Real, N>& start,
			const Vector<Real, N>& end,
			Topology startTopology,
			Topology endTopology);

		// Used for concept checking.
		~Segment();

		//! Swaps two segments.
		void swap(Segment& that);

		//! Sets the end points of the segment.
		void set(const Vector<Real, N>& start,
			const Vector<Real, N>& end);

		//! Returns the start point of the segment.
		Vector<Real, N>& start();

		//! Returns the start point of the segment.
		const Vector<Real, N>& start() const;

		//! Returns the end point of the segment.
		Vector<Real, N>& end();

		//! Returns the end point of the segment.
		const Vector<Real, N>& end() const;

		//! Sets the topology of the start point.
		void setStartTopology(Topology startTopology);

		//! Returns the topology of the start point.
		Topology startTopology() const;

		//! Sets the topology of the end point.
		void setEndTopology(Topology endTopology);

		//! Returns the topology of the end point.
		Topology endTopology() const;

		Vector<Real, N> at(const Real& t) const;

		//! Translates the segment by the given vector.
		Segment<Real, N>& operator+=(const Vector<Real, N>& that);

		//! Translates the segment backwards by the given vector.
		Segment<Real, N>& operator-=(const Vector<Real, N>& that);

		//! Scales up the segment without affecting position.
		Segment<Real, N>& operator*=(const Real& that);

		//! Scales down the segment without affecting position.
		Segment<Real, N>& operator/=(const Real& that);

	private:
		Vector<Real, N> start_;
		Vector<Real, N> end_;
		Topology startTopology_;
		Topology endTopology_;
	};

	using Segment1 = Segment<dreal, 1>;
	using Segment2 = Segment<dreal, 2>;
	using Segment3 = Segment<dreal, 3>;
	using Segment4 = Segment<dreal, 4>;
	using SegmentD = Segment<dreal, Dynamic>;

	template <typename Real, int N>
	void swap(
		Segment<Real, N>& left,
		Segment<Real, N>& right);

}

#include "pastel/sys/vector.h"
#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <typename Real, int N>
	Segment<Real, N>::Segment()
		: start_()
		, end_()
		, startTopology_(Topology::Closed)
		, endTopology_(Topology::Closed)
	{
	}

	template <typename Real, int N>
	Segment<Real, N>::Segment(
		const Vector<Real, N>& start,
		const Vector<Real, N>& end)
		: start_(start)
		, end_(end)
		, startTopology_(Topology::Closed)
		, endTopology_(Topology::Closed)
	{
	}

	template <typename Real, int N>
	Segment<Real, N>::Segment(
		const Vector<Real, N>& start,
		const Vector<Real, N>& end,
		Topology startTopology,
		Topology endTopology)
		: start_(start)
		, end_(end)
		, startTopology_(startTopology)
		, endTopology_(endTopology)
	{
	}

	template <typename Real, int N>
	Segment<Real, N>::~Segment()
	{
		PASTEL_STATIC_ASSERT(N == Dynamic || N > 0);
	}

	template <typename Real, int N>
	void Segment<Real, N>::swap(
		Segment& that)
	{
		using std::swap;

		start_.swap(that.start_);
		end_.swap(that.end_);
		swap(startTopology_, that.startTopology_);
		swap(endTopology_, that.endTopology_);
	}
	
	template <typename Real, int N>
	void Segment<Real, N>::set(
		const Vector<Real, N>& start,
		const Vector<Real, N>& end)
	{
		start_ = start;
		end_ = end;
	}

	template <typename Real, int N>
	Vector<Real, N>& Segment<Real, N>::start()
	{
		return start_;
	}

	template <typename Real, int N>
	const Vector<Real, N>& Segment<Real, N>::start() const
	{
		return start_;
	}

	template <typename Real, int N>
	Vector<Real, N>& Segment<Real, N>::end()
	{
		return end_;
	}

	template <typename Real, int N>
	const Vector<Real, N>& Segment<Real, N>::end() const
	{
		return end_;
	}

	template <typename Real, int N>
	void Segment<Real, N>::setStartTopology(
		Topology startTopology)
	{
		startTopology_ = startTopology;
	}

	template <typename Real, int N>
	Topology Segment<Real, N>::startTopology() const
	{
		return startTopology_;
	}

	template <typename Real, int N>
	void Segment<Real, N>::setEndTopology(
		Topology endTopology)
	{
		endTopology_ = endTopology;
	}

	template <typename Real, int N>
	Topology Segment<Real, N>::endTopology() const
	{
		return endTopology_;
	}

	template <typename Real, int N>
	Vector<Real, N> Segment<Real, N>::at(const Real& t) const
	{
		return linear(start_, end_, t);
	}

	template <typename Real, int N>
	Segment<Real, N>& Segment<Real, N>::operator+=(
		const Vector<Real, N>& that)
	{
		start_ += that;
		end_ += that;

		return *this;
	}

	template <typename Real, int N>
	Segment<Real, N>& Segment<Real, N>::operator-=(
		const Vector<Real, N>& that)
	{
		start_ -= that;
		end_ -= that;

		return *this;
	}

	template <typename Real, int N>
	Segment<Real, N>& Segment<Real, N>::operator*=(
		const Real& that)
	{
		Vector<Real, N> translation =
			(end_ - start_) * ((that - 1) / 2);
		
		start_ -= translation;
		end_ += translation;

		return *this;
	}

	template <typename Real, int N>
	Segment<Real, N>& Segment<Real, N>::operator/=(
		const Real& that)
	{
		PENSURE_OP(that, !=, 0);

		return (*this) *= inverse(that);
	}

	template <typename Real, int N>
	void swap(
		Segment<Real, N>& left,
		Segment<Real, N>& right)
	{
		left.swap(right);
	}

}

#endif
