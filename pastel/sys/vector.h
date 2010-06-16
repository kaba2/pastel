// Description: Vector class
// Detail: Allows to compute with vectors in R^n

#ifndef PASTEL_VECTOR_H
#define PASTEL_VECTOR_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vectorbase.h"

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_integral.hpp>

namespace Pastel
{

	//! A vector in R^n.

	/*!
	This class represents a vector in R^n. Care has
	been taken to make computation as efficient as possible.
	Expression templates are used to evaluate vector
	expressions breadth-first rather than depth-first.
	This allows the compiler to optimize vector
	expressions to perform equal with hand-written
	expressions.
	A simple template trick with TableModify
	is used to ensure unrolling of static-sized loops.
	*/

	/*
	"Curiously recurring template pattern" (CRTP) is used
	to place common functionality into VectorBase, so
	we can easily specialize the lower dimensions to support
	list initialization in constructor and set().
	The "friend trick" is used to implement correctly
	working left hand operators.
	*/

	template <typename Real, int N>
	class Vector
		: public Detail::VectorBase<Real, N>
	{
	private:
		typedef Detail::VectorBase<Real, N> Base;

	public:
		using Base::operator=;

		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		Vector()
			: Base()
		{
		}

		explicit Vector(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
		{
		}

		Vector(
			const Dimension& dimension,
			const Copy<const Real*>& that)
			: Base(dimension, that)
		{
		}

		// Alias for static vector is copying.
		// This is needed so that one does not have to
		// make special cases in generic programming.
		Vector(
			const Dimension& dimension,
			const Alias<Real*>& that)
			: Base(dimension, Copy<const Real*>(that))
		{
		}

		template <typename ThatReal, int ThatN>
		explicit Vector(const Tuple<ThatReal, ThatN>& that)
			: Base(that)
		{
		}

		Vector(const Vector<Real, N>& that)
			: Base(that)
		{
		}

		template <typename ThatReal, int ThatN>
		Vector(const Vector<ThatReal, ThatN>& that)
			: Base(that)
		{
		}

		explicit Vector(const Real& that)
			: Base(that)
		{
		}

		// Alias for static vector is copying.
		// This is needed so that one does not have to
		// make special cases in generic programming.
		explicit Vector(const Alias<Real*>& that)
			: Base(Copy<const Real*>(that))
		{
		}

		explicit Vector(const Copy<const Real*>& that)
			: Base(that)
		{
		}

		template <typename ThatReal, int ThatN, typename Expression>
		Vector(
			const VectorExpression
			<ThatReal, ThatN, Expression>& that)
			: Base(that)
		{
		}
	};

	// Specialization for dimension 1

	template <typename Real>
	class Vector<Real, 1>
		: public Detail::VectorBase<Real, 1>
	{
	private:
		enum
		{
			N = 1
		};

		typedef Detail::VectorBase<Real, N> Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		using Base::operator=;

		Vector()
			: Base()
		{
		}

		explicit Vector(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
		{
		}

		// Alias for static vector is copying.
		// This is needed so that one does not have to
		// make special cases in generic programming.
		Vector(
			const Dimension& dimension,
			const Alias<Real*>& that)
			: Base(dimension, Copy<const Real*>(that))
		{
		}

		Vector(
			const Dimension& dimension,
			const Copy<const Real*>& that)
			: Base(dimension, that)
		{
		}

		template <typename ThatReal, int ThatN>
		explicit Vector(const Tuple<ThatReal, ThatN>& that)
			: Base(that)
		{
		}

		Vector(const Vector<Real, N>& that)
			: Base(that)
		{
		}

		template <typename ThatReal, int ThatN>
		Vector(const Vector<ThatReal, ThatN>& that)
			: Base(that)
		{
		}

		// Implicit conversion allowed
		// only for 1d version.
		Vector(const Real& x)
			: Base(x)
		{
		}

		// Alias for static vector is copying.
		// This is needed so that one does not have to
		// make special cases in generic programming.
		explicit Vector(const Alias<Real*>& that)
			: Base(Copy<const Real*>(that))
		{
		}

		explicit Vector(const Copy<const Real*>& that)
			: Base(that)
		{
		}

		template <typename ThatReal, int ThatN, typename Expression>
		Vector(
			const VectorExpression
			<ThatReal, ThatN, Expression>& that)
			: Base(that)
		{
		}

		Real& x()
		{
			return (*this)[0];
		}

		const Real& x() const
		{
			return (*this)[0];
		}
	};

	// Specialization for dimension 2

	template <typename Real>
	class Vector<Real, 2>
		: public Detail::VectorBase<Real, 2>
	{
	private:
		enum
		{
			N = 2
		};

		typedef Detail::VectorBase<Real, N> Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		using Base::set;
		using Base::operator=;

		Vector()
			: Base()
		{
		}

		explicit Vector(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
		{
		}

		// Alias for static vector is copying.
		// This is needed so that one does not have to
		// make special cases in generic programming.
		Vector(
			const Dimension& dimension,
			const Alias<Real*>& that)
			: Base(dimension, Copy<const Real*>(that))
		{
		}

		Vector(
			const Dimension& dimension,
			const Copy<const Real*>& that)
			: Base(dimension, that)
		{
		}

		template <typename ThatReal, int ThatN>
		explicit Vector(const Tuple<ThatReal, ThatN>& that)
			: Base(that)
		{
		}

		Vector(const Vector<Real, N>& that)
			: Base(that)
		{
		}

		template <typename ThatReal, int ThatN>
		Vector(const Vector<ThatReal, ThatN>& that)
			: Base(that)
		{
		}

		explicit Vector(const Real& x)
			: Base(x)
		{
		}

		// Alias for static vector is copying.
		// This is needed so that one does not have to
		// make special cases in generic programming.
		explicit Vector(const Alias<Real*>& that)
			: Base(Copy<const Real*>(that))
		{
		}

		explicit Vector(const Copy<const Real*>& that)
			: Base(that)
		{
		}

		Vector(const Real& x, const Real& y)
			: Base()
		{
			set(x, y);
		}

		template <typename ThatReal, int ThatN, typename Expression>
		Vector(
			const VectorExpression
			<ThatReal, ThatN, Expression>& that)
			: Base(that)
		{
		}

		void set(const Real& x, const Real& y)
		{
			Vector& v = *this;
			v[0] = x;
			v[1] = y;
		}

		Real& x()
		{
			return (*this)[0];
		}

		const Real& x() const
		{
			return (*this)[0];
		}

		Real& y()
		{
			return (*this)[1];
		}

		const Real& y() const
		{
			return (*this)[1];
		}
	};

	// Specialization for dimension 3

	template <typename Real>
	class Vector<Real, 3>
		: public Detail::VectorBase<Real, 3>
	{
	private:
		enum
		{
			N = 3
		};

		typedef Detail::VectorBase<Real, N> Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		using Base::set;
		using Base::operator=;

		Vector()
			: Base()
		{
		}

		explicit Vector(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
		{
		}

				// Alias for static vector is copying.
		// This is needed so that one does not have to
		// make special cases in generic programming.
		Vector(
			const Dimension& dimension,
			const Alias<Real*>& that)
			: Base(dimension, Copy<const Real*>(that))
		{
		}

		Vector(
			const Dimension& dimension,
			const Copy<const Real*>& that)
			: Base(dimension, that)
		{
		}

		template <typename ThatReal, int ThatN>
		explicit Vector(const Tuple<ThatReal, ThatN>& that)
			: Base(that)
		{
		}

		Vector(const Vector<Real, N>& that)
			: Base(that)
		{
		}

		template <typename ThatReal, int ThatN>
		Vector(const Vector<ThatReal, ThatN>& that)
			: Base(that)
		{
			PASTEL_STATIC_ASSERT(ThatN == N || ThatN == Dynamic);
		}

		explicit Vector(const Real& x)
			: Base(x)
		{
		}

		// Alias for static vector is copying.
		// This is needed so that one does not have to
		// make special cases in generic programming.
		explicit Vector(const Alias<Real*>& that)
			: Base(Copy<const Real*>(that))
		{
		}

		explicit Vector(const Copy<const Real*>& that)
			: Base(that)
		{
		}

		Vector(const Real& x, const Real& y,
			const Real& z)
			: Base()
		{
			set(x, y, z);
		}

		template <typename ThatReal, int ThatN, typename Expression>
		Vector(
			const VectorExpression
			<ThatReal, ThatN, Expression>& that)
			: Base(that)
		{
		}

		void set(const Real& x, const Real& y, const Real& z)
		{
			Vector& v = *this;
			v[0] = x;
			v[1] = y;
			v[2] = z;
		}

		Real& x()
		{
			return (*this)[0];
		}

		const Real& x() const
		{
			return (*this)[0];
		}

		Real& y()
		{
			return (*this)[1];
		}

		const Real& y() const
		{
			return (*this)[1];
		}

		Real& z()
		{
			return (*this)[2];
		}

		const Real& z() const
		{
			return (*this)[2];
		}
	};

	// Specialization for dimension 4

	template <typename Real>
	class Vector<Real, 4>
		: public Detail::VectorBase<Real, 4>
	{
	private:
		enum
		{
			N = 4
		};

		typedef Detail::VectorBase<Real, N> Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		using Base::set;
		using Base::operator=;

		Vector()
			: Base()
		{
		}

		explicit Vector(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
		{
		}

		// Alias for static vector is copying.
		// This is needed so that one does not have to
		// make special cases in generic programming.
		Vector(
			const Dimension& dimension,
			const Alias<Real*>& that)
			: Base(dimension, Copy<const Real*>(that))
		{
		}

		Vector(
			const Dimension& dimension,
			const Copy<const Real*>& that)
			: Base(dimension, that)
		{
		}

		template <typename ThatReal, int ThatN>
		explicit Vector(const Tuple<ThatReal, ThatN>& that)
			: Base(that)
		{
		}

		Vector(const Vector<Real, N>& that)
			: Base(that)
		{
		}

		template <typename ThatReal, int ThatN>
		Vector(const Vector<ThatReal, ThatN>& that)
			: Base(that)
		{
		}

		explicit Vector(const Real& x)
			: Base(x)
		{
		}

		// Alias for static vector is copying.
		// This is needed so that one does not have to
		// make special cases in generic programming.
		explicit Vector(const Alias<Real*>& that)
			: Base(Copy<const Real*>(that))
		{
		}

		explicit Vector(const Copy<const Real*>& that)
			: Base(that)
		{
		}

		Vector(const Real& x, const Real& y,
			const Real& z, const Real& w)
			: Base()
		{
			set(x, y, z, w);
		}

		template <typename ThatReal, int ThatN, typename Expression>
		Vector(
			const VectorExpression
			<ThatReal, ThatN, Expression>& that)
			: Base(that)
		{
		}

		void set(const Real& x, const Real& y, const Real& z,
			const Real& w)
		{
			Vector& v = *this;
			v[0] = x;
			v[1] = y;
			v[2] = z;
			v[3] = w;
		}

		Real& x()
		{
			return (*this)[0];
		}

		const Real& x() const
		{
			return (*this)[0];
		}

		Real& y()
		{
			return (*this)[1];
		}

		const Real& y() const
		{
			return (*this)[1];
		}

		Real& z()
		{
			return (*this)[2];
		}

		const Real& z() const
		{
			return (*this)[2];
		}

		Real& w()
		{
			return (*this)[3];
		}

		const Real& w() const
		{
			return (*this)[3];
		}
	};

	// Specialization for unbounded dimensions

	template <typename Real>
	class Vector<Real, Dynamic>
		: public Detail::VectorBase<Real, Dynamic>
	{
	private:
		enum
		{
			N = Dynamic
		};

		typedef Detail::VectorBase<Real, N> Base;
		using Base::data_;

	public:
		using Base::operator=;

		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		Vector()
			: Base()
		{
		}

		explicit Vector(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
		{
		}

		explicit Vector(
			const Dimension& dimension,
			const Copy<const Real*>& that)
			: Base(dimension, that)
		{
		}

		explicit Vector(
			const Dimension& dimension,
			const Alias<Real*>& that)
			: Base(dimension, that)
		{
		}

		template <typename ThatReal, int ThatN>
		explicit Vector(const Tuple<ThatReal, ThatN>& that)
			: Base(that)
		{
		}

		Vector(const Vector<Real, N>& that)
			: Base(that)
		{
		}

		template <typename ThatReal, int ThatN>
		Vector(const Vector<ThatReal, ThatN>& that)
			: Base(that)
		{
		}

		// We don't want to provide this function
		// for unbounded vectors since it makes no
		// sense, and could be misinterpreted
		// as taking in the dimension of the vector.
		/*
		explicit Vector(const Real& x)
			: Base(x)
		{
		}
		*/

		template <typename ThatReal, int ThatN, typename Expression>
		Vector(
			const VectorExpression
			<ThatReal, ThatN, Expression>& that)
			: Base(that)
		{
		}
	};

	typedef Vector<real, 1> Vector1;
	typedef Vector<real, 2> Vector2;
	typedef Vector<real, 3> Vector3;
	typedef Vector<real, 4> Vector4;
	typedef Vector<real, Dynamic> VectorD;

	typedef Vector<integer, 1> Vector1i;
	typedef Vector<integer, 2> Vector2i;
	typedef Vector<integer, 3> Vector3i;
	typedef Vector<integer, 4> Vector4i;
	typedef Vector<integer, Dynamic> VectorDi;

	typedef Vector1 UnitVector1;
	typedef Vector2 UnitVector2;
	typedef Vector3 UnitVector3;
	typedef Vector4 UnitVector4;

	template <typename Real, int N>
	void swap(Vector<Real, N>& left,
		Vector<Real, N>& right);

	template <typename Real, int N, typename Expression>
	Vector<Real, N> evaluate(
		const VectorExpression<Real, N, Expression>& that);

	template <typename Real, int N>
	Tuple<Real, N>& asTuple(
		Vector<Real, N>& that);

	template <typename Real, int N>
	const Tuple<Real, N>& asTuple(
		const Vector<Real, N>& that);

}

#include "pastel/sys/vector.hpp"

#endif
