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

	template <typename Integer, typename Type> 
	PASTEL_ENABLE_IF((boost::mpl::and_<
		boost::is_integral<Integer>, 
		boost::mpl::bool_<sizeof(Integer) == sizeof(uint32)>>), Integer) 
		combineHash(Integer hash, const Type& that)
	{ 
		// This code is from the Boost library.

		// This is 2^32 / [(1 + sqrt(5)) / 2],
		// where the divider is the golden ratio.
		// The important part about the inverse
		// of the golden ratio is that it is 
		// irrational, supposedly giving bits 
		// without a repeating pattern.
		const uint32 magic = 0x9e3779b9;

		hash ^= 
			computeHash(that) + 
			magic + 
			(hash << 6) + 
			(hash >> 2);

		return hash;
	} 

	template <typename Integer, typename Type> 
	PASTEL_ENABLE_IF((boost::mpl::and_<
		boost::is_integral<Integer>, 
		boost::mpl::bool_<sizeof(Integer) == sizeof(uint64)>>), Integer) 
		combineHash(Integer hash, const Type& that)
	{ 
		// This is 2^64 / [(1 + sqrt(5)) / 2].
		// See the combineHash32() function.
		const uint64 magic = 
			0x4F1BBCDCBFA53E0Bull;

		hash ^= 
			computeHash(that) + 
			magic + 
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
