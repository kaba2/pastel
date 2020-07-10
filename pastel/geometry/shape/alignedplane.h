// Description: Axis-aligned plane

#ifndef PASTELGEOMETRY_ALIGNEDPLANE_H
#define PASTELGEOMETRY_ALIGNEDPLANE_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! An axis-aligned plane.
	/*!
	An axis-aligned plane can be described by an
	axis and a position on that axis. The plane
	is orthogonal to the given axis.
	The axis is described by an index
	into the sequence of natural basis vectors
	e = ((1, 0, 0, ...), (0, 1, 0, ...), ...).
	*/

	template <typename Real, int N = Dynamic>
	class AlignedPlane
	{
	public:
		using Real_ = Real;
		static constexpr int N_ = N;

		// Using default copy constructor.
		// Using default assignment.

		//! Constructs an x-orthogonal plane at origin.
		AlignedPlane()
			: position_(0)
			, axis_(0)
		{
		}

		//! Constructs an x-orthogonal plane at origin.
		explicit AlignedPlane(integer dimension)
			: position_(0)
			, axis_(0)
		{
			PENSURE_OP(dimension, ==, N);
		}

		//! Constructs an 'axis'-orthogonal plane at 'position'.
		AlignedPlane(integer dimension, 
			const Real& position, integer axis)
			: position_(position)
			, axis_(axis)
		{
		}

		~AlignedPlane()
		{
			PASTEL_STATIC_ASSERT(N > 0);
		}

		//! Swaps two AlignedPlane's.
		void swap(AlignedPlane& that)
		{
			using std::swap;

			swap(position_, that.position_);
			swap(axis_, that.axis_);
		}

		//! Returns the dimension of the plane.
		integer n() const
		{
			return N;
		}

		//! Sets the position of the plane on the axis.
		void setPosition(const Real& position)
		{
			position_ = position;
		}

		//! Returns the position of the plane on the axis.
		const Real& position() const
		{
			return position_;
		}

		//! Sets the index of the orthogonal axis of the plane.
		void setAxis(integer axis)
		{
			PENSURE2(axis_ >= 0 && axis < N, axis, N);
			axis_ = axis;
		}

		//! Returns the index of the orthogonal axis of the plane.
		integer axis() const
		{
			return axis_;
		}

	private:
		Real position_;
		integer axis_;
	};

	template <typename Real>
	class AlignedPlane<Real, Dynamic>
	{
	private:
		static constexpr int N = Dynamic;

	public:
		// Using default copy constructor.
		// Using default destructor.

		//! Constructs an x-orthogonal plane at origin.
		explicit AlignedPlane(integer dimension)
			: dimension_(dimension)
			, position_(0)
			, axis_(0)
		{
			PENSURE_OP(dimension, >, 0);
		}

		//! Constructs an 'axis'-orthogonal plane at 'position'.
		AlignedPlane(integer dimension, 
			const Real& position, integer axis)
			: dimension_(dimension)
			, position_(position)
			, axis_(axis)
		{
		}

		AlignedPlane<Real, N>& operator=(
			const AlignedPlane& that)
		{
			// This is why we define the assignment
			// operator: to check for equal dimensions.

			PENSURE_OP(dimension_, ==, that.dimension_);

			// We settle for basic exception safety
			// for performance.

			dimension_ = that.dimension_;
			position_ = that.position_;
			axis_ = that.axis_;

			return *this;
		}

		//! Swaps two AlignedPlane's.
		void swap(AlignedPlane& that)
		{
			using std::swap;

			swap(dimension_, that.dimension_);
			swap(position_, that.position_);
			swap(axis_, that.axis_);
		}

		//! Returns the dimension of the plane.
		integer n() const
		{
			return dimension_;
		}

		//! Sets the position of the plane on the axis.
		void setPosition(const Real& position)
		{
			position_ = position;
		}

		//! Returns the position of the plane on the axis.
		const Real& position() const
		{
			return position_;
		}

		//! Sets the index of the orthogonal axis of the plane.
		void setAxis(integer axis)
		{
			PENSURE2(axis_ >= 0 && axis < dimension_, axis, dimension_);
			axis_ = axis;
		}

		//! Returns the index of the orthogonal axis of the plane.
		integer axis() const
		{
			return axis_;
		}

	private:
		AlignedPlane() = delete;

		integer dimension_;
		Real position_;
		integer axis_;
	};

	using AlignedPlane1 = AlignedPlane<dreal, 1>;
	using AlignedPlane2 = AlignedPlane<dreal, 2>;
	using AlignedPlane3 = AlignedPlane<dreal, 3>;
	using AlignedPlane4 = AlignedPlane<dreal, 4>;
	using AlignedPlaneD = AlignedPlane<dreal, Dynamic>;

}

#endif
