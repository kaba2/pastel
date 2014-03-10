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
		typename Customization>
	class CFastTrie;

	namespace CFastTrie_
	{

		//! C-fast trie chain bundle
		template <typename CFastTrie_Settings>
		class Bundle
		{
		public:
			using Fwd = CFastTrie_Fwd<CFastTrie_Settings>;
			
			PASTEL_FWD(Iterator);
			PASTEL_FWD(ConstIterator);
			PASTEL_FWD(Chain_Iterator);
			PASTEL_FWD(Fork);
			PASTEL_FWD(ForkSet);
			PASTEL_FWD(Fork_ConstIterator);
			PASTEL_FWD(Fork_Iterator);
			PASTEL_FWD(Key);

			Bundle()
			: condition_(0)
			, forkSet_()
			{
			}

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

			//! Returns the condition number.
			integer condition() const
			{
				return condition_;
			}

		private:
			template <
				typename Settings,
				typename Customization>
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
				const Iterator& element,
				const Chain_Iterator& chain)
			{
				ASSERT(even(chain->key()));

				auto forkAndNew = 
					forkSet_.emplace(chain->key(), Fork(element, chain));

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
		};

	}

}

#endif
