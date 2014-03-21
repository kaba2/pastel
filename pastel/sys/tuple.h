// Description: Tuple class
// Detail: Allows to work with n-tuples

#ifndef PASTELSYS_TUPLE_H
#define PASTELSYS_TUPLE_H

#include "pastel/sys/tuplebase.h"

namespace Pastel
{

	template <int N, int NewN>
	class ModifyN
	{
	public:
		PASTEL_CONSTEXPR int Result =
			(N == Dynamic) ? Dynamic : NewN;
	};

	//! A fixed size array of the given type.
	/*!
	This class is a container for a fixed size
	array of elements and directly models the
	mathematical concept of a tuple (a finite ordered set).
	A tuple	has no associated arithmetic operators,
	see Vector and Vector for such tuples.
	*/
	template <typename Type, int N>
	class Tuple
		: public Detail::TupleBase<Type, N>
	{
	private:
		typedef Detail::TupleBase<Type, N> Base;

	public:
		typedef typename Base::Iterator Iterator;
		typedef typename Base::ConstIterator ConstIterator;

		// Using default copy constructor.
		// Using default destructor.

		Tuple()
			: Base()
		{
		}

		explicit Tuple(
			const Dimension& dimension,
			const Type& that = Type())
			: Base(dimension, that)
		{
		}

		// Alias for static tuple is copying.
		// This is needed so that one does not have to
		// make special cases in generic programming.
		Tuple(
			const Dimension& dimension,
			const Alias<Type*>& dataAlias)
			: Base(dimension, Copy<const Type*>(dataAlias))
		{
		}

		explicit Tuple(
			const Dimension& dimension,
			const Copy<const Type*>& that)
			: Base(dimension, that)
		{
		}

		explicit Tuple(const Type& that)
			: Base(that)
		{
		}

		// Alias for static tuple is copying.
		// This is needed so that one does not have to
		// make special cases in generic programming.
		explicit Tuple(const Alias<Type*>& that)
			: Base(Copy<const Type*>(that))
		{
		}

		explicit Tuple(const Copy<const Type*>& that)
			: Base(that)
		{
		}

		template <typename ThatType, int ThatN>
		Tuple(const Tuple<ThatType, ThatN>& that)
			: Base(that)
		{
		}

		Tuple<Type, N>& operator=(const Tuple& that)
		{
			return (Tuple<Type, N>&)Base::operator=(that);
		}
	};

	template <typename Type>
	class Tuple<Type, 1>
		: public Detail::TupleBase<Type, 1>
	{
	private:
		PASTEL_CONSTEXPR int N = 1;

		typedef Detail::TupleBase<Type, N> Base;

	public:
		// Using default copy constructor.
		// Using default destructor.

		using Base::set;
		typedef typename Base::Iterator Iterator;
		typedef typename Base::ConstIterator ConstIterator;

		Tuple()
			: Base()
		{
		}

		explicit Tuple(
			const Dimension& dimension,
			const Type& that = Type())
			: Base(dimension, that)
		{
		}

		// Alias for static tuple is copying.
		// This is needed so that one does not have to
		// make special cases in generic programming.
		Tuple(
			const Dimension& dimension,
			const Alias<Type*>& dataAlias)
			: Base(dimension, Copy<const Type*>(dataAlias))
		{
		}

		Tuple(
			const Dimension& dimension,
			const Copy<const Type*>& that)
			: Base(dimension, that)
		{
		}

		explicit Tuple(const Type& that)
			: Base(that)
		{
		}

		// Alias for static tuple is copying.
		// This is needed so that one does not have to
		// make special cases in generic programming.
		explicit Tuple(const Alias<Type*>& that)
			: Base(Copy<const Type*>(that))
		{
		}

		explicit Tuple(const Copy<const Type*>& that)
			: Base(that)
		{
		}

		template <typename ThatType, int ThatN>
		Tuple(const Tuple<ThatType, ThatN>& that)
			: Base(that)
		{
		}

		Tuple<Type, N>& operator=(const Tuple& that)
		{
			return (Tuple<Type, N>&)Base::operator=(that);
		}

		const Type& x() const
		{
			return (*this)[0];
		}

		Type& x()
		{
			return (*this)[0];
		}
	};

	template <typename Type>
	class Tuple<Type, 2>
		: public Detail::TupleBase<Type, 2>
	{
	private:
		PASTEL_CONSTEXPR int N = 2;

		typedef Detail::TupleBase<Type, N> Base;

	public:
		// Using default copy constructor.
		// Using default destructor.

		using Base::set;
		typedef typename Base::Iterator Iterator;
		typedef typename Base::ConstIterator ConstIterator;

		Tuple()
			: Base()
		{
		}

		explicit Tuple(
			const Dimension& dimension,
			const Type& that = Type())
			: Base(dimension, that)
		{
		}

		// Alias for static tuple is copying.
		// This is needed so that one does not have to
		// make special cases in generic programming.
		Tuple(
			const Dimension& dimension,
			const Alias<Type*>& dataAlias)
			: Base(dimension, Copy<const Type*>(dataAlias))
		{
		}

		Tuple(
			const Dimension& dimension,
			const Copy<const Type*>& that)
			: Base(dimension, that)
		{
		}

		explicit Tuple(const Type& that)
			: Base(that)
		{
		}

		// Alias for static tuple is copying.
		// This is needed so that one does not have to
		// make special cases in generic programming.
		explicit Tuple(const Alias<Type*>& that)
			: Base(Copy<const Type*>(that))
		{
		}

		explicit Tuple(const Copy<const Type*>& that)
			: Base(that)
		{
		}

		template <typename ThatType, int ThatN>
		Tuple(const Tuple<ThatType, ThatN>& that)
			: Base(that)
		{
		}

		explicit Tuple(const Type& a, const Type& b)
			: Base()
		{
			set(a, b);
		}

		Tuple<Type, N>& operator=(const Tuple& that)
		{
			return (Tuple<Type, N>&)Base::operator=(that);
		}

		void set(const Type& a, const Type& b)
		{
			(*this)[0] = a;
			(*this)[1] = b;
		}

		const Type& x() const
		{
			return (*this)[0];
		}

		Type& x()
		{
			return (*this)[0];
		}

		const Type& y() const
		{
			return (*this)[1];
		}

		Type& y()
		{
			return (*this)[1];
		}
	};

	template <typename Type>
	class Tuple<Type, 3>
		: public Detail::TupleBase<Type, 3>
	{
	private:
		PASTEL_CONSTEXPR int N = 3;

		typedef Detail::TupleBase<Type, N> Base;

	public:
		// Using default copy constructor.
		// Using default destructor.

		using Base::set;
		typedef typename Base::Iterator Iterator;
		typedef typename Base::ConstIterator ConstIterator;

		Tuple()
			: Base()
		{
		}

		explicit Tuple(
			const Dimension& dimension,
			const Type& that = Type())
			: Base(dimension, that)
		{
		}

		// Alias for static tuple is copying.
		// This is needed so that one does not have to
		// make special cases in generic programming.
		Tuple(
			const Dimension& dimension,
			const Alias<Type*>& dataAlias)
			: Base(dimension, Copy<const Type*>(dataAlias))
		{
		}

		Tuple(
			const Dimension& dimension,
			const Copy<const Type*>& that)
			: Base(dimension, that)
		{
		}

		explicit Tuple(const Type& that)
			: Base(that)
		{
		}

		// Alias for static tuple is copying.
		// This is needed so that one does not have to
		// make special cases in generic programming.
		explicit Tuple(const Alias<Type*>& that)
			: Base(Copy<const Type*>(that))
		{
		}

		explicit Tuple(const Copy<const Type*>& that)
			: Base(that)
		{
		}

		template <typename ThatType, int ThatN>
		Tuple(const Tuple<ThatType, ThatN>& that)
			: Base(that)
		{
		}

		explicit Tuple(const Type& a, const Type& b,
			const Type& c)
			: Base()
		{
			set(a, b, c);
		}

		Tuple<Type, N>& operator=(const Tuple& that)
		{
			return (Tuple<Type, N>&)Base::operator=(that);
		}

		void set(const Type& a, const Type& b, const Type& c)
		{
			(*this)[0] = a;
			(*this)[1] = b;
			(*this)[2] = c;
		}

		const Type& x() const
		{
			return (*this)[0];
		}

		Type& x()
		{
			return (*this)[0];
		}

		const Type& y() const
		{
			return (*this)[1];
		}

		Type& y()
		{
			return (*this)[1];
		}

		const Type& z() const
		{
			return (*this)[1];
		}

		Type& z()
		{
			return (*this)[1];
		}
	};

	template <typename Type>
	class Tuple<Type, 4>
		: public Detail::TupleBase<Type, 4>
	{
	private:
		PASTEL_CONSTEXPR int N = 4;

		typedef Detail::TupleBase<Type, N> Base;

	public:
		// Using default copy constructor.
		// Using default destructor.

		using Base::set;
		typedef typename Base::Iterator Iterator;
		typedef typename Base::ConstIterator ConstIterator;

		Tuple()
			: Base()
		{
		}

		explicit Tuple(
			const Dimension& dimension,
			const Type& that = Type())
			: Base(dimension, that)
		{
		}

		// Alias for static tuple is copying.
		// This is needed so that one does not have to
		// make special cases in generic programming.
		Tuple(
			const Dimension& dimension,
			const Alias<Type*>& dataAlias)
			: Base(dimension, Copy<const Type*>(dataAlias))
		{
		}

		Tuple(
			const Dimension& dimension,
			const Copy<const Type*>& that)
			: Base(dimension, that)
		{
		}

		explicit Tuple(const Type& that)
			: Base(that)
		{
		}

		// Alias for static tuple is copying.
		// This is needed so that one does not have to
		// make special cases in generic programming.
		explicit Tuple(const Alias<Type*>& that)
			: Base(Copy<const Type*>(that))
		{
		}

		explicit Tuple(const Copy<const Type*>& that)
			: Base(that)
		{
		}

		template <typename ThatType, int ThatN>
		Tuple(const Tuple<ThatType, ThatN>& that)
			: Base(that)
		{
		}

		explicit Tuple(const Type& a, const Type& b,
			const Type& c, const Type& d)
			: Base()
		{
			set(a, b, c, d);
		}

		Tuple<Type, N>& operator=(const Tuple& that)
		{
			return (Tuple<Type, N>&)Base::operator=(that);
		}

		void set(const Type& a, const Type& b,
			const Type& c, const Type& d)
		{
			(*this)[0] = a;
			(*this)[1] = b;
			(*this)[2] = c;
			(*this)[3] = d;
		}

		const Type& x() const
		{
			return (*this)[0];
		}

		Type& x()
		{
			return (*this)[0];
		}

		const Type& y() const
		{
			return (*this)[1];
		}

		Type& y()
		{
			return (*this)[1];
		}

		const Type& z() const
		{
			return (*this)[1];
		}

		Type& z()
		{
			return (*this)[1];
		}

		const Type& w() const
		{
			return (*this)[1];
		}

		Type& w()
		{
			return (*this)[1];
		}
	};

	template <typename Type>
	class Tuple<Type, Dynamic>
		: public Detail::TupleBase<Type, Dynamic>
	{
	private:
		PASTEL_CONSTEXPR int N = Dynamic;

		typedef Detail::TupleBase<Type, N> Base;

	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		using Base::swap;
		typedef typename Base::Iterator Iterator;
		typedef typename Base::ConstIterator ConstIterator;

		Tuple()
			: Base()
		{
		}

		explicit Tuple(
			const Dimension& dimension,
			const Type& that = Type())
			: Base(dimension, that)
		{
		}

		Tuple(
			const Dimension& dimension,
			const Copy<const Type*>& that)
			: Base(dimension, that)
		{
		}

		// This function makes no sense
		// for unbounded tuples, and thus
		// we prohibit it.
		/*
		explicit Tuple(const Type& that)
			: Base(that)
		{
		}
		*/

		Tuple(const Tuple& that)
			: Base(that)
		{
		}

		template <typename ThatType, int ThatN>
		Tuple(const Tuple<ThatType, ThatN>& that)
			: Base(that)
		{
		}

		Tuple(
			const Tuple& that,
			const Dimension& dimension,
			const Type& defaultData = Type())
			: Base(that, dimension, defaultData)
		{
		}

		template <typename ThatType>
		Tuple(const Tuple<ThatType, N>& that,
			const Dimension& dimension,
			const Type& defaultData = Type())
			: Base(that, dimension, defaultData)
		{
		}

		Tuple(
			const Dimension& dimension,
			const Alias<Type*>& dataAlias)
			: Base(dimension, dataAlias)
		{
		}

		Tuple<Type, N>& operator=(const Tuple& that)
		{
			return (Tuple<Type, N>&)Base::operator=(that);
		}
	};

	typedef Tuple<integer, 1> Integer1;
	typedef Tuple<real, 1> Real1;

	typedef Tuple<integer, 2> Integer2;
	typedef Tuple<real, 2> Real2;

	typedef Tuple<integer, 3> Integer3;
	typedef Tuple<real, 3> Real3;

	typedef Tuple<integer, 4> Integer4;
	typedef Tuple<real, 4> Real4;

	typedef Tuple<integer, Dynamic> IntegerD;
	typedef Tuple<real, Dynamic> RealD;

}

#include "pastel/sys/tuple.hpp"

#endif
