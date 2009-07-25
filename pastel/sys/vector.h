// Description: Vector class
// Detail: Allows to compute with vectors in R^n

#ifndef PASTEL_VECTOR_H
#define PASTEL_VECTOR_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vectorbase.h"

#include <boost/utility/enable_if.hpp>

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

		explicit Vector(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
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

		Vector(const Vector<N, Real>& that)
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

	// Specialization for dimension 1

	template <typename Real>
	class Vector<1, Real>
		: public Detail::VectorBase<1, Real>
	{
	private:
		enum
		{
			N = 1
		};

		typedef Detail::VectorBase<N, Real> Base;

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

		Vector(const TemporaryVector<N, Real>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		explicit Vector(const Tuple<N, ThatReal>& that)
			: Base(that)
		{
		}

		Vector(const Vector<N, Real>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		Vector(const Vector<N, ThatReal>& that)
			: Base(that)
		{
		}

		// Implicit conversion allowed
		// only for Nd version.
		Vector(const Real& x)
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
		enum
		{
			N = 2
		};

		typedef Detail::VectorBase<N, Real> Base;

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

		Vector(const TemporaryVector<N, Real>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		explicit Vector(const Tuple<N, ThatReal>& that)
			: Base(that)
		{
		}

		Vector(const Vector<N, Real>& that)
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

		Vector(const Real& x, const Real& y)
			: Base()
		{
			set(x, y);
		}

		template <typename ThatReal, typename Expression>
		Vector(
			const VectorExpression
			<N, ThatReal, Expression>& that)
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
		enum
		{
			N = 3
		};

		typedef Detail::VectorBase<N, Real> Base;

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

		Vector(const TemporaryVector<N, Real>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		explicit Vector(const Tuple<N, ThatReal>& that)
			: Base(that)
		{
		}

		Vector(const Vector<N, Real>& that)
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

		Vector(const Real& x, const Real& y,
			const Real& z)
			: Base()
		{
			set(x, y, z);
		}

		template <typename ThatReal, typename Expression>
		Vector(
			const VectorExpression
			<N, ThatReal, Expression>& that)
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
		enum
		{
			N = 4
		};

		typedef Detail::VectorBase<N, Real> Base;

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

		Vector(const TemporaryVector<N, Real>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		explicit Vector(const Tuple<N, ThatReal>& that)
			: Base(that)
		{
		}

		Vector(const Vector<N, Real>& that)
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

		Vector(const Real& x, const Real& y,
			const Real& z, const Real& w)
			: Base()
		{
			set(x, y, z, w);
		}

		template <typename ThatReal, typename Expression>
		Vector(
			const VectorExpression
			<N, ThatReal, Expression>& that)
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
	class Vector<Dynamic, Real>
		: public Detail::VectorBase<Dynamic, Real>
	{
	private:
		enum
		{
			N = Dynamic
		};

		typedef Detail::VectorBase<N, Real> Base;
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
			const Alias<Real*>& that)
			: Base(dimension, that)
		{
		}

		Vector(const TemporaryVector<N, Real>& that)
			: Base(that)
		{
		}

		explicit Vector(const TemporaryTuple<N, Real>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		explicit Vector(const Tuple<N, ThatReal>& that)
			: Base(that)
		{
		}

		Vector(const Vector<N, Real>& that)
			: Base(that)
		{
		}

		template <typename ThatReal>
		Vector(const Vector<N, ThatReal>& that)
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
			<N, ThatReal, Expression>& that)
			: Base(that)
		{
		}

		// This function can't be included because
		// then assignment of a vector expression would
		// be ambiguous. This is because TemporaryVector
		// has an implicit conversion from a vector
		// expression.
		/*
		Vector<N, Real>& operator=(
			const TemporaryVector<N, Real>& that)
		{
			return Base::operator=(that);
		}
		*/
	};

	typedef Vector<1, real> Vector1;
	typedef Vector<2, real> Vector2;
	typedef Vector<3, real> Vector3;
	typedef Vector<4, real> Vector4;
	typedef Vector<Dynamic, real> VectorD;

	typedef Vector<1, integer> Vector1i;
	typedef Vector<2, integer> Vector2i;
	typedef Vector<3, integer> Vector3i;
	typedef Vector<4, integer> Vector4i;
	typedef Vector<Dynamic, integer> VectorDi;

	typedef Vector1 UnitVector1;
	typedef Vector2 UnitVector2;
	typedef Vector3 UnitVector3;
	typedef Vector4 UnitVector4;

	template <int N, typename Real>
	void swap(Vector<N, Real>& left,
		Vector<N, Real>& right);

	template <int N, typename Real>
	TemporaryVector<N, Real> nullVector();

	template <int N, typename Real>
	TemporaryVector<N, Real> aliasVector(integer dimension,
		PASTEL_NO_DEDUCTION(Real)* data);

	template <int N, typename Real, typename Expression>
	TemporaryVector<N, Real> evaluate(
		const VectorExpression<N, Real, Expression>& that);

	template <int N, typename Real>
	Tuple<N, Real>& asTuple(
		Vector<N, Real>& that);

	template <int N, typename Real>
	const Tuple<N, Real>& asTuple(
		const Vector<N, Real>& that);

	template <int N, typename Real>
	TemporaryTuple<N, Real>& asTuple(
		TemporaryVector<N, Real>& that);

	template <int N, typename Real>
	const TemporaryTuple<N, Real>& asTuple(
		const TemporaryVector<N, Real>& that);

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

		explicit TemporaryVector(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
		{
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

		TemporaryTuple<N, Real>& asTuple()
		{
			return Base::asTuple().asTemporary();
		}

		const TemporaryTuple<N, Real>& asTuple() const
		{
			return ((Tuple<N, Real>&)Base::asTuple()).asTemporary();
		}
	};

	template <typename Real>
	class TemporaryVector<1, Real>
		: public Vector<1, Real>
	{
	private:
		enum
		{
			N = 1
		};

		typedef Vector<N, Real> Base;

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

		explicit TemporaryVector(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
		{
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

		// Implicit conversion allowed.
		TemporaryVector(const Real& x)
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

		TemporaryTuple<N, Real>& asTuple()
		{
			return Base::asTuple().asTemporary();
		}

		const TemporaryTuple<N, Real>& asTuple() const
		{
			return ((Tuple<N, Real>&)Base::asTuple()).asTemporary();
		}
	};

	// Specialization for dimension 2

	template <typename Real>
	class TemporaryVector<2, Real>
		: public Vector<2, Real>
	{
	private:
		enum
		{
			N = 2
		};

		typedef Vector<N, Real> Base;

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

		explicit TemporaryVector(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
		{
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

		TemporaryVector(const Real& x, const Real& y)
			: Base(x, y)
		{
		}

		template <typename ThatReal, typename Expression>
		TemporaryVector(
			const VectorExpression
			<N, ThatReal, Expression>& that)
			: Base(that)
		{
		}

		TemporaryTuple<N, Real>& asTuple()
		{
			return Base::asTuple().asTemporary();
		}

		const TemporaryTuple<N, Real>& asTuple() const
		{
			return ((Tuple<N, Real>&)Base::asTuple()).asTemporary();
		}
	};

	// Specialization for dimension 3

	template <typename Real>
	class TemporaryVector<3, Real>
		: public Vector<3, Real>
	{
	private:
		enum
		{
			N = 3
		};

		typedef Vector<N, Real> Base;

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

		explicit TemporaryVector(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
		{
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

		TemporaryVector(const Real& x, const Real& y,
			const Real& z)
			: Base(x, y, z)
		{
		}

		template <typename ThatReal, typename Expression>
		TemporaryVector(
			const VectorExpression
			<N, ThatReal, Expression>& that)
			: Base(that)
		{
		}

		TemporaryTuple<N, Real>& asTuple()
		{
			return Base::asTuple().asTemporary();
		}

		const TemporaryTuple<N, Real>& asTuple() const
		{
			return ((Tuple<N, Real>&)Base::asTuple()).asTemporary();
		}
	};

	// Specialization for dimension 4

	template <typename Real>
	class TemporaryVector<4, Real>
		: public Vector<4, Real>
	{
	private:
		enum
		{
			N = 4
		};

		typedef Vector<N, Real> Base;

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

		explicit TemporaryVector(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
		{
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

		TemporaryVector(const Real& x, const Real& y,
			const Real& z, const Real& w)
			: Base(x, y, z, w)
		{
		}

		template <typename ThatReal, typename Expression>
		TemporaryVector(
			const VectorExpression
			<N, ThatReal, Expression>& that)
			: Base(that)
		{
		}

		TemporaryTuple<N, Real>& asTuple()
		{
			return Base::asTuple().asTemporary();
		}

		const TemporaryTuple<N, Real>& asTuple() const
		{
			return ((Tuple<N, Real>&)Base::asTuple()).asTemporary();
		}
	};

	// Specialization for unbounded vectors.

	template <typename Real>
	class TemporaryVector<Dynamic, Real>
		: public Vector<Dynamic, Real>
	{
	private:
		enum
		{
			N = Dynamic
		};

		typedef Vector<N, Real> Base;

	public:
		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		using Base::operator=;
		using Base::asTuple;

		// We force the specification of
		// dimension by not allowing
		// default construction.
		/*
		TemporaryVector()
			: Base()
		{
		}
		*/

		explicit TemporaryVector(
			const Dimension& dimension,
			const Real& that = Real())
			: Base(dimension, that)
		{
		}

		explicit TemporaryVector(
			const Dimension& dimension,
			const Alias<Real*>& that)
			: Base(dimension, that)
		{
		}

		TemporaryVector(const TemporaryVector& that)
			: Base(that)
		{
		}

		explicit TemporaryVector(const TemporaryTuple<N, Real>& that)
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

		TemporaryVector(const Real& x, const Real& y,
			const Real& z, const Real& w)
			: Base(x, y, z, w)
		{
		}

		template <typename ThatReal, typename Expression>
		TemporaryVector(
			const VectorExpression
			<N, ThatReal, Expression>& that)
			: Base(that)
		{
		}

		TemporaryTuple<N, Real>& asTuple()
		{
			return Base::asTuple().asTemporary();
		}

		const TemporaryTuple<N, Real>& asTuple() const
		{
			return ((Tuple<N, Real>&)Base::asTuple()).asTemporary();
		}
	};

}

#include "pastel/sys/vector.hpp"

#endif
