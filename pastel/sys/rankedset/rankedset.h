// Description: Ranked set

#ifndef PASTELSYS_RANKEDSET_H
#define PASTELSYS_RANKEDSET_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"

#include <vector>
#include <algorithm>

namespace Pastel
{

	//! A ranked set
	/*!
	Maintains the k smallest elements stored in the set;
	a bounded priority queue. The k is called the capacity 
	of the set.
	*/
	template <typename Type, typename Less = LessThan>
	class RankedSet
	{
	public:
		using DataSet = std::vector<Type>;
		using Iterator = typename DataSet::iterator;
		using ConstIterator = typename DataSet::const_iterator;
		using iterator = Iterator;
		using const_iterator = ConstIterator;

		//! Constructs a set.
		RankedSet(
			integer capacity = 0,
			Less less = Less())
			: dataSet_()
			, capacity_(0)
			, less_(std::move(less))
		{
			ENSURE_OP(capacity, >= , 0);
			reserve(capacity);
		}

		//! Inserts an element into the set.
		Iterator push(const Type& that)
		{
			if (capacity() == 0)
			{
				return end();
			}

			if (size() == capacity())
			{
				if (!less_(that, top()))
				{
					return end();
				}

				pop();
			}
			
			dataSet_.push_back(that);
			std::push_heap(begin(), end(), less_);
			return std::prev(end());
		}

		//! Releases the underlying data-set.
		/*!
		Post-conditions:
		capacity() == 0
		size() == 0

		sorted (bool):
		Whether to sort the data-set in 
		increasing order using heap-sort.

		returns:
		The underlying data-set moved out
		of the ranked-set.
		*/
		DataSet release(bool sorted = true)
		{
			if (sorted)
			{
				for (auto i = end(); i != begin();--i)
				{
					std::pop_heap(begin(), i, less_);
				}
			}

			RankedSet self(0);
			swap(self);

			return std::move(self.dataSet_);
		}

		//! Removes the maximum element.
		void pop()
		{
			ENSURE(!empty());
			std::pop_heap(begin(), end(), less_);
			dataSet_.pop_back();
		}

		//! Removes all elements.
		void clear()
		{
			dataSet_.clear();
			// The implementation of std::vector 
			// may or may not change the capacity 
			// of the vector. Restore capacity to 
			// be sure.
			dataSet_.reserve(capacity_);
		}

		//! Returns an iterator to the first element.
		PASTEL_ITERATOR_FUNCTIONS(begin, dataSet_.begin());

		//! Returns an iterator to the one-past-end element.
		PASTEL_ITERATOR_FUNCTIONS(end, dataSet_.end());

		//! Returns whether the set is full.
		bool full() const
		{
			return size() == capacity();
		}

		//! Returns whether the set is empty.
		bool empty() const
		{
			return dataSet_.empty();
		}

		//! Swaps two sets.
		void swap(RankedSet& that)
		{
			using std::swap;

			dataSet_.swap(that.dataSet_);
			swap(less_, that.less_);
			swap(capacity_, that.capacity_);
		}

		//! Returns the maximum element.
		Type& top()
		{
			ENSURE(!empty());
			return dataSet_.front();
		}

		//! Returns the maximum element.
		const Type& top() const
		{
			ENSURE(!empty());
			return dataSet_.front();
		}

		//! Returns the size of the set.
		integer size() const
		{
			return dataSet_.size();
		}

		//! Sets the capacity of the set.
		/*!
		Preconditions:
		capacity >= 0
		*/
		void reserve(integer capacity)
		{
			ENSURE_OP(capacity, >=, 0);

			while(size() > capacity)
			{
				pop();
			}

			dataSet_.reserve(capacity);
			capacity_ = capacity;
		}

		//! Returns the capacity of the set.
		integer capacity() const
		{
			return capacity_;
		}

	private:
		DataSet dataSet_;
		Less less_;
		// Note that std::vector's capacity
		// may be greater than the capacity
		// stored here.
		integer capacity_;
	};

}

#endif
