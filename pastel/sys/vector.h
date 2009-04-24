/*!
\file
\brief A class for a real vector in R^N.
*/

#ifndef PASTELSYS_VECTOR_H
#define PASTELSYS_VECTOR_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vectorbase.h"

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
	is used to ensure unrolling of static sized loops.
	*/

	/*
	"Curiously recurring template pattern" (CRTP) is used
	to place common functionality into VectorBase, so
	we can easily specialize the lower dimensions to support
	list initialization in constructor and set().
	The "friend trick" is used to implement correctly
	working left hand operators.
	*/

	template <int N, typename Real>
	class Vector
		: public Detail::VectorBase<N, Real>
	{
	private:
		typedef Detail::VectorBase<N, Real> Base;

	public:
		using Base::operator=;

		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		Vector()
			: Base()
		{
		}

		Vector(const TemporaryVector<N, Real>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		explicit Vector(const Tuple<N, ThatReal>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		Vector(const Vector<N, ThatReal>& that)
			: Base(that)
		{
		}

		explicit Vector(const Real& x)
			: Base(x)
		{
		}

		template <typename ThatReal, typename Expression>
		Vector(
			const VectorExpression
			<N, ThatReal, Expression>& that)
			: Base(that)
		{
		}
	};

	// Specialization for unbounded dimensions

	template <typename Real>
	class Vector<Unbounded, Real>
		: public Detail::VectorBase<Unbounded, Real>
	{
	private:
		typedef Detail::VectorBase<Unbounded, Real> Base;
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

		Vector(const TemporaryVector<Unbounded, Real>& that)
			: Base(that)
		{
		}

		// We want to force giving the initialization value.
		// This is because the size alone could be mixed
		// with single-value initialization with
		// finite dimensional vectors (which does not
		// make sense for unbounded vectors).
		explicit Vector(integer size, const Real& that)
			: Base()
		{
			setSize(size, that);
		}

		template <typename ThatReal>
		explicit Vector(const Tuple<Unbounded, ThatReal>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		Vector(const Vector<Unbounded, ThatReal>& that)
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

		template <typename ThatReal, typename Expression>
		Vector(
			const VectorExpression
			<Unbounded, ThatReal, Expression>& that)
			: Base(that)
		{
		}
	};

	// Specialization for dimension 1

	template <typename Real>
	class Vector<1, Real>
		: public Detail::VectorBase<1, Real>
	{
	private:
		typedef Detail::VectorBase<1, Real> Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		using Base::operator=;

		Vector()
			: Base()
		{
		}

		Vector(const TemporaryVector<1, Real>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		explicit Vector(const Tuple<1, ThatReal>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		Vector(const Vector<1, ThatReal>& that)
			: Base(that)
		{
		}

		// Implicit conversion allowed.
		Vector(const Real& x)
			: Base(x)
		{
		}

		template <typename ThatReal, typename Expression>
		Vector(
			const VectorExpression
			<1, ThatReal, Expression>& that)
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
	class Vector<2, Real>
		: public Detail::VectorBase<2, Real>
	{
	private:
		typedef Detail::VectorBase<2, Real> Base;

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

		Vector(const TemporaryVector<2, Real>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		explicit Vector(const Tuple<2, ThatReal>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		Vector(const Vector<2, ThatReal>& that)
			: Base(that)
		{
		}

		explicit Vector(const Real& x)
			: Base(x)
		{
		}

		Vector(const Real& x, const Real& y)
			: Base()
		{
			Vector& v = *this;
			v[0] = x;
			v[1] = y;
		}

		template <typename ThatReal, typename Expression>
		Vector(
			const VectorExpression
			<2, ThatReal, Expression>& that)
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
	class Vector<3, Real>
		: public Detail::VectorBase<3, Real>
	{
	private:
		typedef Detail::VectorBase<3, Real> Base;

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

		Vector(const TemporaryVector<3, Real>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		explicit Vector(const Tuple<3, ThatReal>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		Vector(const Vector<3, ThatReal>& that)
			: Base(that)
		{
		}

		explicit Vector(const Real& x)
			: Base(x)
		{
		}

		Vector(const Real& x, const Real& y, const Real& z)
			: Base()
		{
			Vector& v = *this;
			v[0] = x;
			v[1] = y;
			v[2] = z;
		}

		template <typename ThatReal, typename Expression>
		Vector(
			const VectorExpression
			<3, ThatReal, Expression>& that)
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
	class Vector<4, Real>
		: public Detail::VectorBase<4, Real>
	{
	private:
		typedef Detail::VectorBase<4, Real> Base;

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

		Vector(const TemporaryVector<4, Real>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		explicit Vector(const Tuple<4, ThatReal>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		Vector(const Vector<4, ThatReal>& that)
			: Base(that)
		{
		}

		explicit Vector(const Real& x)
			: Base(x)
		{
		}

		Vector(const Real& x, const Real& y, const Real& z,
			const Real& w)
			: Base()
		{
			Vector& v = *this;
			v[0] = x;
			v[1] = y;
			v[2] = z;
			v[3] = w;
		}

		template <typename ThatReal, typename Expression>
		Vector(
			const VectorExpression
			<4, ThatReal, Expression>& that)
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

	template <int N, typename Real>
	void swap(Vector<N, Real>& left,
		Vector<N, Real>& right);

	template <int N, typename Real, typename Expression>
	Vector<N, Real> evaluate(
		const VectorExpression<N, Real, Expression>& that);

	template <int N, typename Real>
	Tuple<N, Real> asTuple(Vector<N, Real>& that);

	template <int N, typename Real>
	const Tuple<N, Real> asTuple(const Vector<N, Real>& that);

	typedef Vector<Unbounded, real> UnboundedVector;
	typedef Vector<1, real> Vector1;
	typedef Vector<2, real> Vector2;
	typedef Vector<3, real> Vector3;
	typedef Vector<4, real> Vector4;

	typedef Vector<Unbounded, integer> UnboundedIVector;
	typedef Vector<1, integer> IVector1;
	typedef Vector<2, integer> IVector2;
	typedef Vector<3, integer> IVector3;
	typedef Vector<4, integer> IVector4;

	typedef Vector1 UnitVector1;
	typedef Vector2 UnitVector2;
	typedef Vector3 UnitVector3;
	typedef Vector4 UnitVector4;

	template <int N, typename Real>
	class TemporaryVector
		: public Vector<N, Real>
	{
	private:
		typedef Vector<N, Real> Base;

	public:
		using Base::operator=;
		using Base::asTuple;

		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		TemporaryVector()
			: Base()
		{
		}

		TemporaryVector(integer size, const Real& that)
			: Base(that)
		{
			PENSURE1(size == N, size);
		}

		TemporaryVector(const TemporaryVector& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		explicit TemporaryVector(const Tuple<N, ThatReal>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		TemporaryVector(const Vector<N, ThatReal>& that)
			: Base(that)
		{
		}

		explicit TemporaryVector(const Real& x)
			: Base(x)
		{
		}

		template <typename ThatReal, typename Expression>
		TemporaryVector(
			const VectorExpression
			<N, ThatReal, Expression>& that)
			: Base(that)
		{
		}

		TemporaryTuple<N, Real>& asTemporaryTuple()
		{
			return asTuple().asTemporary();
		}

		const TemporaryTuple<N, Real>& asTemporaryTuple() const
		{
			return (const TemporaryTuple<N, Real>&)asTuple();
		}
	};

	template <typename Real>
	class TemporaryVector<1, Real>
		: public Vector<1, Real>
	{
	private:
		typedef Vector<1, Real> Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		using Base::operator=;
		using Base::asTuple;

		TemporaryVector()
			: Base()
		{
		}

		TemporaryVector(integer size, const Real& that)
			: Base(that)
		{
			PENSURE1(size == 1, size);
		}

		TemporaryVector(const TemporaryVector& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		explicit TemporaryVector(const Tuple<1, ThatReal>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		TemporaryVector(const Vector<1, ThatReal>& that)
			: Base(that)
		{
		}

		// Implicit conversion allowed.
		TemporaryVector(const Real& x)
			: Base(x)
		{
		}

		template <typename ThatReal, typename Expression>
		TemporaryVector(
			const VectorExpression
			<1, ThatReal, Expression>& that)
			: Base(that)
		{
		}

		TemporaryTuple<1, Real>& asTemporaryTuple()
		{
			return asTuple().asTemporary();
		}

		const TemporaryTuple<1, Real>& asTemporaryTuple() const
		{
			return (const TemporaryTuple<1, Real>&)asTuple();
		}
	};

	// Specialization for dimension 2

	template <typename Real>
	class TemporaryVector<2, Real>
		: public Vector<2, Real>
	{
	private:
		typedef Vector<2, Real> Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		using Base::operator=;
		using Base::asTuple;

		TemporaryVector()
			: Base()
		{
		}

		TemporaryVector(integer size, const Real& that)
			: Base(that)
		{
			PENSURE1(size == 2, size);
		}

		TemporaryVector(const TemporaryVector& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		explicit TemporaryVector(const Tuple<2, ThatReal>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		TemporaryVector(const Vector<2, ThatReal>& that)
			: Base(that)
		{
		}

		explicit TemporaryVector(const Real& x)
			: Base(x)
		{
		}

		TemporaryVector(const Real& x, const Real& y)
			: Base(x, y)
		{
		}

		template <typename ThatReal, typename Expression>
		TemporaryVector(
			const VectorExpression
			<2, ThatReal, Expression>& that)
			: Base(that)
		{
		}

		TemporaryTuple<2, Real>& asTemporaryTuple()
		{
			return asTuple().asTemporary();
		}

		const TemporaryTuple<2, Real>& asTemporaryTuple() const
		{
			return (const TemporaryTuple<2, Real>&)asTuple();
		}
	};

	// Specialization for dimension 3

	template <typename Real>
	class TemporaryVector<3, Real>
		: public Vector<3, Real>
	{
	private:
		typedef Vector<3, Real> Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		using Base::operator=;
		using Base::asTuple;

		TemporaryVector()
			: Base()
		{
		}

		TemporaryVector(integer size, const Real& that)
			: Base(that)
		{
			PENSURE1(size == 3, size);
		}

		TemporaryVector(const TemporaryVector& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		explicit TemporaryVector(const Tuple<3, ThatReal>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		TemporaryVector(const Vector<3, ThatReal>& that)
			: Base(that)
		{
		}

		explicit TemporaryVector(const Real& x)
			: Base(x)
		{
		}

		TemporaryVector(const Real& x, const Real& y, const Real& z)
			: Base(x, y, z)
		{
		}

		template <typename ThatReal, typename Expression>
		TemporaryVector(
			const VectorExpression
			<3, ThatReal, Expression>& that)
			: Base(that)
		{
		}

		TemporaryTuple<3, Real>& asTemporaryTuple()
		{
			return asTuple().asTemporary();
		}

		const TemporaryTuple<3, Real>& asTemporaryTuple() const
		{
			return (const TemporaryTuple<3, Real>&)asTuple();
		}
	};

	// Specialization for dimension 4

	template <typename Real>
	class TemporaryVector<4, Real>
		: public Vector<4, Real>
	{
	private:
		typedef Vector<4, Real> Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		using Base::operator=;
		using Base::asTuple;

		TemporaryVector()
			: Base()
		{
		}

		TemporaryVector(integer size, const Real& that)
			: Base(that)
		{
			PENSURE1(size == 4, size);
		}

		TemporaryVector(const TemporaryVector& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		explicit TemporaryVector(const Tuple<4, ThatReal>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		TemporaryVector(const Vector<4, ThatReal>& that)
			: Base(that)
		{
		}

		explicit TemporaryVector(const Real& x)
			: Base(x)
		{
		}

		TemporaryVector(const Real& x, const Real& y, const Real& z,
			const Real& w)
			: Base(x, y, z, w)
		{
		}

		template <typename ThatReal, typename Expression>
		TemporaryVector(
			const VectorExpression
			<4, ThatReal, Expression>& that)
			: Base(that)
		{
		}

		TemporaryTuple<4, Real>& asTemporaryTuple()
		{
			return asTuple().asTemporary();
		}

		const TemporaryTuple<4, Real>& asTemporaryTuple() const
		{
			return (const TemporaryTuple<4, Real>&)asTuple();
		}
	};

}

#include "pastel/sys/vector.hpp"

#endif
