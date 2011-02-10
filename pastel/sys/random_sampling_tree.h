// Description: Random sampling tree

#ifndef PASTEL_RANDOM_SAMPLING_TREE_H
#define PASTEL_RANDOM_SAMPLING_TREE_H

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/lessthan.h"
#include "pastel/sys/random_uniform.h"
#include "pastel/sys/keyvalue.h"

namespace Pastel
{

	template <typename Real>
	class RandomSamplingTree_RbtPolicy
	{
	public:
		typedef Real ValueType;

		void swap(RbtPolicy_Concept& that)
		{
		}

		template <typename Node_Iterator>
		void updateInsertDown(const Node_Iterator& iter)
		{
		}

		template <typename Node_Iterator>
		void updateHierarchical(const Node_Iterator& iter)
		{
			iter->value() = 
				iter->key().key() +
				iter.left()->value() +
				iter.right()->value();
		}
	};

	// This simulates a template alias.
#	define PASTEL_RANDOM_SAMPLING_TREE(Real, Data) \
	Pastel::RedBlackTree<KeyValue<Real, Data>, Pastel::LessThan, \
	Pastel::RandomSamplingTree_RbtPolicy<Real> >
	
	template <typename Real, typename Data>
	typename PASTEL_RANDOM_SAMPLING_TREE(Real, Data)::ConstIterator
		randomlySample(
		const PASTEL_RANDOM_SAMPLING_TREE(Real, Data)& tree)
	{
		typedef PASTEL_RANDOM_SAMPLING_TREE(Real, Data) Tree;
		typedef typename Tree::ConstIterator ConstIterator;

		ConstIterator iter = tree.root();

		const Real totalMass = 
			iter->value();
		const Real variate = 
			randomOpen<Real>() * totalMass;

		Real minMass = 
			iter.left()->value();
		while(!iter.sentinel())
		{
			const Real mass =
				iter->key().key();

			if (variate < minMass)
			{
				iter = iter.left();
				minMass -= iter->key().key() + iter.right()->value();
			}
			else if (variate >= minMass + mass)
			{
				iter = iter.right();
				minMass += mass + iter.left()->value();
			}
			else
			{
				// This is the node we want to sample.
				return iter;
			}
		}
		
		// With real arithmetic, we should not
		// get here. However, maybe with floating 
		// point we will. So just in case...
		if (variate < totalMass / 2)
		{
			return tree.begin();
		}

		ConstIterator last = tree.end();
		--last;
		return last;
	}

}

#endif
