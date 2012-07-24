#ifndef PASTEL_REFINABLE_PARTITION_FWD_H
#define PASTEL_REFINABLE_PARTITION_FWD_H

#include <list>
#include <vector>

namespace Pastel
{
	
	class RefinablePartition_Fwd
	{
	public:
		class Element;
		class Block;

		typedef std::vector<Element>
			ElementSet;
		typedef ElementSet::iterator
			Element_Iterator;
		typedef ElementSet::const_iterator
			Element_ConstIterator;

		typedef std::list<Block>
			BlockSet;
		typedef BlockSet::iterator
			Block_Iterator;
		typedef BlockSet::const_iterator
			Block_ConstIterator;

		typedef std::vector<Element_Iterator>
			PartitionSet;
		typedef PartitionSet::iterator
			Partition_Iterator;
		typedef PartitionSet::const_iterator
			Partition_ConstIterator;

		typedef std::vector<Block_Iterator>
			SplitSet;
		typedef SplitSet::iterator
			Split_Iterator;
		typedef SplitSet::const_iterator
			Split_ConstIterator;
	};

}

#endif
