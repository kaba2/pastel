// Description: Hash class for unordered containers
// Detail: Also some other needed functors.
// Documentation: hash_tables.txt

#ifndef PASTEL_FUNCTIONAL_H
#define PASTEL_FUNCTIONAL_H

#include "pastel/sys/hash.h"

#include <utility>
#include <functional>
#include <string>

namespace Pastel
{

	template <typename Type>
	struct identity
		: public std::unary_function<Type, Type>
	{
		const Type& operator()(const Type& val) const
		{
			return val;
		}
	};

	template <typename Type, typename Other>
	struct select1st
		: public std::unary_function<std::pair<Type, Other>, Type>
	{
		const Type& operator()(
			const std::pair<Type, Other>& val) const
		{
			return val.first;
		}
	};

	template <typename Type>
	struct equal_to
		: public std::binary_function<Type, Type, bool>
	{
		bool operator()(
			const Type& left,
			const Type& right) const
		{
			return left == right;
		}
	};

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

#include "pastel/sys/functional.hpp"

#endif
