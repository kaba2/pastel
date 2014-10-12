// Description: A 'second'-member pair-iterator facade

#ifndef PASTELSYS_SECOND_ITERATOR_H
#define PASTELSYS_SECOND_ITERATOR_H

#include <boost/operators.hpp>

#include <type_traits>

namespace Pastel
{

	namespace Second_Iterator_
	{

		template <typename That>
		class Pair_Second_Type
		{
		};

		template <typename First, typename Second>
		class Pair_Second_Type<std::pair<First, Second>>
		{
		public:
			using type = Second;
		};

	}

	template <typename Iterator, bool Const>
	class Second_Iterator
		: boost::less_than_comparable<Second_Iterator<Iterator, Const>
		, boost::unit_steppable<Second_Iterator<Iterator, Const>
		, boost::addable2<Second_Iterator<Iterator, Const>, typename std::iterator_traits<Iterator>::difference_type
		, boost::subtractable<Second_Iterator<Iterator, Const>, typename std::iterator_traits<Iterator>::difference_type
		> > > >
	{
	private:
		class Disabler {};

	public:
		template <typename, bool> friend class Second_Iterator;

		using Pair_Type = typename std::iterator_traits<Iterator>::value_type;
		using value_type = typename Second_Iterator_::Pair_Second_Type<Pair_Type>::type;

		typedef typename std::add_lvalue_reference<
			typename std::conditional<
			Const,
			typename std::add_const<value_type>::type,
			value_type>::type
		>::type reference;

		typedef typename std::add_pointer<
			typename std::conditional<
			Const,
			typename std::add_const<value_type>::type,
			value_type>::type
		>::type pointer;

		using difference_type = typename std::iterator_traits<Iterator>::difference_type;
		using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;

		Second_Iterator()
			: iter_()
		{
		}

		template <typename That_Iterator, bool That_Const>
		Second_Iterator(
			const Second_Iterator<That_Iterator, That_Const>& that,
			DisableIfC<(!Const && That_Const), Disabler> disabler = Disabler())
			: iter_(that.iter_)
		{
		}

		template <typename That_Iterator>
		Second_Iterator(const That_Iterator& that)
			: iter_(that)
		{
		}
		
		Second_Iterator& operator++()
		{
			++iter_;
			return *this;
		}

		Second_Iterator& operator--()
		{
			--iter_;
			return *this;
		}

		template <typename That_Iterator, bool That_Const>
		bool operator==(const Second_Iterator<That_Iterator, That_Const>& that) const
		{
			return iter_ == that.iter_;
		}

		template <typename That_Iterator, bool That_Const>
		bool operator!=(const Second_Iterator<That_Iterator, That_Const>& that) const
		{
			return iter_ != that.iter_;
		}

		template <typename That_Iterator, bool That_Const>
		bool operator<(const Second_Iterator<That_Iterator, That_Const>& that) const
		{
			return iter_ < that.iter_;
		}

		Second_Iterator& operator+=(difference_type n)
		{
			iter_ += n;
		}

		Second_Iterator& operator-=(difference_type n)
		{
			iter_ -= n;
		}

		reference operator*() const
		{
			return iter_->second;
		}

		pointer operator->() const
		{
			return &iter_->second;
		}

		Iterator base() const
		{
			return iter_;
		}

	private:
		Iterator iter_;
	};

}

#endif
