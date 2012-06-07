#ifndef PASTEL_ITERATOR_RANGE_HPP
#define PASTEL_ITERATOR_RANGE_HPP

#include "pastel/sys/iterator_range.h"

#include <boost/iterator/iterator_traits.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/if.hpp>

#include <iterator>

namespace Pastel
{

	template <typename Iterator>
	class IteratorToRange
	{
	public:
		typedef typename std::iterator_traits<Iterator>::iterator_category
			Category;

		typedef boost::mpl::if_<
			std::is_same<Category, std::bidirectional_iterator_tag>,
			BidirectionalIterator_Range<Iterator>,
			ForwardIterator_Range<Iterator> > ElseIf;

		typedef boost::mpl::if_<
			std::is_same<Category, std::random_access_iterator_tag>,
			RandomAccessIterator_Range<Iterator>,
			typename ElseIf::type> If;

		typedef typename If::type type;			
	};

	template <typename Iterator>
	typename IteratorToRange<Iterator>::type 
		range(const Iterator& begin, const Iterator& end)
	{
		typedef typename IteratorToRange<Iterator>::type Range;
		return Range(begin, end);
	}

	template <typename Iterator>
	typename IteratorToRange<Iterator>::type 
		range(const Iterator& begin, integer size)
	{
		PENSURE_OP(size, >=, 0);

		typedef typename IteratorToRange<Iterator>::type Range;
		return Range(begin, size);
	}

	template <typename Iterator>
	typename IteratorToRange<Iterator>::type 
		range(const Iterator& begin, const Iterator& end,
		integer size)
	{
		//PENSURE_OP(std::distance(begin, end), >=, 0);

		typedef typename IteratorToRange<Iterator>::type Range;
		return Range(begin, end, size);
	}

	template <typename Type, int N>
	typename IteratorToRange<Type*>::type 
		range(Type (&begin)[N])
	{
		typedef typename IteratorToRange<Type*>::type Range;
		return Range(begin, begin + N, N);
	}

}

#endif
