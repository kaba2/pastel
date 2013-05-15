// Description: Y-fast trie

#ifndef PASTELSYS_YFASTTRIE_H
#define PASTELSYS_YFASTTRIE_H

#include "pastel/sys/number_tests.h"
#include "pastel/sys/redblacktree.h"

#include <bitset>
#include <array>

namespace Pastel
{

	namespace YFastTrie_
	{

		class Node
		{
		public:
			Node* left() const
			{
				return left_;
			}

			Node* right() const
			{
				return right_;
			}

			bool isLeaf() const
			{
				return !left() && !right();
			}

			bool splitsBoth() const
			{
				return left() && right();
			}

			Node* left_;
			Node* right_;
		};

		template <typename Integer, typename Value_Class>
		class Leaf_Node
		: public Node
		{
		public:
			Map<Integer, Value_Class> map_;
		};	

	}

	//! Y-fast trie
	/*!
	Preconditions:
	isPowerOfTwo(N).

	N:	
	The number of bits in a stored integer.

	References
	----------

	"Log-logarithmic Queries are Possible in Space Theta(N)",
	Dan E. Willard, Information Processing Letters 17 (1983),
	pp. 81-84.

	"Fast Local Searches and Updates in Bounded Universes",
	Prosenjit Bose et al., CCCG 2010 (2010).
	*/
	template <int N, typename Value = void>
	class YFastTrie
	{
	private:
		PASTEL_STATIC_ASSERT(N > 0);

		// FIX: Replace with isPowerOfTwo constexpr
		// after constexpr becomes available in
		// Visual Studio.
		enum
		{
			NIsPowerOfTwo = (N & (N - 1) == 0)
		};
		PASTEL_STATIC_ASSERT(NIsPowerOfTwo);
		// PASTEL_STATIC_ASSERT(isPowerOfTwo(N));

		using Node = YFastTrie_::Node;
		using Leaf_Node = YFastTrie_::Leaf_Node;
		using Split_Node = YFastTrie_::Split_Node;

		using Trie_Iterator = YFastTrie_::Trie_Iterator;
		using Trie_ConstIterator = YFastTrie_::Trie_ConstIterator;

		using Integer = Pastel::Integer<N>;
		using IntegerSet = std::unordered_map<Integer, Node*>;
		using IntegerSet_Iterator = typename IntegerSet::iterator;
		using IntegerSet_ConstIterator = typename IntegerSet::const_iterator;

		using Bucket = Map<Integer, Value>;
		using Bucket_ConstIterator = typename IntegerTree::ConstIterator;
		using Bucket_Iterator = typename IntegerTree::Iterator;

		using ElementSetSet = std::array<IntegerSet, N>;

	public:
		//! Inserts an element.
		/*!
		Time complexity:
		O(log(log(Delta))) expected amortized time,	where 
		Delta = that - *lower_bound(that)
		*/
		void insert(Integer key, Value_Class value)
		{
			if (empty())
			{
				// The tree is empty.

				// Create a new leaf node.
				std::unique_ptr<Leaf_node> leaf(new Leaf_Node());
				Trie_ConstIterator element(leaf.get());

				// Create the split-key to store into the
				// level hash set.
				Integer splitKey;
				splitKey.set(0, key.test(N - 1));
				
				// Insert the key-value pair into the 
				// leaf node bucket.
				Bucket& bucket = bucket(element);
				bucket.insert(std::move(key), std::move(value));

				// Insert the split-key to the level-hash set.
				elementSetSet_[N - 1].insert(
					std::make_pair(std::move(splitKey), element));

				// Update the trie.
				root_ = leaf.release();
				++size_;

				return;
			}

			Trie_ConstIterator lowest =
				lowestAncestor(key);

			if (lowest.isLeaf())
			{
				// Add the element into the bucket.
				bucket(lowest).insert(std::move(key), std::move(value));

				while (true)
				{
					Bucket& bucket = bucket(lowest);
					if (bucket.size() > N)
					{
						// The bucket became too large.
						
						// Split the trie into two at this node.
						std::unique_ptr<Leaf_Node> right(new Leaf_Node);
						Bucket& rightBucket = right->bucket_;
						
						Leaf_Node* left = lowest.base();
						Bucket& leftBucket = bucket;

						std::unique_ptr<Node> parent(new Node(left, right));
						left->parent_ = parent.get();
						right->parent_ = parent.get();

						// Find the splitting key.
						Integer splitKey(key);
						splitKey.resetBits(0, level);
					
						// Splice the higher elements from the left bucket
						// into the right bucket.
						Bucket_ConstIterator iter = 
							leftBucket.lower_bound(splitKey);
						while(iter != bucket.cend())
						{
							iter = rightBucket.splice(leftBucket, iter);
						}

						// Recurse to the proper leaf node.
						if (key < splitKey)
						{
							lowest = Trie_ConstIterator(left);
						}
						else
						{
							lowest = Trie_ConstIterator(right);
						}
						--level;
					}
					else
					{
						break;
					}
				}
			}
			else
			{
			}
		}

		const Bucket& bucket(const Trie_ConstIterator& leaf) const
		{
			ENSURE(leaf.isLeaf());
			Leaf_Node* leaf = (Leaf_Node*)leaf.base();
			return leaf->map_;
		}

		Trie_Iterator lowestAncestor(const Integer& key)
		{
			return cast(addConst(*this).lowestAncestor(key));
		}

		Trie_ConstIterator lowestAncestor(const Integer& key) const
		{
			if (empty())
			{
				return cend();
			}

			// Search the trie levels using binary search.
			Node* lowest = 0;
			integer bottom = 0;
			integer top = levels();
			while (bottom != top)
			{
				integer middle = (bottom + top) / 2;
				
				Integer keySuffix(key);
				keySuffix >>= middle;

				IntegerSet_ConstIterator iter =
					elementSetSet_[middle].find(keySuffix);
				if (iter == elementSetSet_.cend())
				{
					// Since the suffix is not found at this
					// level, longer suffixes wont be found
					// at lower levels either. Continue on
					// higher levels.
					bottom = middle + 1;
				}
				else
				{
					// The suffix was found at this level.
					// However, descendants of this node may
					// be lower ancestors than this node.
					// Continue on lower levels.
					top = middle;
					lowest = iter->second;
				}
			}
			
			// Since we already checked that the tree
			// is not empty, the lowest ancestor must
			// exists.
			ASSERT(lowest);

			// If the node had both children, then it
			// would not be the lowest ancestor.
			ASSERT(!lowest->splitsBoth());

			return ConstIterator(lowest);
		}

		//! Returns an iterator to the given element, if it exists.
		/*!
		Time complexity: 
		O(log(log(N))), if the element exist,
		O(1) on average, otherwise.

		Exception safety:
		nothrow

		returns:
		An iterator to the element, if the element exists,
		cend(), otherwise.
		*/
		ConstIterator find(const Integer& that) const
		{
			// Check in average O(1) time whether the element
			// exists.
			if (!exists(that))
			{
				return tree.cend();
			}

			ConstIterator node = tree_.root();
			for (integer bit = N - 1; bit >= 0;--bit)
			{
				node = node.child(that[bit]);

				// The node can not be empty, since
				// we already checked that the element
				// is stored in the tree.
				ASSERT(!node.empty());
			}

			return node;
		}

		//! Returns whether the element exists.
		/*!
		Time complexity: O(1) on average
		Exception safety: nothrow
		*/
		bool exists(const Integer& that) const
		{
			return elementSetSet_.front().count(that) > 0;
		}

		//! Returns a const-iterator to the smallest element.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		ConstIterator cbegin() const
		{
			return tree_.cbegin();
		}

		//! Returns the one-past-last const-iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		ConstIterator cend() const
		{
			return tree_.cend();
		}

	private:
		//! Finds the node with the longest common suffix.
		/*!
		Time complexity: O(log(log(N)))
		*/
		ConstIterator findLongestSuffix(const Integer& that) const
		{
						

			return node;
		}

		Node* root_;
		integer size_;
		ElementSetSet elementSetSet_;
	};

}

#endif
