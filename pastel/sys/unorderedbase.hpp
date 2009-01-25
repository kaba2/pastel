#ifndef PASTELSYS_UNORDEREDBASE_HPP
#define PASTELSYS_UNORDEREDBASE_HPP

#include "pastel/sys/unorderedbase.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/primes.h"

#include <iterator>
#include <limits>

namespace Pastel
{

	namespace Detail
	{

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			UnorderedBase(
			const allocator_ref& allocatorRef)
			: extractKey_()
			, hasher_()
			, keyEqual_()
			, size_(0)
			, values_(allocatorRef)
			, buckets_()
			, maxLoadFactor_(1)
		{
			buckets_.resize(followingPrime(1),
				Bucket(values_.end(), values_.end()));
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			UnorderedBase(
			size_type bucketCount,
			const hasher& pHasher,
			const key_equal& keyEqual,
			const allocator_ref& allocatorRef)
			: extractKey_()
			, hasher_(pHasher)
			, keyEqual_(keyEqual)
			, size_(0)
			, values_(allocatorRef)
			, buckets_()
			, maxLoadFactor_(1)
		{
			ENSURE1(bucketCount > 0, bucketCount);

			buckets_.resize(followingPrime(bucketCount),
				Bucket(values_.end(), values_.end()));
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			template <class InputIterator>
		UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			UnorderedBase(
			InputIterator from,
			InputIterator to,
			size_type bucketCount,
			const hasher& pHasher,
			const key_equal& keyEqual,
			const allocator_ref& allocatorRef)
			: extractKey_()
			, hasher_(pHasher)
			, keyEqual_(keyEqual)
			, size_(0)
			, values_(allocatorRef)
			, buckets_()
			, maxLoadFactor_(1)
		{
			ENSURE1(bucketCount > 0, bucketCount);

			buckets_.resize(followingPrime(bucketCount),
				Bucket(values_.end(), values_.end()));

			insert(from, to);
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			UnorderedBase(
			const UnorderedBase& that,
			const allocator_ref& allocatorRef)
			: extractKey_(that.extractKey_)
			, hasher_(that.hasher_)
			, keyEqual_(that.keyEqual_)
			, size_(0)
			, values_(allocatorRef)
			, buckets_()
			, maxLoadFactor_(that.maxLoadFactor_)
		{
			buckets_.resize(followingPrime(1),
				Bucket(values_.end(), values_.end()));

			insert(that.begin(), that.end());
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			~UnorderedBase()
		{
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>&
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			operator=(
			const UnorderedBase& that)
		{
			UnorderedBase copy(that, get_allocator());
			swap(copy);
			return *this;
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			void
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			set_allocator(const allocator_ref& allocatorRef)
		{
			UnorderedBase copy(*this, allocatorRef);
			swap(copy);
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			allocator_ref
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			get_allocator() const
		{
			return values_.get_allocator();
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			bool UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			empty() const
		{
			return size_ == 0;
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			size_type
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			size() const
		{
			return size_;
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			size_type
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			max_size() const
		{
			return std::numeric_limits<size_type>::max();
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			iterator
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::begin()
		{
			return values_.begin();
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			const_iterator
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			begin() const
		{
			return values_.begin();
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			iterator
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::end()
		{
			return values_.end();
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			const_iterator
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			end() const
		{
			return values_.end();
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			std::pair<
			typename
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			iterator,
			bool>
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			insert(const value_type& obj)
		{
			ASSERT1(buckets_.size() > 0, buckets_.size());

			const size_type bucketIndex = bucket(extractKey_(obj));
			ASSERT(bucketIndex >= 0 && bucketIndex < buckets_.size());
			Bucket& target(buckets_.at(bucketIndex));

			if (isEmptyBucket(bucketIndex))
			{
				target.first = values_.insert(values_.end(), obj);
				target.second = target.first;
			}
			else
			{
				iterator iter(target.first);
				iterator iterEnd(target.second);
				++iterEnd;

				while (iter != iterEnd)
				{
					if (keyEqual_(extractKey_(*iter), extractKey_(obj)))
					{
						return std::pair<iterator, bool>(iter, false);
					}

					++iter;
				}

				target.first = values_.insert(target.first, obj);
			}

			++size_;

			iterator result(target.first);

			if (((float)size_ / buckets_.size()) > maxLoadFactor_)
			{
				// Always resizing (at least) by the factor of 2

				rehash(2 * buckets_.size());
				result = find(extractKey_(obj));
			}

			return std::pair<iterator, bool>(result, true);
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			iterator
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::insert(
			iterator hint, const value_type& obj)
		{
			// Ignore the hint
			return insert(obj).first;
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			const_iterator
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::insert(
			const_iterator hint, const value_type& obj)
		{
			// Ignore the hint
			return insert(obj).first;
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			template <class InputIterator>
		void UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			insert(InputIterator first, InputIterator last)
		{
			InputIterator current(first);

			iterator valuesFirst(end());
			iterator valuesLast(end());

			try
			{
				if (current != last)
				{
					std::pair<iterator, bool> result = insert(*current);
					valuesFirst = result.first;
					valuesLast = valuesFirst;
					++current;
				}

				while (current != last)
				{
					std::pair<iterator, bool> result = insert(*current);
					valuesLast = result.first;
					++current;
				}
			}
			catch(...)
			{
				++valuesLast;
				erase(valuesFirst, valuesLast);

				throw;
			}
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			iterator
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			erase(iterator position)
		{
			const size_type bucketIndex = bucket(extractKey_(*position));
			ASSERT1(!isEmptyBucket(bucketIndex), bucketIndex);

			Bucket& target(buckets_.at(bucketIndex));

			if (target.first == target.second)
			{
				ASSERT(target.first == position);

				target.first = end();
				target.second = end();
			}
			else if (position == target.first)
			{
				++target.first;
			}
			else if (position == target.second)
			{
				--target.second;
			}

			iterator result(position);
			++result;

			values_.erase(position);
			--size_;

			return result;
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			const_iterator
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			erase(const_iterator position)
		{
			size_type bucketIndex = bucket(*position);
			Bucket& target(buckets_.at(bucketIndex));

			if (target.first == target.second)
			{
				ASSERT(target.first == position);
				target.first = end();
				target.second = end();
			}

			if (position == target.first)
			{
				++target.first;
			}
			else if (position == target.second)
			{
				--target.second;
			}

			const_iterator result(position);
			++result;

			values_.erase(position);
			--size_;

			return result;
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			size_type
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			erase(const key_type& key)
		{
			iterator iter(find(key));

			if (iter == end())
			{
				return 0;
			}

			erase(iter);

			return 1;
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			iterator
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::erase(
			iterator first, iterator last)
		{
			iterator i(first);

			while (i != last)
			{
				i = erase(i);
			}

			return last;
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			const_iterator
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::erase(
			const_iterator first, const_iterator last)
		{
			const_iterator i(first);

			while (i != last)
			{
				i = erase(i);
			}

			return last;
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			void UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			clear()
		{
			size_ = 0;
			values_.clear();
			buckets_.clear();
			buckets_.push_back(Bucket(values_.end(), values_.end()));
			maxLoadFactor_ = 1;
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			void UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			swap(UnorderedBase& that)
		{
			std::swap(extractKey_, that.extractKey_);
			std::swap(hasher_, that.hasher_);
			std::swap(keyEqual_, that.keyEqual_);
			std::swap(size_, that.size_);
			values_.swap(that.values_);
			buckets_.swap(that.buckets_);
			std::swap(maxLoadFactor_, that.maxLoadFactor_);
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			hasher
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			hash_function() const
		{
			return hasher_;
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			key_equal
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			key_eq() const
		{
			return keyEqual_;
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			iterator
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			find(const key_type& key)
		{
			const size_type bucketIndex = bucket(key);

			if (isEmptyBucket(bucketIndex))
			{
				return end();
			}

			const Bucket& bucket = buckets_.at(bucketIndex);

			iterator iter(bucket.first);
			iterator iterEnd(bucket.second);
			++iterEnd;

			while (iter != iterEnd)
			{
				if (keyEqual_(extractKey_(*iter), key))
				{
					return iter;
				}

				++iter;
			}

			return end();
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			const_iterator
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			find(const key_type& key) const
		{
			const size_type bucketIndex = bucket(key);

			if (isEmptyBucket(bucketIndex))
			{
				return end();
			}

			const Bucket& bucket(buckets_.at(bucketIndex));

			const_iterator iter(bucket.first);
			const_iterator iterEnd(bucket.second);
			++iterEnd;

			while (iter != iterEnd)
			{
				if (keyEqual_(extractKey_(*iter), key))
				{
					return iter;
				}

				++iter;
			}

			return end();
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			size_type
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			count(const key_type& key) const
		{
			if (find(key) == end())
			{
				return 0;
			}

			return 1;
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			std::pair<
			typename
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			iterator,
			typename
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			iterator>
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			equal_range(const key_type& key)
		{
			const iterator first(find(key));

			if (first == end())
			{
				// Empty bucket.

				return std::pair<iterator, iterator>(
					end(), end());
			}

			iterator next(first);
			++next;

			return std::pair<iterator, iterator>(
				first, next);
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			std::pair<
			typename
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			const_iterator,
			typename
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			const_iterator>
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			equal_range(const key_type& key) const
		{
			const const_iterator first(find(key));

			if (first == end())
			{
				// Empty bucket.

				return std::pair<const_iterator, const_iterator>(
					end(), end());
			}

			const_iterator next(first);
			++next;

			return std::pair<const_iterator, const_iterator>(
				first, next);
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			size_type
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			bucket_count() const
		{
			return buckets_.size();
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			size_type
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			max_bucket_count() const
		{
			return std::numeric_limits<
				typename BucketContainer::size_type>::max();
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			size_type
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			bucket_size(size_type n) const
		{
			ENSURE2(n >= 0 && n < buckets_.size(),
				n, buckets_.size());

			if (isEmptyBucket(n))
			{
				return 0;
			}

			return std::distance(
				buckets_.at(n).first,
				buckets_.at(n).second) + 1;
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			size_type
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			bucket(const key_type& key) const
		{
			return hasher_(key) % buckets_.size();
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			local_iterator
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			begin(size_type n)
		{
			ENSURE2(n >= 0 && n < buckets_.size(),
				n, buckets_.size());

			return buckets_.at(n).first;
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			const_local_iterator
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			begin(size_type n) const
		{
			ENSURE2(n >= 0 && n < buckets_.size(),
				n, buckets_.size());

			return buckets_.at(n).first;
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			local_iterator
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			end(size_type n)
		{
			ENSURE2(n >= 0 && n < buckets_.size(),
				n, buckets_.size());

			if (isEmptyBucket(n))
			{
				return end();
			}

			iterator iter(buckets_.at(n).second);
			++iter;

			return iter;
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			typename UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			const_local_iterator
			UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			end(size_type n) const
		{
			ENSURE2(n >= 0 && n < buckets_.size(),
				n, buckets_.size());

			if (isEmptyBucket(n))
			{
				return end();
			}

			const_iterator iter(buckets_.at(n).second);
			++iter;

			return iter;
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			float UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			load_factor() const
		{
			return (float)size_ / buckets_.size();
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			float UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			max_load_factor() const
		{
			return maxLoadFactor_;
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			void UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			max_load_factor(float maxLoadFactor)
		{
			maxLoadFactor_ = maxLoadFactor;
		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
		void UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
		rehash(size_type bucketCount)
		{
			ENSURE1(bucketCount > 0, bucketCount);

			integer newBucketCount = followingPrime((integer)std::max(
				(float)bucketCount,
				(float)size_ / maxLoadFactor_ + 1));

			// Calculate hash values in advance
			std::vector<std::size_t> hashValues;
			hashValues.reserve(size_);

			iterator iter = values_.begin();
			iterator iterEnd = values_.end();
			while (iter != iterEnd)
			{
				hashValues.push_back(hasher_(extractKey_(*iter)));
				++iter;
			}

			// After this point an exception must not be thrown to
			// retain strong exception guarantee.
			ValueContainer temp(values_.get_allocator());

			buckets_ = BucketContainer(
				newBucketCount,
				Bucket(temp.end(), temp.end()));

			iter = values_.begin();
			iterEnd = values_.end();
			std::vector<std::size_t>::const_iterator hashIter =
				hashValues.begin();
			while (iter != iterEnd)
			{
				size_type bucketIndex = *hashIter % newBucketCount;
				ASSERT(bucketIndex < buckets_.size());

				Bucket& target(buckets_[bucketIndex]);

				iterator nextIter(iter);
				++nextIter;

				if (target.first == temp.end())
				{
					temp.splice(temp.end(), values_, iter);
					target.first = iter;
					target.second = iter;
				}
				else
				{
					temp.splice(target.first, values_, iter);
					target.first = iter;
				}

				iter = nextIter;
				++hashIter;
			}

			values_.swap(temp);

			ASSERT(load_factor() <= maxLoadFactor_);

		}

		template <class Key, class Value, class ExtractKey,
		class Hash, class Pred, class UniformAllocator>
			bool UnorderedBase<Key, Value, ExtractKey, Hash, Pred, UniformAllocator>::
			isEmptyBucket(size_type bucket) const
		{
			return buckets_.at(bucket).first == end();
		}

	}

}

#endif
