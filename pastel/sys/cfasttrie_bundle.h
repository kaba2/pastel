// Description: C-fast trie chain bundle

#ifndef PASTELSYS_CFASTTRIE_BUNDLE_H
#define PASTELSYS_CFASTTRIE_BUNDLE_H

#include "pastel/sys/cfasttrie_fwd.h"
#include "pastel/sys/zero_higher_bits.h"

#include <map>

namespace Pastel
{

	template <
		typename Settings,
		template <typename> class Customization>
	class CFastTrie;

	namespace CFastTrie_
	{

		//! C-fast trie chain bundle
		template <typename CFastTrie_Settings>
		class Bundle
		{
		public:
			using Fwd = CFastTrie_Fwd<CFastTrie_Settings>;
			
			PASTEL_FWD(DataSet);
			PASTEL_FWD(Element);
			PASTEL_FWD(Element_Iterator);
			PASTEL_FWD(Element_ConstIterator);
			PASTEL_FWD(Iterator);
			PASTEL_FWD(ConstIterator);
			PASTEL_FWD(Chain_Iterator);
			PASTEL_FWD(Fork);
			PASTEL_FWD(ForkSet);
			PASTEL_FWD(Fork_ConstIterator);
			PASTEL_FWD(Fork_Iterator);
			PASTEL_FWD(Key);
			PASTEL_FWD(Value_Class);
			PASTEL_FWD(Bundle_Iterator);

			Bundle()
			: condition_(0)
			, forkSet_()
			, elementSet_()
			{
			}

			std::pair<Iterator, bool> insert(
				const Bundle_Iterator& bundle,
				bool equalToChain,
				const Key& key,
				const Value_Class& value)
			{
				if (elementSet_.empty() || key < elementSet_.begin().key())
				{
					Bundle_Iterator prevBundle = std::prev(bundle);
					if (!prevBundle->empty() && key == prevBundle->last().key())
					{
						return std::make_pair(prevBundle->last(), false);
					}
				}

				if (elementSet_.empty() || key > elementSet_.last().key())
				{
					Bundle_Iterator nextBundle = std::next(bundle);
					if (!nextBundle->empty() && key == nextBundle->begin().key())
					{
						return std::make_pair(nextBundle->begin(), false);
					}
				}

				integer complexity = 
					numberOfOneBits(key ^ (key >> 1));

				return elementSet_.insert(key, 
					Element(bundle, equalToChain, complexity, value));
			}

			Iterator erase(const ConstIterator& that)
			{
				return elementSet_.erase(that);
			}

			//! Finds the nearest element unequal to a chain.
			/*!
			Time complexity: O(log(size()))
			Exception safety: nothrow

			key:
			An element from which to start the search.

			direction:
			Whether to search for the smallest greater element
			(or the greatest smaller element).
			*/
			Iterator findNearestUnequalToChain(const Key& key)
			{
				Element_Iterator node = elementSet_.root();
				Element_Iterator candidate = elementSet_.end();

				integer minComplexity = node.propagation().minComplexity;
				if (minComplexity == infinity<integer>())
				{
					// There are no free elements.
					return candidate;
				}

				while(!node.isSentinel())
				{
					if (node.data().complexity() == minComplexity)
					{
						candidate = node;
						if (node.key() == key)
						{
							break;
						}
					}

					bool leftViable = 
						(node.left().propagation().minComplexity == minComplexity);
					bool rightViable = 
						(node.right().propagation().minComplexity == minComplexity);

					if (!leftViable && !rightViable)
					{
						break;
					}

					bool right = rightViable;
					if (leftViable && rightViable)
					{
						// When both sub-trees are viable,
						// choose the one which gets closer
						// to the key.
						right = (node.key() < key);
					}

					node = node.child(right);
				}

				ASSERT(candidate.isSentinel() ||
					candidate.data().complexity() == minComplexity);

				return candidate;
			}

			bool empty() const
			{
				return elementSet_.empty();
			}

			integer size() const
			{
				return elementSet_.size();
			}

			Iterator upperBound(const Key& key)
			{
				return elementSet_.upperBound(key);
			}

			PASTEL_ITERATOR_FUNCTIONS(begin, elementSet_.begin());
			PASTEL_ITERATOR_FUNCTIONS(end, elementSet_.end());
			PASTEL_ITERATOR_FUNCTIONS(last, std::prev(end()));

			//! Returns the fork closest to the key in tree-distance.
			Fork_ConstIterator closestFork(const Key& key) const
			{
				ASSERT(!forkSet_.empty());

				// The closest fork is either the successor 
				// or the predecessor of the key.
				Fork_ConstIterator right = 
					forkSet_.upper_bound(key);

				// TODO: Not sure if the checks are needed.
				if (right == forkSet_.cbegin())
				{
					return right;
				}

				Fork_ConstIterator left =
					std::prev(right);
				if (right == forkSet_.cend())
				{
					return left;
				}

				// Interestingly, we can can compute the closest fork
				// without computing tree-distances.
				if (zeroHigherBits(left->second.chain->key() ^ key) <
					zeroHigherBits(key ^ right->second.chain->key()))
				{
					return left;
				}

				return right;
			}

			Iterator cast(const ConstIterator& that)
			{
				return elementSet_.cast(that.base());
			}

			//! Returns the condition number.
			integer condition() const
			{
				return condition_;
			}

		private:
			template <
				typename Settings,
				template <typename> class Customization>
			friend class CFastTrie;

		private:
			//! Sets the condition number.
			void setCondition(integer condition)
			{
				condition_ = condition;
			}

			Fork_Iterator cast(const Fork_ConstIterator& fork)
			{
				return forkSet_.erase(fork, fork);
			}

			integer forks() const
			{
				return forkSet_.size();
			}

			void insertChain(
				const Chain_Iterator& chain)
			{
				if (chain->normal())
				{
					++condition_;
				}
				else
				{
					ASSERT_OP(condition_, >, 0);
					--condition_;
				}

				auto iter = elementSet_.find(chain->key());
				if (iter != elementSet_.cend())
				{
					iter->equalToChain_ = true;
					elementSet_.updateToRoot(iter);
				}
			}

			void removeChain(
				const Chain_Iterator& chain)
			{
				if (chain->normal())
				{
					//ASSERT_OP(condition_, >, 0);
					--condition_;
				}
				else
				{
					++condition_;
				}
			}

			Fork_Iterator insertFork(
				const Chain_Iterator& chain)
			{
				ASSERT(even(chain->key()));

				auto forkAndNew = 
					forkSet_.emplace(chain->key(), Fork(chain));

				Fork_Iterator fork = forkAndNew.first;
				bool isNew = forkAndNew.second;
				ASSERT(isNew);

				return fork;
			}

			Fork_Iterator findFork(const Key& key)
			{
				return forkSet_.find(key);
			}

			void removeFork(const Fork_Iterator& fork)
			{
				ASSERT(even(fork->second.chain->key()));
				forkSet_.erase(fork);
			}

			//! The condition number of the bundle.
			/*!
			The condition number is a non-negative integer 
			which is the difference between the number of
			normal chains and the number of abnormal chains
			in the bundle. When the condition number hits zero,
			the bundle is partitioned along the fork-chains.
			Delaying the partitioning in such a way amortizes 
			the complexity of inserting and removing elements.
			*/
			integer condition_;

			//! The set of fork-chains.
			/*!
			A fork-chain is an even chain such that the 
			next chain is odd. It follows that either the
			fork-chain or the next chain is a leaf-chain.
			*/
			ForkSet forkSet_;

			//! The set of elements.
			/*!
			Each bundle contains a set of elements stored
			in a red-black tree. These red-black trees
			are linked to each other in an ordered sequence.
			*/
			DataSet elementSet_;
		};

	}

}

#endif
