/*!
\file
\brief A class for a fixed tuple of elements.
*/

#ifndef PASTELSYS_TUPLE_H
#define PASTELSYS_TUPLE_H

#include "pastel/sys/tuplebase.h"

namespace Pastel
{

	//! A fixed size array of the given type.

	/*!
	This class is a container for a fixed size
	array of elements and directly models the
	mathematical concept of a tuple (a finite ordered set).
	A tuple	has no associated arithmetic operations,
	see Vector and Point for such tuples.
	*/

	template <int N, typename Type>
	class Tuple
		: public Detail::TupleBase<N, Type, Tuple<N, Type> >
	{
	private:
		typedef Detail::TupleBase<N, Type, Tuple<N, Type> > Base;

	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		Tuple()
			: Base()
		{
		}

		explicit Tuple(const Type& that)
			: Base(that)
		{
		}

		template <typename OtherType>
		Tuple(const Tuple<N, OtherType>& that)
			: Base(that)
		{
		}
	};

	template <typename Type>
	class Tuple<1, Type>
		: public Detail::TupleBase<1, Type, Tuple<1, Type> >
	{
	private:
		typedef Detail::TupleBase<1, Type, Tuple<1, Type> > Base;

	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		using Base::set;

		Tuple()
			: Base()
		{
		}

		explicit Tuple(const Type& that)
			: Base(that)
		{
		}

		template <typename OtherType>
		Tuple(const Tuple<1, OtherType>& that)
			: Base(that)
		{
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
	class Tuple<2, Type>
		: public Detail::TupleBase<2, Type, Tuple<2, Type> >
	{
	private:
		typedef Detail::TupleBase<2, Type, Tuple<2, Type> > Base;

	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		using Base::set;

		Tuple()
			: Base()
		{
		}

		explicit Tuple(const Type& that)
			: Base(that)
		{
		}

		template <typename OtherType>
		Tuple(const Tuple<2, OtherType>& that)
			: Base(that)
		{
		}

		explicit Tuple(const Type& a, const Type& b)
			: Base()
		{
			set(a, b);
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
	class Tuple<3, Type>
		: public Detail::TupleBase<3, Type, Tuple<3, Type> >
	{
	private:
		typedef Detail::TupleBase<3, Type, Tuple<3, Type> > Base;

	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		using Base::set;

		Tuple()
			: Base()
		{
		}

		explicit Tuple(const Type& that)
			: Base(that)
		{
		}

		template <typename OtherType>
		Tuple(const Tuple<3, OtherType>& that)
			: Base(that)
		{
		}

		explicit Tuple(const Type& a, const Type& b,
			const Type& c)
			: Base()
		{
			set(a, b, c);
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
	class Tuple<4, Type>
		: public Detail::TupleBase<4, Type, Tuple<4, Type> >
	{
	private:
		typedef Detail::TupleBase<4, Type, Tuple<4, Type> > Base;

	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		using Base::set;

		Tuple()
			: Base()
		{
		}

		explicit Tuple(const Type& that)
			: Base(that)
		{
		}

		template <typename OtherType>
		Tuple(const Tuple<4, OtherType>& that)
			: Base(that)
		{
		}

		explicit Tuple(const Type& a, const Type& b,
			const Type& c, const Type& d)
			: Base()
		{
			set(a, b, c, d);
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

	typedef Tuple<1, integer> Integer1;
	typedef Tuple<1, real> Real1;

	typedef Tuple<2, integer> Integer2;
	typedef Tuple<2, real> Real2;

	typedef Tuple<3, integer> Integer3;
	typedef Tuple<3, real> Real3;

	typedef Tuple<4, integer> Integer4;
	typedef Tuple<4, real> Real4;

}

#include "pastel/sys/tuple.hpp"

#endif
