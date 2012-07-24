#ifndef PASTEL_REFINABLE_PARTITION_FWD_H
#define PASTEL_REFINABLE_PARTITION_FWD_H

#include <list>
#include <vector>

namespace Pastel
{
	
	template <typename Type>
	class RefinablePartition_Fwd
	{
	public:
		class Element;
		class Block;

		typedef std::vector<Element>
			ElementSet;
		typedef typename ElementSet::iterator
			Element_Iterator;
		typedef typename ElementSet::const_iterator
			Element_ConstIterator;

		typedef std::list<Block>
			BlockSet;
		typedef typename BlockSet::iterator
			Block_Iterator;
		typedef typename BlockSet::const_iterator
			Block_ConstIterator;

		typedef std::vector<Element_Iterator>
			PartitionSet;
		typedef typename PartitionSet::iterator
			Partition_Iterator;
		typedef typename PartitionSet::const_iterator
			Partition_ConstIterator;

		typedef std::vector<Block_Iterator>
			SplitSet;
		typedef typename SplitSet::iterator
			Split_Iterator;
		typedef typename SplitSet::const_iterator
			Split_ConstIterator;
	};

}

#endif
