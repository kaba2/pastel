#ifndef PASTELSYS_FASTLIST_TOOLS_H
#define PASTELSYS_FASTLIST_TOOLS_H

#include "pastel/sys/fastlist.h"

namespace Pastel
{

	template <typename Type, typename UniformAllocator, typename Predicate>
	std::pair<std::pair<typename FastList<Type, UniformAllocator>::iterator, integer>,
		std::pair<typename FastList<Type, UniformAllocator>::iterator, integer> >
		partition(
		FastList<Type, UniformAllocator>& list,
		const typename FastList<Type, UniformAllocator>::const_iterator& from,
		const typename FastList<Type, UniformAllocator>::const_iterator& to,
		const Predicate& predicate);

	template <typename Type, typename UniformAllocator, typename Predicate>
	std::pair<std::pair<typename FastList<Type, UniformAllocator>::iterator, integer>,
		std::pair<typename FastList<Type, UniformAllocator>::iterator, integer> >
		fuzzyPartition(
		FastList<Type, UniformAllocator>& list,
		const typename FastList<Type, UniformAllocator>::const_iterator& from,
		const typename FastList<Type, UniformAllocator>::const_iterator& to,
		const Predicate& predicate);

}

#include "pastel/sys/fastlist_tools.hpp"

#endif
