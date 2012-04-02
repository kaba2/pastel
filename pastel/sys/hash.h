// Description: Hash functions
// Documentation: hash_tables.txt

#ifndef PASTEL_HASH_H
#define PASTEL_HASH_H

#include "pastel/sys/iterator_range.h"
#include "pastel/sys/keyvalue.h"

#include <boost/iterator/iterator_traits.hpp>

#include <string>

namespace Pastel
{

	//! Computes a hash-value.
	/*!
	To provide support for hashing additional types, 
	overload the partialHash() function.
	*/
	template <typename Type, typename HashFunction>
	hash_integer computeHash(
		const Type& that, 
		const HashFunction& hashFunction);

	//! Computes a hash-value.
	/*!
	This is a convenience function that calls
	computeHash(that, Jenkins_HashFunction()).
	See the documentation for that function.
	*/
	template <typename Type>
	hash_integer computeHash(const Type& that);

	//! Computes a hash value for a sequence of objects.
	template <typename ConstIterator, typename HashFunction>
	hash_integer computeHashMany(
		const ForwardIterator_Range<ConstIterator>& input,
		const HashFunction& hashFunction);

	//! Computes a hash value for a sequence of objects.
	/*!
	This is a convenience function that calls
	computeHashMany(input, Jenkins_HashFunction()).
	See the documentation for that function.
	*/
	template <typename ConstIterator>
	hash_integer computeHashMany(
		const ForwardIterator_Range<ConstIterator>& input);

	//! Computes a partial hash value for a sequence of objects.
	template <typename ConstIterator, typename HashFunction>
	hash_integer partialHashMany(
		const ForwardIterator_Range<ConstIterator>& input,
		hash_integer currentHash,
		const HashFunction& hashFunction);

	//! Computes a partial hash value for a sequence of objects.
	/*!
	This is a convenience function that calls
	partialHash(input, currentHash, Jenkins_HashFunction()).
	See the documentation for that function.
	*/
	template <typename ConstIterator>
	hash_integer partialHashMany(
		const ForwardIterator_Range<ConstIterator>& input,
		hash_integer currentHash);

	//! Computes a partial hash value for a pointer.
	template <typename Type, typename HashFunction>
	hash_integer partialHash(
		const Type* that, hash_integer currentHash,
		const HashFunction& hashFunction);

	//! Computes a partial hash value for a native arithmetic type.
	template <typename Type, typename HashFunction>
	PASTEL_ENABLE_IF(std::is_arithmetic<Type>, hash_integer)
		partialHash(Type that, hash_integer currentHash,
		const HashFunction& hashFunction);

	//! Computes a partial hash value for an std::string.
	template <typename HashFunction>
	hash_integer partialHash(
		const std::string& that, hash_integer currentHash,
		const HashFunction& hashFunction);

	//! Computes a partial hash value for an std::wstring.
	template <typename HashFunction>
	hash_integer partialHash(
		const std::wstring& that, hash_integer currentHash,
		const HashFunction& hashFunction);

	//! Computes a partial hash value for a KeyValue.
	template <typename Key, typename Value, typename HashFunction>
	hash_integer partialHash(
		const KeyValue<Key, Value>& that, hash_integer currentHash,
		const HashFunction& hashFunction);

	//! Computes a partial hash value for a pointer.
	/*!
	This is a convenience function that calls
	partialHash(that, currentHash, Jenkins_HashFunction()).
	*/
	template <typename Type>
	hash_integer partialHash(
		const Type& that, hash_integer currentHash);

	//! Perform the final hash value after partial hashes.
	template <typename HashFunction>
	hash_integer finalizeHash(hash_integer currentHash,
		const HashFunction& hashFunction);

	//! Perform the final hash value after partial hashes.
	/*!
	This is a convenience function that calls
	finalizeHash(currentHash, Jenkins_HashFunction()).
	*/
	hash_integer finalizeHash(hash_integer currentHash);

	template <class T>
	struct Hash
		: public std::unary_function<T, std::size_t>
	{
		std::size_t operator()(const T& val) const
		{
			return computeHash(val);
		}
	};

	template <typename Iterator>
	class IteratorAddress_Hash
		: public std::unary_function<Iterator, std::size_t>
	{
	public:
		std::size_t operator()(const Iterator& iter) const
		{
			typedef typename 
				boost::iterator_value<Iterator>::type Type;

			return computeHash(&*iter);
		}
	};

	template <typename Iterator>
	IteratorAddress_Hash<Iterator> iteratorAddressHash(
		const Iterator& iter)
	{
		return IteratorAddress_Hash<Iterator>();
	}

}

#include "pastel/sys/hash.hpp"

#endif
