/*!
\file
\brief A class for an oriented bounding box.
*/

#ifndef PASTEL_BOX_H
#define PASTEL_BOX_H

#include "pastel/sys/point.h"
#include "pastel/sys/vector.h"

#include "pastel/math/matrix.h"

namespace Pastel
{

	//! An oriented box.

	/*!
	An oriented box B is described by a position p,
	an orthonormal n-tuple of vectors n_i and an n-tuple of
	half-widths h_i:
	B = {p + sum(i)(x_i * n_i) | -h_i <= x_i <= h_i for all i}
	The vectors n_i are packed as row vectors
	into a matrix which is called the
	'rotation' of the box.

	Invariants:
	Must be preserved by the user!

	'rotation()' is a rotation matrix
	'width()' >= 0
	*/

	template <int N, typename Real>
	class Box
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		//! Constructs an axis-aligned, origin centered 1-halfwidth box.
		Box();

		//! Constructs an axis-aligned, origin centered 1-halfwidth box.
		explicit Box(integer dimension);

		//! Constructs using the given information.
		Box(const Point<N, Real>& position,
			const Vector<N, Real>& width,
			const Matrix<N, N, Real>& rotation);

		//! Constructs using the given information.
		Box(integer dimension,
			const Point<N, Real>& position,
			const Vector<N, Real>& width,
			const Matrix<N, N, Real>& rotation);

		// Used for concept checking.
		~Box();

		//! Returns the dimensions of the box.
		integer dimension() const;

		//! Sets the dimensions of the box.
		void setWidth(const Vector<N, Real>& width);

		//! Returns the dimensions of the box.
		const Vector<N, Real>& width() const;

		//! Sets the position of the box.
		void setPosition(const Point<N, Real>& position);

		//! Returns the position of the box.
		const Point<N, Real>& position() const;

		//! Sets the rotation of the box.
		void setRotation(const Matrix<N, N, Real>& rotation);

		//! Returns the rotation of the box.
		const Matrix<N, N, Real>& rotation() const;

	private:
		Point<N, Real> position_;
		Vector<N, Real> width_;
		Matrix<N, N, Real> rotation_;
	};

	typedef Box<1, real> Box1;
	typedef Box<2, real> Box2;
	typedef Box<3, real> Box3;
	typedef Box<4, real> Box4;

}

#include "pastel/geometry/box.hpp"

#endif
