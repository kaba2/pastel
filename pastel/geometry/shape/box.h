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
		static constexpr int N_ = N;

		// Using default copy constructor.
		// Using default assignment.

		//! Constructs an axis-aligned, origin centered 1-halfwidth box.
		Box();

		//! Constructs an axis-aligned, origin centered 1-halfwidth box.
		explicit Box(integer dimension);

		//! Constructs using the given information.
		Box(const Vector<Real, N>& position,
			const Vector<Real, N>& width,
			const Matrix<Real, N, N>& rotation);

		//! Constructs using the given information.
		Box(integer dimension,
			const Vector<Real, N>& position,
			const Vector<Real, N>& width,
			const Matrix<Real, N, N>& rotation);

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
		void setRotation(const Matrix<Real, N, N>& rotation);

		//! Returns the rotation of the box.
		const Matrix<Real, N, N>& rotation() const;

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
		Matrix<Real, N, N> rotation_;
	};

	using Box1 = Box<dreal, 1>;
	using Box2 = Box<dreal, 2>;
	using Box3 = Box<dreal, 3>;
	using Box4 = Box<dreal, 4>;
	using BoxD = Box<dreal, Dynamic>;

}

namespace Pastel
{

	template <typename Real, int N>
	Box<Real, N>::Box()
		: position_(0)
		, width_(1)
		, rotation_(N, N)
	{
		PASTEL_STATIC_ASSERT(N != Dynamic);
	}

	template <typename Real, int N>
	Box<Real, N>::Box(integer dimension)
		: position_(ofDimension(dimension), 0)
		, width_(ofDimension(dimension), 1)
		, rotation_(dimension, dimension)
	{
	}

	template <typename Real, int N>
	Box<Real, N>::Box(
		const Vector<Real, N>& position,
		const Vector<Real, N>& width,
		const Matrix<Real, N, N>& rotation)
		: position_(position)
		, width_(width)
		, rotation_(rotation)
	{
		PASTEL_STATIC_ASSERT(N != Dynamic);
	}

	template <typename Real, int N>
	Box<Real, N>::Box(
		integer dimension,
		const Vector<Real, N>& position,
		const Vector<Real, N>& width,
		const Matrix<Real, N, N>& rotation)
		: position_(position)
		, width_(width)
		, rotation_(rotation)
	{
		PENSURE_OP(dimension, ==, position.size());
		PENSURE_OP(dimension, ==, width.size());
		PENSURE_OP(dimension, ==, rotation.cols());
		PENSURE_OP(dimension, ==, rotation.rows());
	}

	template <typename Real, int N>
	Box<Real, N>::~Box()
	{
		PASTEL_STATIC_ASSERT(N == Dynamic || N > 0);
	}

	template <typename Real, int N>
	integer Box<Real, N>::n() const
	{
		return position_.n();
	}

	template <typename Real, int N>
	void Box<Real, N>::setPosition(
		const Vector<Real, N>& position)
	{
		position_ = position;
	}

	template <typename Real, int N>
	const Vector<Real, N>& Box<Real, N>::position() const
	{
		return position_;
	}

	template <typename Real, int N>
	void Box<Real, N>::setWidth(
		const Vector<Real, N>& width)
	{
		width_ = width;
	}

	template <typename Real, int N>
	const Vector<Real, N>& Box<Real, N>::width() const
	{
		return width_;
	}

	template <typename Real, int N>
	void Box<Real, N>::setRotation(
		const Matrix<Real, N, N>& rotation)
	{
		rotation_ = rotation;
	}

	template <typename Real, int N>
	const Matrix<Real, N, N>& Box<Real, N>::rotation() const
	{
		return rotation_;
	}

	template <typename Real, int N>
	Box<Real, N>& Box<Real, N>::operator+=(
		const Vector<Real, N>& that)
	{
		position_ += that;

		return *this;
	}

	template <typename Real, int N>
	Box<Real, N>& Box<Real, N>::operator-=(
		const Vector<Real, N>& that)
	{
		position_ -= that;

		return *this;
	}

	template <typename Real, int N>
	Box<Real, N>& Box<Real, N>::operator*=(
		const Real& that)
	{
		width_ *= that;

		return *this;
	}

	template <typename Real, int N>
	Box<Real, N>& Box<Real, N>::operator/=(
		const Real& that)
	{
		PENSURE_OP(that, !=, 0);

		width_ /= that;

		return *this;
	}

}

#endif
