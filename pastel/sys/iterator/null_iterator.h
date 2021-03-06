// Description: NullIterator class
// Detail: An iterator for sending data into oblivion.
// Documentation: iterator.txt

#ifndef PASTELSYS_NULL_ITERATOR_H
#define PASTELSYS_NULL_ITERATOR_H

#include "pastel/sys/mytypes.h"

#include <boost/operators.hpp>

#include <iterator>

namespace Pastel
{

	namespace NullIterator_
	{

		class NullObject
		{
		public:
			template <typename Type>
			void operator=(const Type&)
			{
			}
		};

		template <typename Derived, typename ValueType>
		class NullIteratorBase
			: public boost::random_access_iterator_helper<Derived, ValueType, integer>
		{
		private:
			typedef boost::random_access_iterator_helper<Derived, ValueType, integer>
				Base;

		public:
			// Using default copy constructor.
			// Using default assignment.
			// Using default destructor.

			NullIteratorBase()
				: data_()
			{
			}

			Derived& operator++()
			{
				return (Derived&)*this;
			}
			
			Derived& operator--()
			{
				return (Derived&)*this;
			}

			Derived& operator+=(
				integer)
			{
				return (Derived&)*this;
			}

			Derived& operator-=(
				integer)
			{
				return (Derived&)*this;
			}

			friend typename Base::difference_type operator-(
				const Derived&, const Derived&)
			{
				return 0;
			}

			typename Base::reference operator*() const
			{
				return (typename Base::reference)data_;
			}

			friend bool operator==(
				const Derived&, const Derived&)
			{
				return true;
			}

			friend bool operator<(
				const Derived&, const Derived&)
			{
				return false;
			}

		protected:
			ValueType data_;
		};

	}

	class NullIterator
		: public NullIterator_::NullIteratorBase<NullIterator, NullIterator_::NullObject>
	{
	};

	class ConstNullIterator
		: public NullIterator_::NullIteratorBase<ConstNullIterator, const NullIterator_::NullObject>
	{
	public:
		ConstNullIterator()
		{
		}

		ConstNullIterator(
			const NullIterator& that)
		{
		}
	};

}

#endif
