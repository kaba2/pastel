/*!
\file
\brief A class for a fixed tuple of elements.
*/

#ifndef PASTELSYS_TUPLE_H
#define PASTELSYS_TUPLE_H

#include "pastel/sys/tuplebase.h"

namespace Pastel
{

	template <int N, typename Type>
	class TemporaryTuple;

	//! A fixed size array of the given type.

	/*!
	This class is a container for a fixed size
	array of elements and directly models the
	mathematical concept of a tuple (a finite ordered set).
	A tuple	has no associated arithmetic operators,
	see Vector and Point for such tuples.
	*/

	template <int N, typename Type>
	class Tuple
		: public Detail::TupleBase<N, Type>
	{
	private:
		typedef Detail::TupleBase<N, Type> Base;

	public:
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

		explicit Tuple(const Type& that)
			: Base(that)
		{
		}

		Tuple(const TemporaryTuple<N, Type>& that)
			: Base(that)
		{
		}

		template <typename OtherType>
		Tuple(const Tuple<N, OtherType>& that)
			: Base(that)
		{
		}

		Tuple<N, Type>& operator=(const Tuple& that)
		{
			return (Tuple<N, Type>&)Base::operator=(that);
		}
	};

	template <typename Type>
	class Tuple<1, Type>
		: public Detail::TupleBase<1, Type>
	{
	private:
		enum
		{
			N  = 1
		};

		typedef Detail::TupleBase<N, Type> Base;

	public:
		// Using default copy constructor.
		// Using default destructor.

		using Base::set;

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

		explicit Tuple(const Type& that)
			: Base(that)
		{
		}

		Tuple(const TemporaryTuple<N, Type>& that)
			: Base(that)
		{
		}

		template <typename OtherType>
		Tuple(const Tuple<N, OtherType>& that)
			: Base(that)
		{
		}

		Tuple<N, Type>& operator=(const Tuple& that)
		{
			return (Tuple<N, Type>&)Base::operator=(that);
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
		: public Detail::TupleBase<2, Type>
	{
	private:
		enum
		{
			N  = 2
		};

		typedef Detail::TupleBase<N, Type> Base;

	public:
		// Using default copy constructor.
		// Using default destructor.

		using Base::set;

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

		explicit Tuple(const Type& that)
			: Base(that)
		{
		}

		Tuple(const TemporaryTuple<N, Type>& that)
			: Base(that)
		{
		}

		template <typename OtherType>
		Tuple(const Tuple<N, OtherType>& that)
			: Base(that)
		{
		}

		explicit Tuple(const Type& a, const Type& b)
			: Base()
		{
			set(a, b);
		}

		Tuple<N, Type>& operator=(const Tuple& that)
		{
			return (Tuple<N, Type>&)Base::operator=(that);
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
		: public Detail::TupleBase<3, Type>
	{
	private:
		enum
		{
			N = 3
		};

		typedef Detail::TupleBase<N, Type> Base;

	public:
		// Using default copy constructor.
		// Using default destructor.

		using Base::set;

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

		explicit Tuple(const Type& that)
			: Base(that)
		{
		}

		Tuple(const TemporaryTuple<N, Type>& that)
			: Base(that)
		{
		}

		template <typename OtherType>
		Tuple(const Tuple<N, OtherType>& that)
			: Base(that)
		{
		}

		explicit Tuple(const Type& a, const Type& b,
			const Type& c)
			: Base()
		{
			set(a, b, c);
		}

		Tuple<N, Type>& operator=(const Tuple& that)
		{
			return (Tuple<N, Type>&)Base::operator=(that);
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
		: public Detail::TupleBase<4, Type>
	{
	private:
		enum
		{
			N = 4
		};

		typedef Detail::TupleBase<N, Type> Base;

	public:
		// Using default copy constructor.
		// Using default destructor.

		using Base::set;

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

		explicit Tuple(const Type& that)
			: Base(that)
		{
		}

		Tuple(const TemporaryTuple<N, Type>& that)
			: Base(that)
		{
		}

		template <typename OtherType>
		Tuple(const Tuple<N, OtherType>& that)
			: Base(that)
		{
		}

		explicit Tuple(const Type& a, const Type& b,
			const Type& c, const Type& d)
			: Base()
		{
			set(a, b, c, d);
		}

		Tuple<N, Type>& operator=(const Tuple& that)
		{
			return (Tuple<N, Type>&)Base::operator=(that);
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
	class Tuple<Dynamic, Type>
		: public Detail::TupleBase<Dynamic, Type>
	{
	private:
		enum
		{
			N = Dynamic
		};

		typedef Detail::TupleBase<N, Type> Base;

	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		using Base::swap;

		// We prohibit default construction
		// to force specifying the dimension
		// for unbounded tuples.
		/*
		Tuple()
			: Base()
		{
		}
		*/

		explicit Tuple(
			const Dimension& dimension,
			const Type& that = Type())
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

		template <typename ThatType>
		Tuple(const Tuple<N, ThatType>& that)
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
		Tuple(const Tuple<N, ThatType>& that,
			const Dimension& dimension,
			const Type& defaultData = Type())
			: Base(that, dimension, defaultData)
		{
		}

		Tuple(
			const TemporaryTuple<N, Type>& that)
			: Base(that)
		{
		}

		// The TemporaryTuple's of ThatType
		// are handled via the Tuple<N, ThatType>
		// constructor.

		Tuple(
			const Dimension& dimension,
			const Alias<Type*>& dataAlias)
			: Base(dimension, dataAlias)
		{
		}

		Tuple<N, Type>& operator=(const Tuple& that)
		{
			return (Tuple<N, Type>&)Base::operator=(that);
		}

		Tuple<N, Type>& operator=(
			const TemporaryTuple<N, Type>& that)
		{
			Tuple<N, Type> copy(that);
			swap(copy);
			return *this;
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

	template <int N, typename Type>
	class TemporaryTuple
		: public Tuple<N, Type>
	{
	private:
		typedef Tuple<N, Type> Base;

	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		TemporaryTuple()
			: Base()
		{
		}

		explicit TemporaryTuple(
			const Dimension& dimension,
			const Type& that = Type())
			: Base(dimension, that)
		{
		}

		explicit TemporaryTuple(const Type& that)
			: Base(that)
		{
		}

		TemporaryTuple(const TemporaryTuple& that)
			: Base(that)
		{
		}

		template <typename OtherType>
		TemporaryTuple(const TemporaryTuple<N, OtherType>& that)
			: Base(that)
		{
		}
	};

	template <typename Type>
	class TemporaryTuple<1, Type>
		: public Tuple<1, Type>
	{
	private:
		typedef Tuple<1, Type> Base;

	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		TemporaryTuple()
			: Base()
		{
		}

		explicit TemporaryTuple(
			const Dimension& dimension,
			const Type& that = Type())
			: Base(dimension, that)
		{
		}

		explicit TemporaryTuple(const Type& that)
			: Base(that)
		{
		}

		TemporaryTuple(const TemporaryTuple& that)
			: Base(that)
		{
		}

		template <typename OtherType>
		TemporaryTuple(const TemporaryTuple<1, OtherType>& that)
			: Base(that)
		{
		}
	};

	template <typename Type>
	class TemporaryTuple<2, Type>
		: public Tuple<2, Type>
	{
	private:
		typedef Tuple<2, Type> Base;

	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		TemporaryTuple()
			: Base()
		{
		}

		explicit TemporaryTuple(
			const Dimension& dimension,
			const Type& that = Type())
			: Base(dimension, that)
		{
		}

		explicit TemporaryTuple(const Type& that)
			: Base(that)
		{
		}

		TemporaryTuple(const TemporaryTuple& that)
			: Base(that)
		{
		}

		template <typename OtherType>
		TemporaryTuple(const TemporaryTuple<2, OtherType>& that)
			: Base(that)
		{
		}

		explicit TemporaryTuple(const Type& a, const Type& b)
			: Base(a, b)
		{
		}
	};

	template <typename Type>
	class TemporaryTuple<3, Type>
		: public Tuple<3, Type>
	{
	private:
		typedef Tuple<3, Type> Base;

	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		TemporaryTuple()
			: Base()
		{
		}

		explicit TemporaryTuple(
			const Dimension& dimension,
			const Type& that = Type())
			: Base(dimension, that)
		{
		}

		explicit TemporaryTuple(const Type& that)
			: Base(that)
		{
		}

		TemporaryTuple(const TemporaryTuple& that)
			: Base(that)
		{
		}

		template <typename OtherType>
		TemporaryTuple(const TemporaryTuple<3, OtherType>& that)
			: Base(that)
		{
		}

		explicit TemporaryTuple(const Type& a, const Type& b,
			const Type& c)
			: Base(a, b, c)
		{
		}
	};

	template <typename Type>
	class TemporaryTuple<4, Type>
		: public Tuple<4, Type>
	{
	private:
		typedef Tuple<4, Type> Base;

	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		TemporaryTuple()
			: Base()
		{
		}

		explicit TemporaryTuple(
			const Dimension& dimension,
			const Type& that = Type())
			: Base(dimension, that)
		{
		}

		explicit TemporaryTuple(const Type& that)
			: Base(that)
		{
		}

		TemporaryTuple(const TemporaryTuple& that)
			: Base(that)
		{
		}

		template <typename OtherType>
		TemporaryTuple(const TemporaryTuple<4, OtherType>& that)
			: Base(that)
		{
		}

		explicit TemporaryTuple(const Type& a, const Type& b,
			const Type& c, const Type& d)
			: Base(a, b, c, d)
		{
		}
	};

	template <typename Type>
	class TemporaryTuple<Dynamic, Type>
		: public Tuple<Dynamic, Type>
	{
	private:
		enum
		{
			N = Dynamic
		};

		typedef Tuple<N, Type> Base;

	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		using Base::swap;

		TemporaryTuple()
			: Base()
		{
		}

		explicit TemporaryTuple(
			const Dimension& dimension,
			const Type& that = Type())
			: Base(dimension, that)
		{
		}

		// This constructor does not make
		// sense for unbounded vectors and thus
		// we leave it out.
		/*
		explicit TemporaryTuple(const Type& that)
			: Base(that)
		{
		}
		*/

		TemporaryTuple(const TemporaryTuple& that)
			: Base(that)
		{
		}

		template <typename OtherType>
		TemporaryTuple(const TemporaryTuple<N, OtherType>& that)
			: Base(that)
		{
		}

		TemporaryTuple(
			const Dimension& dimension,
			Type* dataAlias)
			: Base(dimension, dataAlias)
		{
		}

		TemporaryTuple<N, Type>& operator=(
			const TemporaryTuple& that)
		{
			TemporaryTuple copy(that);
			swap(copy);
			return *this;
		}
	};

}

#include "pastel/sys/tuple.hpp"

#endif
