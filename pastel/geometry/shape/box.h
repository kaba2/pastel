// Description: Oriented box

#ifndef PASTELGEOMETRY_BOX_H
#define PASTELGEOMETRY_BOX_H

#include "pastel/sys/vector.h"

#include "pastel/math/matrix/matrix.h"

#include <boost/operators.hpp>

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
		: boost::multipliable<Box<Real, N>, Real
		, boost::dividable<Box<Real, N>, Real
		, boost::addable<Box<Real, N>, Vector<Real, N>
		, boost::subtractable<Box<Real, N>, Vector<Real, N>
		> > > >
	{
	public:
		using Real_ = Real;
		static PASTEL_CONSTEXPR int N_ = N;

		// Using default copy constructor.
		// Using default assignment.

		//! Constructs an axis-aligned, origin centered 1-halfwidth box.
		Box();

		//! Constructs an axis-aligned, origin centered 1-halfwidth box.
		explicit Box(integer dimension);

		//! Constructs using the given information.
		Box(const Vector<Real, N>& position,
			const Vector<Real, N>& width,
			const Matrix<Real>& rotation);

		//! Constructs using the given information.
		Box(integer dimension,
			const Vector<Real, N>& position,
			const Vector<Real, N>& width,
			const Matrix<Real>& rotation);

		// Used for concept checking.
		~Box();

		//! Returns the dimensions of the box.
		integer n() const;

		//! Sets the dimensions of the box.
		void setWidth(const Vector<Real, N>& width);

		//! Returns the dimensions of the box.
		const Vector<Real, N>& width() const;

		//! Sets the position of the box.
		void setPosition(const Vector<Real, N>& position);

		//! Returns the position of the box.
		const Vector<Real, N>& position() const;

		//! Sets the rotation of the box.
		void setRotation(const Matrix<Real>& rotation);

		//! Returns the rotation of the box.
		const Matrix<Real>& rotation() const;

		//! Translates the box by the given vector.
		Box<Real, N>& operator+=(const Vector<Real, N>& that);

		//! Translates the box backwards by the given vector.
		Box<Real, N>& operator-=(const Vector<Real, N>& that);

		//! Scales up the box without affecting position.
		Box<Real, N>& operator*=(const Real& that);

		//! Scales down the box without affecting position.
		Box<Real, N>& operator/=(const Real& that);

	private:
		Vector<Real, N> position_;
		Vector<Real, N> width_;
		Matrix<Real> rotation_;
	};

	using Box1 = Box<real, 1>;
	using Box2 = Box<real, 2>;
	using Box3 = Box<real, 3>;
	using Box4 = Box<real, 4>;
	using BoxD = Box<real, Dynamic>;

}

#include "pastel/geometry/shape/box.hpp"

#endif
