// Description: Hash functions
// Documentation: hash_tables.txt

#ifndef PASTEL_HASH_H
#define PASTEL_HASH_H

#include "pastel/sys/iteratorrange.h"

#include <boost/type_traits/is_arithmetic.hpp>

#include <string>

namespace Pastel
{

	//! Computes a 32-bit hash-value.
	/*!
	To provide support for hashing additional types, 
	overload the partialHash() function.
	*/
	template <typename Type, typename HashFunction>
	uint32 computeHash(
		const Type& that, 
		const HashFunction& hashFunction);

	//! Computes a 32-bit hash-value.
	/*!
	This is a convenience function that calls
	computeHash(that, Jenkins_HashFunction()).
	See the documentation for that function.
	*/
	template <typename Type>
	uint32 computeHash(const Type& that);

	//! Computes a 32-bit hash value for a sequence of objects.
	template <typename ConstIterator, typename HashFunction>
	uint32 computeHashMany(
		const ForwardRange<ConstIterator>& input,
		const HashFunction& hashFunction);

	//! Computes a 32-bit hash value for a sequence of objects.
	/*!
	This is a convenience function that calls
	computeHash(input, Jenkins_HashFunction()).
	See the documentation for that function.
	*/
	template <typename ConstIterator>
	uint32 computeHashMany(
		const ForwardRange<ConstIterator>& input);

	//! Computes a 32-bit partial hash value for a sequence of objects.
	template <typename ConstIterator, typename HashFunction>
	uint32 partialHashMany(
		const ForwardRange<ConstIterator>& input,
		uint32 currentHash,
		const HashFunction& hashFunction);

	//! Computes a 32-bit partial hash value for a sequence of objects.
	/*!
	This is a convenience function that calls
	partialHash(input, currentHash, Jenkins_HashFunction()).
	See the documentation for that function.
	*/
	template <typename ConstIterator>
	uint32 partialHashMany(
		const ForwardRange<ConstIterator>& input,
		uint32 currentHash);

	//! Computes a 32-bit partial hash value for a pointer.
	template <typename Type, typename HashFunction>
	uint32 partialHash(
		const Type* that, uint32 currentHash,
		const HashFunction& hashFunction);

	//! Computes a 32-bit partial hash value for a native arithmetic type.
	template <typename Type, typename HashFunction>
	PASTEL_ENABLE_IF(boost::is_arithmetic<Type>, uint32)
		partialHash(Type that, uint32 currentHash,
		const HashFunction& hashFunction);

	//! Computes a 32-bit partial hash value for an std::string.
	template <typename HashFunction>
	uint32 partialHash(
		const std::string& that, uint32 currentHash,
		const HashFunction& hashFunction);

	//! Computes a 32-bit partial hash value for an std::wstring.
	template <typename HashFunction>
	uint32 partialHash(
		const std::wstring& that, uint32 currentHash,
		const HashFunction& hashFunction);

	template <class T>
	struct Hash
		: public std::unary_function<T, std::size_t>
	{
		std::size_t operator()(const T& val) const
		{
			return computeHash(val);
		}
	};

}

#include "pastel/sys/hash.hpp"

#endif
