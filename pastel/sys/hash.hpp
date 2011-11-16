#ifndef PASTEL_HASH_HPP
#define PASTEL_HASH_HPP

#include "pastel/sys/hash.h"

#include <boost/type_traits/is_same.hpp>

namespace Pastel
{

	class Jenkins_HashFunction
	{
	public:
		enum
		{
			HashIntegerIs32Bit = boost::is_same<hash_integer, uint32>::value
		};
		PASTEL_STATIC_ASSERT(HashIntegerIs32Bit);

		template <typename Char_ConstIterator>
		hash_integer partialHash(
			const ForwardIterator_Range<Char_ConstIterator>& input,
			hash_integer currentHash) const
		{
			// One-at-a-time hash function by Bob Jenkins,
			// the incremental part.
			
			// FIX: This function produces only 32-bit hashes.

			hash_integer hash = currentHash;

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

		hash_integer finalize(hash_integer currentHash) const
		{
			// One-at-a-time hash function by Bob Jenkins,
			// the finalization part.

			// FIX: This function produces only 32-bit hashes.

			hash_integer hash = currentHash;

			hash += (hash << 3);
			hash ^= (hash >> 11);
			hash += (hash << 15);

			return hash;
		}
	};

	template <typename Type, typename HashFunction>
	inline hash_integer computeHash(
		const Type& that, 
		const HashFunction& hashFunction)
	{
		return hashFunction.finalize(
			partialHash(that, 0, hashFunction));
	}

	template <typename Type>
	inline hash_integer computeHash(const Type& that)
	{
		return Pastel::computeHash(that, Jenkins_HashFunction());
	}

	template <typename ConstIterator, typename HashFunction>
	inline hash_integer computeHashMany(
		const ForwardIterator_Range<ConstIterator>& input,
		const HashFunction& hashFunction)
	{
		return hashFunction.finalize(
			partialHashMany(input, 0, hashFunction));
	}

	template <typename ConstIterator>
	inline hash_integer computeHashMany(
		const ForwardIterator_Range<ConstIterator>& input)
	{
		return Pastel::computeHashMany(input, Jenkins_HashFunction());
	}

	template <typename ConstIterator, typename HashFunction>
	inline hash_integer partialHashMany(
		const ForwardIterator_Range<ConstIterator>& input,
		hash_integer currentHash,
		const HashFunction& hashFunction)
	{
		hash_integer result = currentHash;

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
	inline hash_integer partialHashMany(
		const ForwardIterator_Range<ConstIterator>& input,
		hash_integer currentHash)
	{
		return Pastel::partialHashMany(
			input, currentHash, Jenkins_HashFunction());
	}

	template <typename Type, typename HashFunction>
	inline hash_integer partialHash(
		const Type* that, hash_integer currentHash,
		const HashFunction& hashFunction)
	{
		return hashFunction.partialHash(
			range((char*)&that, sizeof(const Type*)), 
			currentHash);
	}

	template <typename Type, typename HashFunction>
	inline PASTEL_ENABLE_IF(boost::is_arithmetic<Type>, hash_integer)
		partialHash(Type that, hash_integer currentHash,
		const HashFunction& hashFunction)
	{
		return hashFunction.partialHash(
			range((char*)&that, sizeof(Type)),
			currentHash);
	}

	template <typename HashFunction>
	inline hash_integer partialHash(
		const std::string& that, hash_integer currentHash,
		const HashFunction& hashFunction)
	{
		return hashFunction.partialHash(
			range(that.data(), that.size() * sizeof(char)),
			currentHash);
	}

	template <typename HashFunction>
	inline hash_integer partialHash(
		const std::wstring& that, hash_integer currentHash,
		const HashFunction& hashFunction)
	{
		return hashFunction.partialHash(
			range((char*)that.data(), 
			that.size() * sizeof(wchar_t)), currentHash);
	}

	template <typename Key, typename Value, typename HashFunction>
	hash_integer partialHash(
		const KeyValue<Key, Value>& that, hash_integer currentHash,
		const HashFunction& hashFunction)
	{
		hash_integer hash = partialHash(
			that.key(), currentHash, hashFunction);

		hash = partialHash(
			that.value(), hash, hashFunction);

		return hash;
	}

	template <typename Type>
	hash_integer partialHash(
		const Type& that, hash_integer currentHash)
	{
		return Pastel::partialHash(that, currentHash,
			Jenkins_HashFunction());
	}

	template <typename HashFunction>
	hash_integer finalizeHash(hash_integer currentHash,
		const HashFunction& hashFunction)
	{
		return hashFunction.finalize(currentHash);
	}

	inline hash_integer finalizeHash(
		hash_integer currentHash)
	{
		return Pastel::finalizeHash(currentHash,
			Jenkins_HashFunction());
	}

}

#endif
