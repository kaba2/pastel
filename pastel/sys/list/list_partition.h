// Description: Partition a list

#ifndef PASTELSYS_LIST_PARTITION_H
#define PASTELSYS_LIST_PARTITION_H

#include "pastel/sys/list.h"

namespace Pastel
{

	//! Partition a list.
	template <
		typename Settings,
		template <typename> class Customization,
		typename Predicate>
	std::pair<std::pair<typename List<Settings, Customization>::iterator, integer>,
		std::pair<typename List<Settings, Customization>::iterator, integer> >
		partition(
		List<Settings, Customization>& list,
		const typename List<Settings, Customization>::ConstRange& range,
		const Predicate& predicate);

	//! Partition a list with a fuzzy predicate.
	template <
		typename Settings,
		template <typename> class Customization,
		typename Predicate>
	std::pair<std::pair<typename List<Settings, Customization>::iterator, integer>,
		std::pair<typename List<Settings, Customization>::iterator, integer> >
		fuzzyPartition(
		List<Settings, Customization>& list,
		const typename List<Settings, Customization>::ConstRange& range,
		const Predicate& predicate);

}

#include "pastel/sys/list/list_partition.hpp"

#endif
