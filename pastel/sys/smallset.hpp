#ifndef PASTEL_SMALLSET_HPP
#define PASTEL_SMALLSET_HPP

#include "pastel/sys/smallset.h"

#include "pastel/sys/ensure.h"

#include <algorithm>

namespace Pastel
{

	template <typename Type, typename Compare>
	SmallSet<Type, Compare>::SmallSet(const Compare& compare)
		: data_()
		, compare_(compare)
	{
	}

	template <typename Type, typename Compare>
	SmallSet<Type, Compare>::SmallSet(const SmallSet& that)
		: data_(that.data_)
		, compare_(that.compare_)
	{
	}

	template <typename Type, typename Compare>
	SmallSet<Type, Compare>::~SmallSet()
	{
	}

	template <typename Type, typename Compare>
	SmallSet<Type, Compare>& SmallSet<Type, Compare>::operator=(
		const SmallSet& that)
	{
		SmallSet<Type, Compare> copy(that);
		swap(copy);
		return *this;
	}

	template <typename Type, typename Compare>
	void SmallSet<Type, Compare>::swap(SmallSet& that)
	{
		using std::swap;

		data_.swap(that.data_);
		swap(compare_, that.compare_);
	}

	template <typename Type, typename Compare>
	void SmallSet<Type, Compare>::clear()
	{
		data_.clear();
	}

	template <typename Type, typename Compare>
	void SmallSet<Type, Compare>::reserve(integer count)
	{
		ENSURE_OP(count, >=, 0);

		data_.reserve(count);
	}

	template <typename Type, typename Compare>
	integer SmallSet<Type, Compare>::insert(const Type& that)
	{
		const integer index = lower_bound(that);

		if (index == data_.size() ||
			compare_(that, data_[index]))
		{
			data_.insert(
				data_.begin() + index, that);
		}

		ASSERT(index == 0 || compare_(data_[index - 1], that));
		ASSERT(index == data_.size() - 1 || compare_(that, data_[index + 1]));

		return index;
	}

	template <typename Type, typename Compare>
	void SmallSet<Type, Compare>::erase(const Type& that)
	{
		const integer index = find(that);
		if (index == data_.size())
		{
			return;
		}
		ASSERT(equivalent(data_[index], that, compare_));

		data_.erase(data_.begin() + index);
	}

	template <typename Type, typename Compare>
	void SmallSet<Type, Compare>::eraseFrom(integer index)
	{
		PENSURE2(index >= 0 && index < size(), index, size());

		data_.erase(data_.begin() + index);
	}

	template <typename Type, typename Compare>
	void SmallSet<Type, Compare>::pop_back()
	{
		ENSURE(!empty());

		data_.pop_back();
	}

	template <typename Type, typename Compare>
	integer SmallSet<Type, Compare>::find(const Type& that) const
	{
		const integer lowerBound = lower_bound(that);

		if (lowerBound == data_.size() ||
			equivalent(that, data_[lowerBound], compare_))
		{
			return lowerBound;
		}
		
		return data_.size();
	}

	template <typename Type, typename Compare>
	integer SmallSet<Type, Compare>::lower_bound(const Type& that) const
	{
		if (data_.empty())
		{
			return 0;
		}

		if (compare_(data_.back(), that))
		{
			return data_.size();
		}

		/*
		if (!compare_(data_.front(), that))
		{
			return 0;
		}
		*/

		integer lowerBound = 0;
		integer upperBound = data_.size() - 1;
		while (lowerBound < upperBound)
		{
			const integer middle = (lowerBound + upperBound) / 2;

			if (compare_(data_[middle], that))
			{
				lowerBound = middle + 1;
			}
			else
			{
				if (!compare_(that, data_[middle]))
				{
					// Equivalent object at middle.
					return middle;
				}

				upperBound = middle;
			}

			ASSERT(lowerBound <= upperBound);
		}

		ASSERT(lowerBound == 0 || compare_(data_[lowerBound - 1], that));
		ASSERT(!compare_(data_[lowerBound], that));

		return lowerBound;
	}

	template <typename Type, typename Compare>
	integer SmallSet<Type, Compare>::upper_bound(const Type& that) const
	{
		const integer lowerBound = lower_bound(that);
		if (lowerBound < data_.size())
		{
			return lowerBound + 1;
		}

		return lowerBound;
	}

	template <typename Type, typename Compare>
	Type& SmallSet<Type, Compare>::operator[](integer index)
	{
		PENSURE2(index >= 0 && index < data_.size(),
			index, data_.size());

		return data_[index];
	}

	template <typename Type, typename Compare>
	const Type& SmallSet<Type, Compare>::operator[](integer index) const
	{
		PENSURE2(index >= 0 && index < data_.size(),
			index, data_.size());

		return data_[index];
	}

	template <typename Type, typename Compare>
	const Type& SmallSet<Type, Compare>::front() const
	{
		PENSURE(!empty());

		return data_.front();
	}

	template <typename Type, typename Compare>
	const Type& SmallSet<Type, Compare>::back() const
	{
		PENSURE(!empty());

		return data_.back();
	}

	template <typename Type, typename Compare>
	bool SmallSet<Type, Compare>::empty() const
	{
		return data_.empty();
	}

	template <typename Type, typename Compare>
	integer SmallSet<Type, Compare>::size() const
	{
		return data_.size();
	}

	template <typename Type, typename Compare>
	integer SmallSet<Type, Compare>::capacity() const
	{
		return data_.capacity();
	}

	template <typename Type, typename Compare>
	void swap(SmallSet<Type, Compare>& left,
		SmallSet<Type, Compare>& right)
	{
		left.swap(right);
	}

}

#endif
