#ifndef PASTELSYS_SMALLSET_HPP
#define PASTELSYS_SMALLSET_HPP

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
		using NameLookUpTrick::swap;

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
		ENSURE1(count >= 0, count);

		data_.reserve(count);
	}

	template <typename Type, typename Compare>
	integer SmallSet<Type, Compare>::set(integer index, const Type& that)
	{
		PENSURE2(index >= 0 && index < size(), index, size());

		// Assign.
		data_[index] = that;

		const integer elements = size();

		if (elements == 1)
		{
			return 0;
		}

		// Move to the right position
		// by repeated swaps.

		if (index > 0 && !compare_(data_[index - 1], data_[index]))
		{
			integer left = index - 1;
			integer right = index;
			while(left >= 0 && !compare_(data_[left], data_[right]))
			{
				using NameLookUpTrick::swap;
				std::swap(data_[left], data_[right]);

				if (!compare_(data_[left], data_[right]))
				{
					// Equivalent elements.
					break;
				}

				--left;
				--right;
			}

			if (!(compare_(data_[left], data_[right]) ||
				compare_(data_[right], data_[left])))
			{
				// Equivalent elements not allowed.
				data_.erase(data_.begin() + left);
				return -1;
			}

			return left + 1;
		}
		else if (index < elements - 1 && !compare_(data_[index], data_[index + 1]))
		{
			integer left = index;
			integer right = index + 1;
			while(right < elements && !compare_(data_[left], data_[right]))
			{
				using NameLookUpTrick::swap;
				std::swap(data_[left], data_[right]);

				if (!compare_(data_[left], data_[right]))
				{
					// Equivalent elements.
					break;
				}

				++left;
				++right;
			}

			if (!(compare_(data_[left], data_[right]) ||
				compare_(data_[right], data_[left])))
			{
				// Equivalent elements not allowed.
				data_.erase(data_.begin() + right);
				return -1;
			}

			return right - 1;
		}

		return index;
	}

	template <typename Type, typename Compare>
	integer SmallSet<Type, Compare>::insert(const Type& that)
	{
		integer index = -1;

		if (empty())
		{
			// If the set is empty, just
			// insert 'that' in the list.

			data_.push_back(that);
			index = 0;
		}
		else if (compare_(that, back()))
		{
			// If 'that' will not be the largest
			// element, find a place to insert it.

			index = lower_bound(that);

			// Since the set is not empty,
			// the lower bound must be found.
			ASSERT2(index >= 0 && index <= data_.size(),
				index, data_.size());

			if (index < data_.size())
			{
				if (!(compare_(that, data_[index]) || compare_(data_[index], that)))
				{
					// Equivalent objects are not allowed
					// in a set.
					return -1;
				}
			}

			data_.insert(data_.begin() + index, that);
		}
		else if (compare_(back(), that))
		{
			// If 'that' will be the largest element,
			// just append it at the back of the list,
			// making this a constant time operation.

			index = data_.size();
			data_.push_back(that);
		}

		// The default value for 'index' handles the
		// following (thus the commenting):

		//else
		//{
			// 'that' is equivalent to back().
			// Equivalent objects are not allowed
			// in a set.
			//index = -1;
		//}

		return index;
	}

	template <typename Type, typename Compare>
	void SmallSet<Type, Compare>::erase(const Type& that)
	{
		const integer index = find(that);
		if (index < 0)
		{
			return;
		}

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
		if (empty())
		{
			return -1;
		}

		integer lowerBound = 0;
		integer upperBound = data_.size() - 1;
		while (lowerBound <= upperBound)
		{
			const integer middle = (lowerBound + upperBound) / 2;
			if (compare_(that, data_[middle]))
			{
				upperBound = middle - 1;
			}
			else if (compare_(data_[middle], that))
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

	template <typename Type, typename Compare>
	integer SmallSet<Type, Compare>::lower_bound(const Type& that) const
	{
		if (empty())
		{
			return -1;
		}

		integer lowerBound = 0;
		integer upperBound = data_.size() - 1;
		while (lowerBound <= upperBound)
		{
			const integer middle = (lowerBound + upperBound) / 2;
			if (compare_(that, data_[middle]))
			{
				upperBound = middle - 1;
			}
			else if (compare_(data_[middle], that))
			{
				lowerBound = middle + 1;
			}
			else
			{
				return middle;
			}
		}

		return lowerBound;
	}

	template <typename Type, typename Compare>
	integer SmallSet<Type, Compare>::upper_bound(const Type& that) const
	{
		if (empty())
		{
			return -1;
		}

		return lower_bound(that) + 1;
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
