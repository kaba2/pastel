// Description: Iterator ranges

#ifndef PASTEL_ITERATOR_RANGE_H
#define PASTEL_ITERATOR_RANGE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/forwarditerator_range.h"
#include "pastel/sys/bidirectionaliterator_range.h"
#include "pastel/sys/randomaccessiterator_range.h"

namespace Pastel
{

	//! Returns the type of the iterator range.
	template <typename Iterator>
	class IteratorToRange;

	//! Returns an iterator range.
	template <typename Iterator>
	typename IteratorToRange<Iterator>::type 
		range(const Iterator& begin, const Iterator& end);

	//! Returns an iterator range.
	/*!
	Preconditions:
	size >= 0
	*/
	template <typename Iterator>
	typename IteratorToRange<Iterator>::type 
		range(const Iterator& begin, integer size);

	//! Returns an iterator range.
	/*!
	Preconditions:
	std::distance(begin, end) == size
	*/
	template <typename Iterator>
	typename IteratorToRange<Iterator>::type 
		range(const Iterator& begin, const Iterator& end,
		integer size);

	//! Returns an iterator range.
	template <typename Type, int N>
	typename IteratorToRange<Type*>::type 
		range(Type (&begin)[N]);

}

#include "pastel/sys/iterator_range.hpp"

#endif
