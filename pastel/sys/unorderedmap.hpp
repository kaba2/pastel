#ifndef PASTELSYS_UNORDEREDMAP_HPP
#define PASTELSYS_UNORDEREDMAP_HPP

#include "pastel/sys/unorderedmap.h"

namespace Pastel
{

	template <class Key, class T,
	class Hash,	class Pred,
	class UniformAllocator>
		UnorderedMap<Key, T, Hash, Pred, UniformAllocator>::
		UnorderedMap(const allocator_ref& allocatorRef)
		: Base(allocatorRef)
	{
	}

	template <class Key, class T,
	class Hash,	class Pred,
	class UniformAllocator>
		UnorderedMap<Key, T, Hash, Pred, UniformAllocator>::
		UnorderedMap(size_type bucketCount, const hasher& pHasher,
		const key_equal& keyEqual, const allocator_ref& allocatorRef)
		: Base(bucketCount, pHasher, keyEqual, allocatorRef)
	{
	}

	template <class Key, class T,
	class Hash,	class Pred,
	class UniformAllocator>
		template <class InputIterator>
	UnorderedMap<Key, T, Hash, Pred, UniformAllocator>::
		UnorderedMap(
		InputIterator f,
		InputIterator l,
		size_type bucketCount,
		const hasher& pHasher,
		const key_equal& keyEqual,
		const allocator_ref& allocatorRef)
		: Base(f, l, bucketCount, pHasher, keyEqual, allocatorRef)
	{
	}

	template <class Key, class T,
	class Hash,	class Pred,
	class UniformAllocator>
		UnorderedMap<Key, T, Hash, Pred, UniformAllocator>::
		UnorderedMap(const UnorderedMap& that,
		const allocator_ref& allocatorRef)
		: Base(that, allocatorRef)
	{
	}

	template <class Key, class T,
	class Hash,	class Pred,
	class UniformAllocator>
		typename UnorderedMap<Key, T, Hash, Pred, UniformAllocator>::mapped_type&
		UnorderedMap<Key, T, Hash, Pred, UniformAllocator>::
		operator[](const key_type& key)
	{
		iterator iter(Base::find(key));
		if (iter == Base::end())
		{
			return Base::insert(
				value_type(key, mapped_type())).first->second;
		}
		else
		{
			return iter->second;
		}
	}

}

#endif
