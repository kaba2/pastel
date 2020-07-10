// Description: Flat class

#ifndef PASTELGEOMETRY_FLAT_H
#define PASTELGEOMETRY_FLAT_H

#include "pastel/sys/tuple.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	//! An M-flat.

	/*!
	An M-flat is an object that is the span
	of M linearly independent vectors added
	to a point. It is described by M
	tangent vectors plus a point.
	For specific examples, 1-flat is a line,
	(N - 1)-flat is a (hyper)plane.
	Clearly it must be that M <= N.
	*/
	template <typename Real, int N, int M>
	class Flat
	{
	public:
		using Real_ = Real;
		static constexpr int N_ = N;
		static constexpr int M_ = M;

		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.

		// Used for concept checking.
		~Flat()
		{
			PASTEL_STATIC_ASSERT(N == Dynamic || N > 0);
			PASTEL_STATIC_ASSERT(M <= N);
		}

		//! Sets the position of the flat.
		void setPosition(const Vector<Real, N>& position)
		{
			position_ = position;
		}

		//! Returns the position of the flat.
		const Vector<Real, N>& position() const
		{
			return position_;
		}

		//! Returns a specific tangent vector of the flat.
		Vector<Real, N>& operator[](integer index)
		{
			PENSURE2(index >= 0 && index < M, index, M);

			return tangent_[index];
		}

		//! Returns a specific tangent vector of the flat.
		const Vector<Real, N>& operator[](integer index) const
		{
			PENSURE2(index >= 0 && index < M, index, M);

			return tangent_[index];
		}

	private:
		Vector<Real, N> position_;
		Tuple<Vector<Real, N>, M> tangent_;
	};

}

#endif
