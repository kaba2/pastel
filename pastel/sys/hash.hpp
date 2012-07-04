#ifndef PASTEL_HASH_HPP
#define PASTEL_HASH_HPP

#include "pastel/sys/hash.h"

namespace Pastel
{

	template <typename Type>
	hash_integer computeHash(
		const Type& that)
	{
		std::hash<Type> hasher;
		return hasher(that);
	}

	template <class Type> 
	hash_integer combineHash(
		hash_integer hash, 
		const Type& that)
	{ 
		// This code is from the Boost library.

		hash ^= 
			computeHash(that) + 
			0x9e3779b9 + 
			(hash << 6) + 
			(hash >> 2);

		return hash;
	} 

	template <typename ConstIterator>
	hash_integer computeHashMany(
		ForwardIterator_Range<ConstIterator> input)
	{
		hash_integer hash = 0;
		while(!input.empty())
		{
			hash = combineHash(hash, input.front());
			input.pop_front();
		}
		return hash;
	}

}

namespace std
{

	// The hash can be specialized for user-defined types,
	// but not for the types in the STL itself.

	template <typename Key, typename Value>
	class hash<Pastel::KeyValue<Key, Value>>
	{
	public:
		Pastel::hash_integer operator()(
			const Pastel::KeyValue<Key, Value>& that) const
		{
			return thatcombineHash(
				thatcomputeHash(that.key()),
				that.value());
		}
	};

}

namespace Pastel
{

	class Jenkins32_HashFunction
	{
	public:
		template <typename Char_ConstIterator>
		uint32 partialHash(
			const ForwardIterator_Range<Char_ConstIterator>& input,
			uint32 currentHash) const
		{
			// One-at-a-time hash function by Bob Jenkins,
			// the incremental part.
			
			// FIX: This function produces only 32-bit hashes.

			uint32 hash = currentHash;

			Char_ConstIterator iter = input.begin();
			const Char_ConstIterator end = input.end();
			while(iter != end)
			{
				hash += *iter;
				hash += (hash << 10);
				hash ^= (hash >> 6);

				++iter;
			}

			return hash;
		}

		uint32 finalize(uint32 currentHash) const
		{
			// One-at-a-time hash function by Bob Jenkins,
			// the finalization part.

			// FIX: This function produces only 32-bit hashes.

			uint32 hash = currentHash;

			hash += (hash << 3);
			hash ^= (hash >> 11);
			hash += (hash << 15);

			return hash;
		}
	};

}

#endif
