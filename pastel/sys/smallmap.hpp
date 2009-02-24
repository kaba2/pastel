#ifndef PASTELSYS_SMALLMAP_HPP
#define PASTELSYS_SMALLMAP_HPP

#include "pastel/sys/smallmap.h"

#include "pastel/sys/ensure.h"

#include <algorithm>

namespace Pastel
{

	template <typename KeyType, typename ValueType, typename Compare>
	SmallMap<KeyType, ValueType, Compare>::SmallMap(
		const Compare& compare)
		: data_()
		, compare_(compare)
	{
	}

	template <typename KeyType, typename ValueType, typename Compare>
	SmallMap<KeyType, ValueType, Compare>::SmallMap(
		const SmallMap& that)
		: data_(that.data_)
		, compare_(that.compare_)
	{
	}

	template <typename KeyType, typename ValueType, typename Compare>
	SmallMap<KeyType, ValueType, Compare>::SmallMap(
		const SmallMap& that,
		const Compare& compare)
		: data_(that.data_)
		, compare_(compare)
	{
	}

	template <typename KeyType, typename ValueType, typename Compare>
	SmallMap<KeyType, ValueType, Compare>::~SmallMap()
	{
	}

	template <typename KeyType, typename ValueType, typename Compare>
	SmallMap<KeyType, ValueType, Compare>&
		SmallMap<KeyType, ValueType, Compare>::operator=(
		const SmallMap& that)
	{
		SmallMap copy(that);
		swap(copy);
		return *this;
	}

	template <typename KeyType, typename ValueType, typename Compare>
	void SmallMap<KeyType, ValueType, Compare>::swap(SmallMap& that)
	{
		using std::swap;

		data_.swap(that.data_);
		swap(compare_, that.compare_);
	}

	template <typename KeyType, typename ValueType, typename Compare>
	void SmallMap<KeyType, ValueType, Compare>::clear()
	{
		data_.clear();
	}

	template <typename KeyType, typename ValueType, typename Compare>
	void SmallMap<KeyType, ValueType, Compare>::reserve(integer count)
	{
		ENSURE1(count >= 0, count);

		data_.reserve(count);
	}

	template <typename KeyType, typename ValueType, typename Compare>
	integer SmallMap<KeyType, ValueType, Compare>::insert(
		const KeyType& key, const ValueType& value)
	{
		integer index = -1;

		if (empty())
		{
			// If the set is empty, just
			// insert the new element in the list.

			data_.push_back(Element(key, value));
			index = 0;
		}
		else if (compare_(key, keyBack()))
		{
			// If the new element is not the largest
			// element, find a place to insert it.

			index = lower_bound(key);

			// Since the set is not empty,
			// the lower bound must be found.
			ASSERT2(index >= 0 && index <= data_.size(),
				index, data_.size());

			if (index < data_.size())
			{
				if (!(compare_(key, data_[index].key_) || compare_(data_[index].key_, key)))
				{
					// Equivalent objects are not allowed
					// in a set.
					return -1;
				}
			}

			data_.insert(data_.begin() + index,
				Element(key, value));
		}
		else if (compare_(keyBack(), key))
		{
			// If 'that' will be the largest element,
			// just append it at the back of the list,
			// making this a constant time operation.

			index = data_.size();
			data_.push_back(Element(key, value));
		}

		// The default value for 'index' handles the
		// following (thus the commenting):

		// else
		// {
		//		// 'that' is equivalent to back().
		//		// Equivalent objects are not allowed.
		//		index = -1;
		// }

		return index;
	}

	template <typename KeyType, typename ValueType, typename Compare>
	void SmallMap<KeyType, ValueType, Compare>::erase(const KeyType& that)
	{
		integer index = find(that);
		if (index < 0)
		{
			return;
		}

		data_.erase(data_.begin() + index);
	}

	template <typename KeyType, typename ValueType, typename Compare>
	void SmallMap<KeyType, ValueType, Compare>::eraseFrom(integer index)
	{
		PENSURE2(index >= 0 && index < size(), index, size());

		data_.erase(data_.begin() + index);
	}

	template <typename KeyType, typename ValueType, typename Compare>
	void SmallMap<KeyType, ValueType, Compare>::pop_back()
	{
		ENSURE(!empty());

		data_.pop_back();
	}

	template <typename KeyType, typename ValueType, typename Compare>
	integer SmallMap<KeyType, ValueType, Compare>::find(
		const KeyType& that) const
	{
		if (empty())
		{
			return -1;
		}

		// Perform a binary search on
		// the already sorted data.

		integer lowerBound = 0;
		integer upperBound = data_.size() - 1;
		while (lowerBound <= upperBound)
		{
			integer middle = (lowerBound + upperBound) / 2;
			if (compare_(that, data_[middle].key_))
			{
				upperBound = middle - 1;
			}
			else if (compare_(data_[middle].key_, that))
			{
				lowerBound = middle + 1;
			}
			else
			{
				return middle;
			}
		}

		return -1;
	}

	template <typename KeyType, typename ValueType, typename Compare>
	integer SmallMap<KeyType, ValueType, Compare>::lower_bound(
		const KeyType& that) const
	{
		if (empty())
		{
			return -1;
		}

		if (compare_(data_.back().key_, that))
		{
			return data_.size();
		}

		if (compare_(that, data_.front().key_))
		{
			return 0;
		}

		integer lowerBound = 0;
		integer upperBound = data_.size();
		while (lowerBound + 1 < upperBound)
		{
			const integer middle = (lowerBound + upperBound) / 2;

			if (compare_(that, data_[middle].key_))
			{
				upperBound = middle;
			}
			else if (compare_(data_[middle].key_, that))
			{
				lowerBound = middle;
			}
			else
			{
				return middle;
			}
		}

		return lowerBound;
	}

	template <typename KeyType, typename ValueType, typename Compare>
	integer SmallMap<KeyType, ValueType, Compare>::upper_bound(
		const KeyType& that) const
	{
		if (empty())
		{
			return -1;
		}

		return lower_bound(that) + 1;
	}

	template <typename KeyType, typename ValueType, typename Compare>
	const KeyType& SmallMap<KeyType, ValueType, Compare>::keyAt(
		integer index) const
	{
		PENSURE2(index >= 0 && index < data_.size(),
			index, data_.size());

		return data_[index].key_;
	}

	template <typename KeyType, typename ValueType, typename Compare>
	ValueType& SmallMap<KeyType, ValueType, Compare>::valueAt(
		integer index)
	{
		PENSURE2(index >= 0 && index < data_.size(),
			index, data_.size());

		return data_[index].value_;
	}

	template <typename KeyType, typename ValueType, typename Compare>
	const ValueType& SmallMap<KeyType, ValueType, Compare>::valueAt(
		integer index) const
	{
		PENSURE2(index >= 0 && index < data_.size(),
			index, data_.size());

		return data_[index].value_;
	}

	template <typename KeyType, typename ValueType, typename Compare>
	const KeyType&
		SmallMap<KeyType, ValueType, Compare>::keyFront() const
	{
		PENSURE(!empty());

		return data_.front().key_;
	}

	template <typename KeyType, typename ValueType, typename Compare>
	const KeyType&
		SmallMap<KeyType, ValueType, Compare>::keyBack() const
	{
		PENSURE(!empty());

		return data_.back().key_;
	}

	template <typename KeyType, typename ValueType, typename Compare>
	ValueType& SmallMap<KeyType, ValueType, Compare>::valueFront()
	{
		PENSURE(!empty());

		return data_.front().value_;
	}

	template <typename KeyType, typename ValueType, typename Compare>
	const ValueType&
		SmallMap<KeyType, ValueType, Compare>::valueFront() const
	{
		PENSURE(!empty());

		return data_.front().value_;
	}

	template <typename KeyType, typename ValueType, typename Compare>
	ValueType& SmallMap<KeyType, ValueType, Compare>::valueBack()
	{
		PENSURE(!empty());

		return data_.back().value_;
	}

	template <typename KeyType, typename ValueType, typename Compare>
	const ValueType&
		SmallMap<KeyType, ValueType, Compare>::valueBack() const
	{
		PENSURE(!empty());

		return data_.back().value_;
	}

	template <typename KeyType, typename ValueType, typename Compare>
	bool SmallMap<KeyType, ValueType, Compare>::empty() const
	{
		return data_.empty();
	}

	template <typename KeyType, typename ValueType, typename Compare>
	integer SmallMap<KeyType, ValueType, Compare>::size() const
	{
		return data_.size();
	}

	template <typename KeyType, typename ValueType, typename Compare>
	integer SmallMap<KeyType, ValueType, Compare>::capacity() const
	{
		return data_.capacity();
	}

}

#endif
