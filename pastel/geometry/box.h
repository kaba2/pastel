// Description: Box class
// Detail: Allows to work with oriented boxes
// Documentation: shapes.txt

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

	template <typename Real, int N = Dynamic>
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
		Box(const Point<Real, N>& position,
			const Vector<Real, N>& width,
			const Matrix<N, N, Real>& rotation);

		//! Constructs using the given information.
		Box(integer dimension,
			const Point<Real, N>& position,
			const Vector<Real, N>& width,
			const Matrix<N, N, Real>& rotation);

		// Used for concept checking.
		~Box();

		//! Returns the dimensions of the box.
		integer dimension() const;

		//! Sets the dimensions of the box.
		void setWidth(const Vector<Real, N>& width);

		//! Returns the dimensions of the box.
		const Vector<Real, N>& width() const;

		//! Sets the position of the box.
		void setPosition(const Point<Real, N>& position);

		//! Returns the position of the box.
		const Point<Real, N>& position() const;

		//! Sets the rotation of the box.
		void setRotation(const Matrix<N, N, Real>& rotation);

		//! Returns the rotation of the box.
		const Matrix<N, N, Real>& rotation() const;

	private:
		Point<Real, N> position_;
		Vector<Real, N> width_;
		Matrix<N, N, Real> rotation_;
	};

	typedef Box<real, 1> Box1;
	typedef Box<real, 2> Box2;
	typedef Box<real, 3> Box3;
	typedef Box<real, 4> Box4;

}

#include "pastel/geometry/box.hpp"

#endif
