#ifndef PASTEL_HASHING_HPP
#define PASTEL_HASHING_HPP

#include "pastel/sys/hashing.h"

namespace Pastel
{

	template <typename Type>
	hash_integer computeHash(
		const Type& that)
	{
		std::hash<Type> hasher;
		return hasher(that);
	}

	inline uint32 combineHash(uint32 left, uint32 right)
	{ 
		// This code is from the Boost library.

		// This is 2^32 / [(1 + sqrt(5)) / 2],
		// where the divider is the golden ratio.
		// The important part about the inverse
		// of the golden ratio is that it is 
		// irrational, supposedly giving bits 
		// without a repeating pattern.
		const uint32 magic = 0x9e3779b9;

		left ^= 
			right + 
			magic + 
			(left << 6) + 
			(left >> 2);

		return left;
	} 

	inline uint64 combineHash(uint64 left, uint64 right)
	{ 
		// This is 2^64 / [(1 + sqrt(5)) / 2].
		// See the combineHash32() function.
		const uint64 magic = 
			0x4F1BBCDCBFA53E0Bull;

		left ^= 
			right + 
			magic + 
			(left << 6) + 
			(left >> 2);

		return left;
	} 

	template <typename ConstRange>
	hash_integer computeHashMany(
		const ConstRange& input)
	{
		auto iter = input.begin();

		hash_integer hash = 0;
		while(iter != input.end())
		{
			hash = combineHash(hash, 
				computeHash(*iter));
			++iter;
		}

		return hash;
	}

}

#include "pastel/sys/keyvalue.h"

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
			return combineHash(
				computeHash(that.key()),
				computeHash(that.value()));
		}
	};

}

namespace Pastel
{

	class Jenkins32_HashFunction
	{
	public:
		template <typename Char_ConstRange>
		uint32 partialHash(
			const Char_ConstRange& input,
			uint32 currentHash) const
		{
			// One-at-a-time hash function by Bob Jenkins,
			// the incremental part.
			
			// FIX: This function produces only 32-bit hashes.

			uint32 hash = currentHash;

			Char_ConstIterator iter = boost::begin(input);
			const Char_ConstIterator end = boost::end(input);
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
