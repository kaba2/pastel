#ifndef PASTEL_HASH_HPP
#define PASTEL_HASH_HPP

#include "pastel/sys/hash.h"

namespace Pastel
{

	class Jenkins_HashFunction
	{
	public:
		template <typename Char_ConstIterator>
		uint32 partialHash(
			const ForwardRange<Char_ConstIterator>& input,
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

	template <typename Type, typename HashFunction>
	inline uint32 computeHash(
		const Type& that, 
		const HashFunction& hashFunction)
	{
		return hashFunction.finalize(
			partialHash(that, 0, hashFunction));
	}

	template <typename Type>
	inline uint32 computeHash(const Type& that)
	{
		return Pastel::computeHash(that, Jenkins_HashFunction());
	}

	template <typename ConstIterator, typename HashFunction>
	inline uint32 computeHashMany(
		const ForwardRange<ConstIterator>& input,
		const HashFunction& hashFunction)
	{
		return hashFunction.finalize(
			partialHashMany(input, 0, hashFunction));
	}

	template <typename ConstIterator>
	inline uint32 computeHashMany(
		const ForwardRange<ConstIterator>& input)
	{
		return Pastel::computeHashMany(input, Jenkins_HashFunction());
	}

	template <typename ConstIterator, typename HashFunction>
	inline uint32 partialHashMany(
		const ForwardRange<ConstIterator>& input,
		uint32 currentHash,
		const HashFunction& hashFunction)
	{
		uint32 result = currentHash;

		ConstIterator iter = input.begin();
		const ConstIterator end = input.end();
		while(iter != end)
		{
			result = partialHash(
				*iter, result, hashFunction);

			++iter;
		}
		
		return result;
	}

	template <typename ConstIterator>
	inline uint32 partialHashMany(
		const ForwardRange<ConstIterator>& input,
		uint32 currentHash)
	{
		return Pastel::partialHashMany(
			input, currentHash, Jenkins_HashFunction());
	}

	template <typename Type, typename HashFunction>
	inline uint32 partialHash(
		const Type* that, uint32 currentHash,
		const HashFunction& hashFunction)
	{
		return hashFunction.partialHash(
			forwardRange((char*)&that, sizeof(const Type*)), 
			currentHash);
	}

	template <typename Type, typename HashFunction>
	inline PASTEL_ENABLE_IF(boost::is_arithmetic<Type>, uint32)
		partialHash(Type that, uint32 currentHash,
		const HashFunction& hashFunction)
	{
		return hashFunction.partialHash(
			forwardRange((char*)&that, sizeof(Type)),
			currentHash);
	}

	template <typename HashFunction>
	inline uint32 partialHash(
		const std::string& that, uint32 currentHash,
		const HashFunction& hashFunction)
	{
		return hashFunction.partialHash(
			forwardRange(that.data(), that.size() * sizeof(char)),
			currentHash);
	}

	template <typename HashFunction>
	inline uint32 partialHash(
		const std::wstring& that, uint32 currentHash,
		const HashFunction& hashFunction)
	{
		return hashFunction.partialHash(
			forwardRange((char*)that.data(), 
			that.size() * sizeof(wchar_t)), currentHash);
	}

}

#endif
