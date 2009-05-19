#ifndef PASTEL_LISTARRAY_HPP
#define PASTEL_LISTARRAY_HPP

#include "pastel/sys/listarray.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Type>
	ListArray<Type>::ListArray()
		: range_()
		, data_()
	{
	}

	template <typename Type>
	ListArray<Type>::ListArray(
		const ListArray<Type>& that)
		: range_()
		, data_()
	{
		// TODO
		ENSURE(false);
	}

	template <typename Type>
	ListArray<Type>& ListArray<Type>::operator=(
		const ListArray& that)
	{
		ListArray<Type> copy(*this);
		swap(copy);
		return *this;
	}

	template <typename Type>
	void ListArray<Type>::swap(ListArray& that)
	{
		range_.swap(that.range_);
		data_.swap(that.data_);
	}

	template <typename Type>
	void ListArray<Type>::setBuckets(integer buckets)
	{
		range_.resize(buckets,
			Range(data_.end(), data_.end()));
	}

	template <typename Type>
	void ListArray<Type>::push_back(
		integer bucket, const Type& that)
	{
		PENSURE2(bucket >= 0 && bucket < range_.size(),
			bucket, range_.size());

		Range& range = range_[bucket];

		if (range.first_ == data_.end())
		{
			// This bucket is empty.
			data_.push_back(that);
			range.first_ = data_.end();
			--range.first_;
			range.last_ = range.first_;
		}
		else
		{
			Iterator rangeEnd(range.last_);
			++rangeEnd;

			range.last_ = data_.insert(rangeEnd, that);
		}
	}

	template <typename Type>
	void ListArray<Type>::pop_back(
		integer bucket)
	{
		PENSURE2(bucket >= 0 && bucket < range_.size(),
			bucket, range_.size());

		Range& range = range_[bucket];

		PENSURE(range.first_ != data_.end());

		if (range.first_ == range.last_)
		{
			// This is the last element in the bucket.

			data_.erase(range.last_);
			range.first_ = data_.end();
			range.last_ = range.first_;
		}
		else
		{
			Iterator iter(range.last_);
			--range.last_;
			data_.erase(iter);
		}
	}

	template <typename Type>
	integer ListArray<Type>::bucketCount() const
	{
		return range_.size();
	}

	template <typename Type>
	integer ListArray<Type>::size() const
	{
		return data_.size();
	}

	template <typename Type>
	typename ListArray<Type>::Iterator
		ListArray<Type>::begin()
	{
		return data_.begin();
	}

	template <typename Type>
	typename ListArray<Type>::ConstIterator
		ListArray<Type>::begin() const
	{
		return data_.begin();
	}

	template <typename Type>
	typename ListArray<Type>::Iterator
		ListArray<Type>::end()
	{
		return data_.end();
	}

	template <typename Type>
	typename ListArray<Type>::ConstIterator
		ListArray<Type>::end() const
	{
		return data_.end();
	}

	template <typename Type>
	typename ListArray<Type>::Iterator
		ListArray<Type>::begin(integer bucket)
	{
		PENSURE2(bucket >= 0 && bucket < range_.size(),
			bucket, range_.size());

		return range_[bucket].first_;
	}

	template <typename Type>
	typename ListArray<Type>::ConstIterator
		ListArray<Type>::begin(integer bucket) const
	{
		PENSURE2(bucket >= 0 && bucket < range_.size(),
			bucket, range_.size());

		return range_[bucket].first_;
	}

	template <typename Type>
	typename ListArray<Type>::Iterator
		ListArray<Type>::end(integer bucket)
	{
		PENSURE2(bucket >= 0 && bucket < range_.size(),
			bucket, range_.size());

		Iterator result(range_[bucket].last_);

		if (result == data_.end())
		{
			return result;
		}

		++result;
		return result;
	}

	template <typename Type>
	typename ListArray<Type>::ConstIterator
		ListArray<Type>::end(integer bucket) const
	{
		PENSURE2(bucket >= 0 && bucket < range_.size(),
			bucket, range_.size());

		ConstIterator result(range_[bucket].last_);

		if (result == data_.end())
		{
			return result;
		}

		++result;
		return result;
	}

}

#endif
