// Description: Array of lists
// Documentation: undocumented.txt

#ifndef PASTELSYS_LISTARRAY_H
#define PASTELSYS_LISTARRAY_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/list.h"
#include "pastel/sys/allocator/pool_allocator.h"

namespace Pastel
{

	template <typename Type>
	class ListArray
	{
	private:
		using ListContainer = List<Type, PoolAllocator>;

	public:
		typedef typename ListContainer::iterator
			Iterator;
		typedef typename ListContainer::const_iterator
			ConstIterator;

		// Using default destructor.

		ListArray();
		ListArray(const ListArray& that);
		ListArray<Type>& operator=(const ListArray& that);

		void swap(ListArray& that);

		void setBuckets(integer buckets);

		void push_back(integer bucket, const Type& that);
		void pop_back(integer bucket);

		Iterator begin();
		ConstIterator begin() const;

		Iterator end();
		ConstIterator end() const;

		Iterator begin(integer bucket);
		ConstIterator begin(integer bucket) const;

		Iterator end(integer bucket);
		ConstIterator end(integer bucket) const;

		integer bucketCount() const;

		integer size() const;

	private:
		struct Range
		{
			Range()
				: first_()
				, last_()
			{
			}

			Range(const Iterator& first,
				const Iterator& last)
				: first_(first)
				, last_(last)
			{
			}

			Iterator first_;
			Iterator last_;
		};

		using RangeArray = std::vector<Range>;

		RangeArray range_;
		ListContainer data_;
	};

}

#include "pastel/sys/listarray.hpp"

#endif
