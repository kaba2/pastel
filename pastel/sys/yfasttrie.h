// Description: Y-fast trie

#ifndef PASTELSYS_YFASTTRIE_H
#define PASTELSYS_YFASTTRIE_H

#include "pastel/sys/yfasttrie_concepts.h"
#include "pastel/sys/yfasttrie_node.h"
#include "pastel/sys/number_tests.h"
#include "pastel/sys/redblacktree.h"

#include <boost/integer/static_log2.hpp>

#include <bitset>
#include <array>
#include <list>

namespace Pastel
{

	class Default_YFastTrie_Settings
	{
	public:
		static const integer Bits = 64;
		using Value = void;
	};

	//! Y-fast trie
	/*!
	Preconditions:
	isPowerOfTwo(Bits).

	Bits:	
	The number of bits in a stored integer.

	References
	----------

	"Log-logarithmic Queries are Possible in Space Theta(Bits)",
	Dan E. Willard, Information Processing Letters 17 (1983),
	pp. 81-84.

	"Fast Local Searches and Updates in Bounded Universes",
	Prosenjit Bose et al., CCCG 2010 (2010).
	*/
	template <typename Settings = Default_YFastTrie_Settings>
	class YFastTrie
	{
	private:
		PASTEL_STATIC_ASSERT(Bits > 0);

		// FIX: Replace with isPowerOfTwo constexpr
		// after constexpr becomes available in
		// Visual Studio.
		enum
		{
			NIsPowerOfTwo = (Bits & (Bits - 1) == 0),
			LogBits = boost::static_log2<Bits>::value,
			M = Bits - LogBits
		};
		PASTEL_STATIC_ASSERT(NIsPowerOfTwo);
		// PASTEL_STATIC_ASSERT(isPowerOfTwo(Bits));

		using Node = YFastTrie_::Node;
		using Leaf_Node = YFastTrie_::Leaf_Node;
		using Split_Node = YFastTrie_::Split_Node;

		using Trie_Iterator = YFastTrie_::Trie_Iterator;
		using Trie_ConstIterator = YFastTrie_::Trie_ConstIterator;

		using Integer = Pastel::Integer<Bits>;
		using IntegerSet = std::unordered_map<Integer, Node*>;
		using IntegerSet_Iterator = typename IntegerSet::iterator;
		using IntegerSet_ConstIterator = typename IntegerSet::const_iterator;

		using Bucket = Map<Integer, Value>;
		using Bucket_ConstIterator = typename IntegerTree::ConstIterator;
		using Bucket_Iterator = typename IntegerTree::Iterator;

		using BucketSet = std::list<Bucket>;
		using BucketSet_ConstIterator = typename BucketSet::ConstIterator;
		using BucketSet_Iterator = typename BucketSet::Iterator;

		using ElementSetSet = std::array<IntegerSet, M>;

	public:
		YFastTrie()
		: root_(0)
		, size_(0)
		, nodes_(0)
		, elementSetSet_()
		{
			initialize();
		}

		//! Swaps two tries.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void swap(YFastTrie& that)
		{
			using std::swap;
			swap(root_, that.root_);
			swap(size_, that.size_);
			swap(nodes_, that.nodes_);
			elementSetSet_.swap(elementSetSet_);
		}

		//! Inserts an element.
		/*!
		Time complexity: O(log(log(Bits)))
		Exception safety: strong

		returns:
		The leaf node which contains the bucket 
		which contains the element.
		*/
		Trie_ConstIterator insert(
			Integer key, 
			Value_Class value = Value_Class())
		{
			// Find the subtree corresponding to the smallest
			// interval containing the key.
			std::pair<Trie_ConstIterator, integer> result =
				lowestAncestor(key);

			Node* node = result.first.base();
			integer level = result.second;
			if (level > LogBits)
			{
				// The bucket that would contain the key does 
				// not exist in the trie. We will add the bucket
				// and the split nodes which lead to that bucket.

				// Create a leaf node (bucket) at level 'LogBits'.
				// Note that the destructor of Node is virtual, so
				// that the potential destruction works correctly here.
				nodeSet.push_back(std::unique_ptr<Node>(new LeafNode));
				Node* leaf = nodeSet.back().get();

				// Create the split nodes at the level range
				// [LogBits + 1, level).
				std::vector<std::unique_ptr<Node>> nodeSet;
				nodeSet.reserve(level - LogBits);
				for (integer i = LogBits + 1;i < level;++i)
				{
					nodeSet.push_back(std::unique_ptr<Node>(new Node));
				}

				// Add the element into the bucket.
				leaf->map_.insert(std::move(key), std::move(value));

				// Link the newly created nodes together.
				for (integer i = LogBits + 1;i < level;++i)
				{
					// At level i the (i - 1):th bit is used
					// to decide the child node. The nodeSet[0]
					// has level 'LogBits'.
					bool branch = key.test(i - 1);

					Node* parent = nodeSet[i - LogBits];
					Node* child = nodeSet[i - LogBits - 1];

					parent->child(branch) = child;
					parent->shortcut() = leaf;
					child->parent() = parent;
				}

				// Link the newly created sub-tree to the trie.
				{
					bool branch = key.test(level - 1);

					Node* parent = node;
					Node* child = nodeSet.back().get();

					parent->child(branch) = child;
					if (!parent->child(!branch))
					{
						parent->shortcut() = leaf;
					}
					child->parent() = parent;
				}

				// Transfer the ownership of the new nodes
				// to the trie.
				for (integer i = 0;i < nodeSet.size();++i)
				{
					nodeSet[i].release();
				}

				// Continue as if the leaf node existed
				// from the start.
				node = leaf;
			}
			else
			{
				// The bucket to contain the key already exists
				// in the trie. We only need to add the key into
				// the bucket.

				Leaf_Node* leaf = (Leaf_Node*)node;

				// Add the element into the bucket.
				leaf->map_.insert(std::move(key), std::move(value));
			}

			// Return an iterator into the bucket.
			return Trie_ConstIterator(node);
		}

		//! Returns the bucket of a leaf node.
		/*!
		Time complexity: O(1)
		Exceptions safety: nothrow

		Preconditions: leaf.isLeaf()
		*/
		const Bucket& bucket(const Trie_ConstIterator& leaf) const
		{
			ENSURE(leaf.isLeaf());
			Leaf_Node* leaf = (Leaf_Node*)leaf.base();
			return leaf->map_;
		}

		//! Returns the bucket, if it exists, that spans the given key.
		/*!
		Time complexity: O(1)
		Exceptions safety: nothrow

		Preconditions: leaf.isLeaf()

		returns:
		The bucket that spans the given key, if it exists,
		cend(), otherwise.
		*/
		const Bucket& bucket(const Integer& key) const
		{

		}

		//! Returns an iterator to the bucket spanning the key, if it exists.
		/*!
		Time complexity: 
		O(log(log(Bits))).

		Exception safety:
		nothrow

		returns:
		An iterator to the element, if the bucket exists,
		cend(), otherwise.
		*/
		Trie_ConstIterator find(const Integer& key) const
		{
			Trie_ConstIterator iter = lowestAncestor(key);
			if (!iter.isLeaf())
			{
				// If the lowest ancestor of this key is 
				// not a leaf node, then the bucket is not
				// stored in the trie. 
				return cend();
			}

			if (bucket(iter)->count(key) == 0)
			{
				// The key is
				return cend();
			}

			return iter;
		}

		//! Returns an iterator to the first element >= 'key'.
		/*!
		Time complexity: 
		O(log(log(Bits)))

		Exception safety:
		nothrow

		returns:
		An iterator to the element, if the element exists,
		cend(), otherwise.
		*/
		Trie_ConstIterator lowerBound(const Integer& key) const
		{
			Trie_ConstIterator iter = lowestAncestor(key);
			if (iter.isLeaf())
			{
				// Since the lowest ancestor is a leaf node,
				// the element is in the trie. Return the
				// containing bucket.
				return iter;
			}

			// If the lowest ancestor is not a leaf node,
			// then the element is not stored in the trie.
			// In addition, the lowest ancestor must have 
			// exactly one child; otherwise it would not be
			// the lowest.

			if (iter.right().empty())
			{
				// It is the right child which is missing.
				// Therefore the 'shortcut' node contains
				// the node of the predecessor bucket.
				// Return the successor of the predecessor
				// bucket.

				return iter.shortcut().next();
			}

			// It is the left child which is missing.
			// Therefore the 'shortcut' node contains
			// the node of the successor bucket.
			return iter.shortcut();
		}

		//! Returns an iterator to the first element >= 'key'.
		/*!
		This is a convenience function which calls
		lowerBound(key).
		*/
		Trie_ConstIterator lower_bound(const Integer& key) const
		{
			return lowerBound(key);
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
		Trie_ConstIterator cbegin() const
		{
			return tree_.cbegin();
		}

		//! Returns the one-past-last const-iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Trie_ConstIterator cend() const
		{
			return tree_.cend();
		}

		//! Returns the lowest ancestor node of the given key.
		/*!
		Time complexity: 
		O(log(Bits)) average, if the key does not exist in the trie,
		O(1) average, otherwise.
		
		Exception safety: 
		nothrow
		*/
		auto lowestAncestor(const Integer& key) const
		 -> std::make_pair<Trie_ConstIterator, integer>
		{
			// Check whether 'key' already exists in the trie.
			// In this case we are able to return the result in O(1)
			// average time.
			{
				Integer keySuffix(key >> LogBits);

				IntegerSet_ConstIterator iter = 
					elementSetSet_.front().find(keySuffix);
				if (iter != cend())
				{
					return std::make_pair(iter, LogBits);
				}
			}

			// We will be searching the key from the
			// level-search structures by doing a 
			// a binary search over the levels.
			Node* lowest = 0;
			
			// Since this is a y-fast trie, the lowest log(Bits) 
			// levels are not included in the trie. In addition,
			// we already know from above that the key is not 
			// contained at level LogBits.
			integer bottom = LogBits + 1;
			integer top = Bits;
			while (bottom != top)
			{
				// Pick the middle level from the level
				// range [bottom, top). Note that the 
				// implicit rounding-towards-zero with integer
				// division is the correct thing to do
				// with such a positive half-open interval;
				// the 'middle' can equal 'bottom', but
				// can not equal 'top'.
				integer middle = (bottom + top) / 2;
				
				// Find out the suffix of the key at 
				// the 'middle' level. For example,
				// at level zero the whole key would
				// be the suffix, while at the (Bits - 1):th
				// level only the last bit would be the 
				// suffix.
				Integer keySuffix(key >> middle);

				// Find out whether the key-suffix can be
				// found at the 'middle' level.
				IntegerSet_ConstIterator iter =
					elementSetSet_[middle - LogBits].find(keySuffix);
				if (iter == elementSetSet_.cend())
				{
					// The suffix was not found at this level.
					// It follows that longer suffixes won't be 
					// found at lower levels either. Continue at
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
			// exist.
			ASSERT(lowest);

			// The node can not have both children;
			// otherwise it would not be the lowest 
			// ancestor.
			ASSERT(!lowest->splitsBoth());

			// Return the lowest ancestor.
			return std::make_pair(Trie_ConstIterator(lowest), bottom);
		}

	private:
		void initialize()
		{
			root_ = new Node;
		}

		void deinitialize()
		{
			delete root_;
		}

		//! The root node of the trie.
		/*!
		The root node represents all the elements in 
		the trie. That is, those elements which have
		the empty sequence as a suffix.
		*/
		Node* root_;

		//! The number of elements stored in the trie.
		integer size_;

		//! The number of nodes in the trie.
		integer nodes_;

		//! Level-search structures.
		/*!
		The nodes at the (LogBits + i):th level of the trie
		are stored in elementSetSet_[i]; lower levels are
		not stored at all in the trie. The elementSetSet_[i] 
		is a hash-table which allows to efficiently find 
		whether a [LogBits + i, Bits)-suffix of the searched key
		is present at the i:th level of the trie.
		*/
		ElementSetSet elementSetSet_;
	};

}

{

	template <
		integer N_, 
		typename Value_>
	class YFastTrie_Map_Settings
	{
	public:
		static const integer Bits = N_;
		using Value = Value_;
	};

	template <
		integer Bits, 
		typename Value = void>
	using YFastTrie_Map = YFastTrie<YFastTrie_Map_Settings<Bits, Value>>;

	template <integer Bits>
	using YFastTrie_Set = YFastTrie<YFastTrie_Map_Settings<Bits, void>>;

}

#endif
