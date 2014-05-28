// Description: Flat class

#ifndef PASTELGEOMETRY_FLAT_H
#define PASTELGEOMETRY_FLAT_H

#include "pastel/sys/tuple.h"
#include "pastel/sys/vector.h"

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
		typedef Real Real_;
		static PASTEL_CONSTEXPR int N_ = N;
		static PASTEL_CONSTEXPR int M_ = M;

		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.

		// Used for concept checking.
		~Flat();

		//! Sets the position of the flat.
		void setPosition(const Vector<Real, N>& position);

		//! Returns the position of the flat.
		const Vector<Real, N>& position() const;

		//! Returns a specific tangent vector of the flat.
		Vector<Real, N>& operator[](integer index);

		//! Returns a specific tangent vector of the flat.
		const Vector<Real, N>& operator[](integer index) const;

	private:
		Vector<Real, N> position_;
		Tuple<Vector<Real, N>, M> tangent_;
	};

}

#include "pastel/geometry/flat.hpp"

#endif
