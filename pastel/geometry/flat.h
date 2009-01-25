/*!
\file
\brief A class for an M-flat.
*/

#ifndef PASTELGEOMETRY_FLAT_H
#define PASTELGEOMETRY_FLAT_H

#include "pastel/sys/tuple.h"

#include "pastel/sys/point.h"
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

	template <int N, typename Real, int M>
	class Flat
	{
	public:
		// Using default constructor.
		// Using default copy constructor.
		// Using default assignment.

		// Used for concept checking.
		~Flat();

		//! Sets the position of the flat.
		void setPosition(const Point<N, Real>& position);

		//! Returns the position of the flat.
		const Point<N, Real>& position() const;

		//! Returns a specific tangent vector of the flat.
		Vector<N, Real>& operator[](integer index);

		//! Returns a specific tangent vector of the flat.
		const Vector<N, Real>& operator[](integer index) const;

	private:
		Point<N, Real> position_;
		Tuple<M, Vector<N, Real> > tangent_;
	};

}

#include "pastel/geometry/flat.hpp"

#endif
