// Description: UnorderedMap class
// Detail: An unordered associative map
// Documentation: unordered.txt

#ifndef PASTEL_UNORDEREDMAP_H
#define PASTEL_UNORDEREDMAP_H

#include "pastel/sys/unorderedbase.h"
#include "pastel/sys/functional.h"
#include "pastel/sys/poolallocator.h"

namespace Pastel
{

	template <
		class Key,
		class T,
		class Hash = hash<Key>,
		class Pred = std::equal_to<Key>,
		class UniformAllocator = PoolAllocator>
	class UnorderedMap
		: public Detail::UnorderedBase<
			Key, std::pair<const Key, T>,
			select1st<const Key, T>,
			Hash, Pred, UniformAllocator>
	{
	private:
		typedef Detail::UnorderedBase<
			Key, std::pair<const Key, T>,
			select1st<const Key, T>,
			Hash, Pred, UniformAllocator>
			Base;
	public:
		typedef Key key_type;
		typedef T mapped_type;

		typedef typename Base::allocator_ptr allocator_ptr;
		typedef typename Base::const_allocator_ptr const_allocator_ptr;

		typedef typename Base::value_type value_type;
		typedef typename Base::hasher hasher;
		typedef typename Base::key_equal key_equal;
		typedef typename Base::allocator_type allocator_type;
		typedef typename Base::pointer pointer;
		typedef typename Base::const_pointer const_pointer;
		typedef typename Base::reference reference;
		typedef typename Base::const_reference const_reference;
		typedef typename Base::size_type size_type;
		typedef typename Base::difference_type difference_type;

		typedef typename Base::iterator iterator;
		typedef typename Base::const_iterator const_iterator;
		typedef typename Base::local_iterator local_iterator;
		typedef typename Base::const_local_iterator const_local_iterator;

		explicit UnorderedMap(
			const allocator_ptr& allocatorPtr);

		explicit UnorderedMap(
			size_type bucketCount = 1,
			const hasher& pHasher = hasher(),
			const key_equal& keyEqual = key_equal(),
			const allocator_ptr& allocatorPtr = allocator_ptr());

		template <class InputIterator>
		UnorderedMap(
			InputIterator f,
			InputIterator l,
			size_type bucketCount = 1,
			const hasher& pHasher = hasher(),
			const key_equal& keyEqual = key_equal(),
			const allocator_ptr& allocatorPtr = allocator_ptr());

		UnorderedMap(const UnorderedMap&,
			const allocator_ptr& allocatorPtr = allocator_ptr());

		mapped_type& operator[](const key_type& k);
	};

}

#include "pastel/sys/unorderedmap.hpp"

#endif
