#ifndef PASTELSYS_UNORDEREDSET_H
#define PASTELSYS_UNORDEREDSET_H

#include "pastel/sys/unorderedbase.h"
#include "pastel/sys/functional.h"
#include "pastel/sys/poolallocator.h"

namespace Pastel
{

	template <
		class Value,
		class Hash = hash<Value>,
		class Pred = std::equal_to<Value>,
		class UniformAllocator = PoolAllocator>
	class UnorderedSet
		: public Detail::UnorderedBase<
			Value, Value, identity<Value>, Hash, Pred, UniformAllocator>
	{
	private:
		typedef Detail::UnorderedBase<
			Value, Value, identity<Value>, Hash, Pred, UniformAllocator>
			Base;

	public:
		typedef typename Base::allocator_ref allocator_ref;
		typedef typename Base::const_allocator_ref const_allocator_ref;

		typedef typename Base::key_type key_type;
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
		typedef	typename Base::const_local_iterator const_local_iterator;

		explicit UnorderedSet(
			const allocator_ref& allocatorRef);

		explicit UnorderedSet(
			size_type bucketCount = 1,
			const hasher& pHasher = hasher(),
			const key_equal& keyEqual = key_equal(),
			const allocator_ref& allocatorRef = allocator_ref());

		template <class InputIterator>
		UnorderedSet(
			InputIterator f,
			InputIterator l,
			size_type bucketCount = 1,
			const hasher& pHasher = hasher(),
			const key_equal& keyEqual = key_equal(),
			const allocator_ref& allocatorRef = allocator_ref());
		UnorderedSet(const UnorderedSet& that,
			const allocator_ref& allocatorRef = allocator_ref());
	};

}

#include "pastel/sys/unorderedset.hpp"

#endif
