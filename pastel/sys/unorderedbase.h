#ifndef PASTELSYS_UNORDEREDBASE_H
#define PASTELSYS_UNORDEREDBASE_H

#include "pastel/sys/fastlist.h"

#include <boost/shared_ptr.hpp>

#include <vector>
#include <cstddef>

namespace Pastel
{

	namespace Detail
	{
		template <
			class Key, class Value, class ExtractKey,
			class Hash, class Pred, class UniformAllocator>
		class UnorderedBase
		{
		private:
			typedef FastList<Value, UniformAllocator> ValueContainer;

		public:
			// types
			typedef boost::shared_ptr<UniformAllocator>
				allocator_ref;
			typedef boost::shared_ptr<const UniformAllocator>
				const_allocator_ref;

			typedef Key key_type;
			typedef Value value_type;
			typedef ExtractKey extract_key;
			typedef Hash hasher;
			typedef Pred key_equal;
			typedef UniformAllocator allocator_type;

			typedef Value* pointer;
			typedef const Value* const_pointer;
			typedef Value& reference;
			typedef const Value& const_reference;
			typedef std::size_t size_type;
			typedef size_type difference_type;

			typedef typename ValueContainer::iterator iterator;
			typedef typename ValueContainer::const_iterator const_iterator;
			typedef iterator local_iterator;
			typedef const_iterator const_local_iterator;

		protected:
			// construct/destroy/copy

			explicit UnorderedBase(
				const allocator_ref& allocatorRef);

			explicit UnorderedBase(
				size_type bucketCount = 1,
				const hasher& pHasher = hasher(),
				const key_equal& keyEqual = key_equal(),
				const allocator_ref& allocatorRef = allocator_ref());

			template <class InputIterator>
			UnorderedBase(
				InputIterator f,
				InputIterator l,
				size_type bucketCount = 1,
				const hasher& pHasher = hasher(),
				const key_equal& keyEqual = key_equal(),
				const allocator_ref& allocatorRef = allocator_ref());
			UnorderedBase(const UnorderedBase& that,
				const allocator_ref& allocatorRef = allocator_ref());

			~UnorderedBase();
			UnorderedBase& operator=(const UnorderedBase& that);

		public:
			void set_allocator(const allocator_ref& allocatorRef);
			allocator_ref get_allocator() const;

			// size and capacity
			bool empty() const;
			size_type size() const;
			size_type max_size() const;

			// iterators
			iterator begin();
			const_iterator begin() const;
			iterator end();
			const_iterator end() const;

			// modifiers
			std::pair<iterator, bool> insert(const value_type& obj);
			iterator insert(iterator hint, const value_type& obj);
			const_iterator
			insert(const_iterator hint, const value_type& obj);
			template <class InputIterator>
			void insert(InputIterator first, InputIterator last);

			iterator erase(iterator position);
			const_iterator erase(const_iterator position);
			size_type erase(const key_type& k);
			iterator erase(iterator first, iterator last);
			const_iterator erase(const_iterator first, const_iterator last);
			void clear();

			void swap(UnorderedBase&);

			// observers
			hasher hash_function() const;
			key_equal key_eq() const;

			// lookup
			iterator find(const key_type& k);
			const_iterator find(const key_type& k) const;
			size_type count(const key_type& k) const;
			std::pair<iterator, iterator> equal_range(const key_type& k);
			std::pair<const_iterator, const_iterator> equal_range(
				const key_type& k) const;

			// bucket interface
			size_type bucket_count() const;
			size_type max_bucket_count() const;
			size_type bucket_size(size_type n) const;
			size_type bucket(const key_type& k) const;
			local_iterator begin(size_type n);
			const_local_iterator begin(size_type n) const;
			local_iterator end(size_type n);
			const_local_iterator end(size_type n) const;

			// hash policy
			float load_factor() const;
			float max_load_factor() const;
			void max_load_factor(float z);
			void rehash(size_type n);

		private:
			bool isEmptyBucket(size_type bucket) const;

			typedef std::pair<iterator, iterator> Bucket;
			typedef std::vector<Bucket> BucketContainer;

			extract_key extractKey_;
			hasher hasher_;
			key_equal keyEqual_;

			size_type size_;
			ValueContainer values_;
			BucketContainer buckets_;
			float maxLoadFactor_;
		};
	}

}

#include "pastel/sys/unorderedbase.hpp"

#endif
