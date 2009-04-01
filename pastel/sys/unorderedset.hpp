#ifndef PASTELSYS_UNORDEREDSET_HPP
#define PASTELSYS_UNORDEREDSET_HPP

#include "pastel/sys/unorderedset.h"

namespace Pastel
{

	template <class Value, class Hash,
	class Pred, class UniformAllocator>
		UnorderedSet<Value, Hash, Pred, UniformAllocator>::
		UnorderedSet(
		const allocator_ptr& allocatorPtr)
		: Base(allocatorPtr)
	{
	}

	template <class Value, class Hash,
	class Pred, class UniformAllocator>
		UnorderedSet<Value, Hash, Pred, UniformAllocator>::
		UnorderedSet(
		size_type bucketCount,
		const hasher& pHasher,
		const key_equal& keyEqual,
		const allocator_ptr& allocatorPtr)
		: Base(bucketCount, pHasher, keyEqual, allocatorPtr)
	{
	}

	template <class Value, class Hash,
	class Pred, class UniformAllocator>
		template <class InputIterator>
	UnorderedSet<Value, Hash, Pred, UniformAllocator>::
		UnorderedSet(
		InputIterator f,
		InputIterator l,
		size_type bucketCount,
		const hasher& pHasher,
		const key_equal& keyEqual,
		const allocator_ptr& allocatorPtr)
		: Base(f, l, bucketCount, pHasher, keyEqual, allocatorPtr)
	{
	}

	template <class Value, class Hash,
	class Pred, class UniformAllocator>
		UnorderedSet<Value, Hash, Pred, UniformAllocator>::
		UnorderedSet(const UnorderedSet& that,
		const allocator_ptr& allocatorPtr)
		: Base(that, allocatorPtr)
	{
	}

}

#endif
