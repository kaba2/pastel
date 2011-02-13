// Description: Random sampling tree

#ifndef PASTEL_RANDOM_SAMPLING_TREE_H
#define PASTEL_RANDOM_SAMPLING_TREE_H

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/lessthan.h"
#include "pastel/sys/random_uniform.h"
#include "pastel/sys/keyvalue.h"

namespace Pastel
{

	template <typename Real, typename Value>
	class RandomSamplingTree_RbtPolicy
	{
	public:
		struct ValueType
		{
			ValueType()
				: totalMass(0)
				, value()
			{
			}

			Real totalMass;
			Value value;
		};

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
			iter->value().totalMass = 
				iter->key().key() +
				iter.left()->value().totalMass +
				iter.right()->value().totalMass;
		}
	};

	// This simulates a template alias.
	template <typename Real, typename Key, typename Value = EmptyClass>
	class RandomSamplingTree
	{
	public:
		typedef RedBlackTree<KeyValue<Real, Key>, LessThan,
			RandomSamplingTree_RbtPolicy<Real, Value> > Tree;
		typedef typename Tree::ConstIterator ConstIterator;
		typedef typename Tree::Iterator Iterator;

		Tree& tree()
		{
			return tree_;
		}

		const Tree& tree() const
		{
			return tree_;
		}

	private:
		Tree tree_;
	};

	template <typename Real, typename Key, typename Value>
	typename RandomSamplingTree<Real, Key, Value>::ConstIterator
		randomlySample(
		const RandomSamplingTree<Real, Key, Value>& samplingTree)
	{
		typedef RandomSamplingTree<Real, Key, Value> SamplingTree;
		typedef typename SamplingTree::Tree Tree;
		typedef typename Tree::ConstIterator ConstIterator;

		const Tree& tree = samplingTree.tree();

		ConstIterator iter = tree.root();

		const Real totalMass = 
			iter->value().totalMass;
		const Real variate = 
			randomOpen<Real>() * totalMass;

		Real minMass = 
			iter.left()->value().totalMass;
		while(!iter.sentinel())
		{
			const Real mass =
				iter->key().key();

			if (variate < minMass)
			{
				iter = iter.left();
				minMass -= iter->key().key() + iter.right()->value().totalMass;
			}
			else if (variate >= minMass + mass)
			{
				iter = iter.right();
				minMass += mass + iter.left()->value().totalMass;
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
