#ifndef PASTELSYS_HASHING_HPP
#define PASTELSYS_HASHING_HPP

#include "pastel/sys/hashing.h"

#include <boost/type_traits/is_same.hpp>

namespace Pastel
{

	template <typename Type>
	hash_integer computeHash(
		const Type& that)
	{
		std::hash<Type> hasher;
		return hasher(that);
	}

	template <typename Integer>
	PASTEL_ENABLE_IF_C(sizeof(Integer) == sizeof(uint32), Integer) 
		combineHash(Integer left, Integer right)
	{ 
		// This code is from the Boost library.

		// This is 2^32 / [(1 + sqrt(5)) / 2],
		// where the divider is the golden ratio.
		// The important part about the inverse
		// of the golden ratio is that it is 
		// irrational, supposedly giving bits 
		// without a repeating pattern.
		const Integer magic = 0x9e3779b9;

		left ^= 
			right + 
			magic + 
			(left << 6) + 
			(left >> 2);

		return left;
	} 

	template <typename Integer>
	PASTEL_ENABLE_IF_C(sizeof(Integer) == sizeof(uint64), Integer) 
		combineHash(Integer left, Integer right)
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

			auto iter = boost::begin(input);
			auto end = boost::end(input);
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
