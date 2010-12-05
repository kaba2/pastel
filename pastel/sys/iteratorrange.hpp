#ifndef PASTEL_ITERATORRANGE_HPP
#define PASTEL_ITERATORRANGE_HPP

#include "pastel/sys/iteratorrange.h"
#include "pastel/sys/forwardrange.h"
#include "pastel/sys/bidirectionalrange.h"
#include "pastel/sys/randomaccessrange.h"

#include <boost/iterator/iterator_traits.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/if.hpp>

#include <iterator>

namespace Pastel
{

	template <typename Iterator>
	class IteratorRange
	{
	public:
		typedef typename boost::iterator_category<Iterator>::type 
			Category;

		typedef boost::mpl::if_<
			boost::is_same<Category, std::bidirectional_iterator_tag>,
			BidirectionalRange<Iterator>,
			ForwardRange<Iterator> > ElseIf;

		typedef boost::mpl::if_<
			boost::is_same<Category, std::random_access_iterator_tag>,
			RandomAccessRange<Iterator>,
			typename ElseIf::type> If;

		typedef typename If::type type;			
	};

	template <typename Iterator>
	typename IteratorRange<Iterator>::type 
		range(const Iterator& begin, const Iterator& end)
	{
		typedef typename IteratorRange<Iterator>::type Range;
		return Range(begin, end);
	}

	template <typename Iterator>
	typename IteratorRange<Iterator>::type 
		range(const Iterator& begin, integer size)
	{
		PENSURE_OP(size, >=, 0);

		typedef typename IteratorRange<Iterator>::type Range;
		return Range(begin, size);
	}

	template <typename Iterator>
	typename IteratorRange<Iterator>::type 
		range(const Iterator& begin, const Iterator& end,
		integer size)
	{
		//PENSURE_OP(std::distance(begin, end), >=, 0);

		typedef typename IteratorRange<Iterator>::type Range;
		return Range(begin, end, size);
	}

	template <typename Type, int N>
	typename IteratorRange<Type*>::type 
		range(Type (&begin)[N])
	{
		typedef typename IteratorRange<Type*>::type Range;
		return Range(begin, N);
	}

}

#endif
