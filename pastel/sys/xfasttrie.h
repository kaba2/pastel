// Description: X-fast trie

#ifndef PASTELSYS_XFASTTRIE_H
#define PASTELSYS_XFASTTRIE_H

#include "pastel/sys/xfasttrie_concepts.h"
#include "pastel/sys/xfasttrie_node.h"
#include "pastel/sys/integer.h"
#include "pastel/sys/object_forwarding.h"

#include <list>
#include <unordered_map>
#include <vector>

namespace Pastel
{

	//! X-fast trie
	template <typename XFastTrie_Settings>
	class XFastTrie
	{
	public:
		using Settings = XFastTrie_Settings;

		enum
		{
			Bits = Settings::Bits
		};
		PASTEL_STATIC_ASSERT(Bits > 0);

		using Key = Integer<Bits>;
		using Value = typename Settings::Value;
		using Value_Class = typename AsClass<Value>::type;

		/*!
		We are making use of the empty base-class optimization,
		so that if Value = void, then the value-data will not
		use any memory.
		*/
		class Data
		: public Value_Class
		{
		public:
			explicit Data(Key key, Value_Class value)
			: Value_Class(std::move(value))
			, key_(key)
			{
			}

			const Key& key() const
			{
				return key_;
			}

		private:
			// The key must never be overwritten.
			Data& operator=(Data) = delete;

			Key key_;
		};

		using DataSet = std::list<Data>;
		using ConstIterator = typename DataSet::const_iterator;
		using Iterator = typename DataSet::iterator;

		using const_iterator = ConstIterator;
		using iterator = Iterator;

		using Node = XFastTrie_::Node<Key, Iterator>;

		using NodeSet = std::unordered_map<Key, Node>;
		using Node_ConstIterator = typename NodeSet::const_iterator;
		using Node_Iterator = typename NodeSet::iterator;

		using NodeSetSet = std::vector<NodeSet>;

		//! Constructs an empty x-fast trie.
		/*!
		Time complexity: O(endBit - beginBit)
		Exception safety: strong
		*/
		explicit XFastTrie(
			integer beginBit = 0, 
			integer endBit = Bits)
		: beginBit_(beginBit)
		, endBit_(endBit)
		, nodeSetSet_()
		, dataSet_()
		{
			ENSURE_OP(0, <=, beginBit);
			ENSURE_OP(beginBit, <, endBit);
			ENSURE_OP(endBit, <=, Bits);

			initialize();
		}

		//! Destructs the trie.
		/*!
		Time complexity: O(size())
		Exception safety: nothrow
		*/
		~XFastTrie()
		{
			clear();
		}

		//! Swaps two tries.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void swap(XFastTrie& that)
		{
			using std::swap;

			swap(beginBit_, that.beginBit_);
			swap(endBit_, that.endBit_);
			nodeSetSet_.swap(nodeSetSet_);
			dataSet_.swap(that.dataSet_);
		}

		//! Removes all elements.
		/*!
		Time complexity: O(size())
		Exception safety: nothrow
		*/
		void clear()
		{
			// Clear all elements.
			dataSet_.clear();

			// Clear all nodes, except the root.
			for (integer i = 0;i < bits();++i)
			{
				nodeSetSet_[i].clear();
			}

			// Reset the root node.
			Node* root = &(nodeSetSet_.front().begin()->second);
			root->child_[0] = 0;
			root->child_[1] = 0;
			root->shortcut_ = 0;
			root->element_ = dataSet_.end();
		}

		//! Inserts an element.
		/*!
		Time complexity: O(log(log(bits())))
		Exception safety: strong

		returns:
		An iterator to the inserted element, if it did not
		exist before, or an iterator to the existing element,
		otherwise.
		*/
		Iterator insert(
			Key key, 
			Value_Class value = Value_Class())
		{
			// Find the subtree corresponding to the smallest
			// interval containing the key.
			std::pair<Node_Iterator, integer> result =
				lowestAncestor(key);

			Node_Iterator node = result.first;
			integer level = result.second;

			ASSERT_OP(beginBit_, <=, level);
			ASSERT_OP(level, <=, endBit_);

			if (level == beginBit_)
			{
				// The element already exists in the trie. 
				// Return an iterator to that element.
				return node->second.element_;
			}

			// The element does not exist in the trie.
			// We will add the nodes that lead to the new
			// element.

			// We will store the iterators to newly-created 
			// nodes in 'nodeSet', so that we can rollback 
			// in case of an exception.
			std::vector<Node_Iterator> nodeSet;
			
			// We preallocate the memory, so that push_back()
			// can not throw an exception.
			nodeSet.reserve(level - beginBit_);

			// The newly-created element will be stored here.
			Iterator element;

			try
			{
				// Create the new nodes.
				for (integer i = beginBit_;i < level;++i)
				{
					auto result = nodeSetSet_[i - beginBit_].insert(
						std::make_pair(key, Node()));
					ASSERT(result.second);
					nodeSet.push_back(result.first);
				}

				// Find where the new element should be
				// inserted in 'dataSet_'. The insertion
				// position is either the element of the
				// shortcut, or its successor.
				auto shortcut = node->second.shortcut();
				Iterator position = shortcut->element_;
				if (node->second.left())
				{
					ASSERT(!node->second.right());
					
					// The right child is missing.
					// Therefore, the shortcut points
					// to the largest element in the
					// 'node' subtree. Since the new
					// element is going to the right
					// child, we want to insert the
					// new element after the shortcut.

					// So go past the shortcut, unless
					// this is the first element in the
					// trie.
					if (position != end())
					{
						++position;
					}
				}
				else
				{
					ASSERT(!node->second.left());

					// The left child is missing.
					// Therefore, the shortcut points
					// to the smallest element in the
					// 'node' subtree. Since the new
					// element is going to the left
					// child, we want to insert the
					// new element before the shortcut.

					// So do nothing.
				}

				// Insert the new element to the appropriate
				// position.
				element = dataSet_.insert(
					position, Data(key, std::move(value)));
			}
			catch(...)
			{
				// Rollback for strong exception safety.
				for (integer i = 0;i < nodeSet.size();++i)
				{
					nodeSetSet_[i].erase(nodeSet[i]);
				}

				throw;
			}

			Node_Iterator leaf = nodeSet.front();

			// Link the newly created nodes together.
			for (integer i = beginBit_;i <= level;++i)
			{
				// At level i + 1 the i:th bit is used
				// to decide the child node. 
				bool branch = key.test(i - 1);

				// The nodeSet[0] has level 'beginBit_'.
				Node_Iterator parent = nodeSet[i + 1 - beginBit_];
				Node_Iterator child = nodeSet[i - beginBit_];

				parent->second.child(branch) = &child->second;
				parent->second.shortcut() = &leaf->second;
			}

			// Link the newly created sub-tree to the trie.
			{
				bool branch = key.test(level - 1);

				Node_Iterator parent = node;
				Node_Iterator child = nodeSet.back();

				parent->second.child(branch) = &child->second;
				if (!parent->second.child(!branch))
				{
					parent->second.shortcut() = &leaf->second;
				}
			}

			leaf->second.element_ = element;

			// Return an iterator to the element.
			return element;
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
		ConstIterator find(const Key& key) const
		{
			ConstIterator iter = lowestAncestor(key);
			if (!iter->element_ == end())
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

		Iterator find(const Key& key)
		{
			return cast(removeConst(*this).find(key));
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
		ConstIterator lowerBound(const Key& key) const
		{
			ConstIterator iter = lowestAncestor(key);
			if (iter->element_ == end())
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

		Iterator lowerBound(const Key& key)
		{
			return cast(removeConst(*this).lowerBound(key));
		}

		ConstIterator lower_bound(const Key& key) const
		{
			return lowerBound(key);
		}

		Iterator lower_bound(const Key& key)
		{
			return lowerBound(key);
		}

		//! Returns whether the element exists.
		/*!
		Time complexity: O(1) on average
		Exception safety: nothrow
		*/
		bool exists(const Key& key) const
		{
			return nodeSetSet_.front().count(
				Key(key, beginBit_, endBit_)) > 0;
		}

		//! Converts a const-iterator to an iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Iterator cast(const ConstIterator& that)
		{
			return dataSet_.erase(that, that);
		}

		//! Returns a const-iterator to the smallest element.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		ConstIterator cbegin() const
		{
			return dataSet_.cbegin();
		}

		Iterator begin()
		{
			return dataSet_.begin();
		}

		ConstIterator begin() const
		{
			return dataSet_.begin();
		}

		//! Returns the one-past-last const-iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		ConstIterator cend() const
		{
			return dataSet_.cend();
		}

		Iterator end()
		{
			return dataSet_.end();
		}

		ConstIterator end() const
		{
			return dataSet_.end();
		}

		//! Returns the number of used bits.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer bits() const
		{
			return endBit_ - beginBit_;
		}

		//! Returns the number of elements.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer size() const
		{
			return dataSet_.size();
		}

	private:
		void initialize()
		{
			/*
			Consider the following x-fast trie on
			2-bit integers.

			          -
			       /     \
			      0       1
	    		 / \     / \
	    	   00   01 10   11

	    	Then bits() = 2, and the height of the
	    	node tree is bits() + 1. This is
	    	also the general formula. 
			*/
			nodeSetSet_.resize(bits() + 1);

			// Create the root node.
			auto result = nodeSetSet_[bits()].insert(
				std::make_pair(Key(), Node()));

			// Suppose there are no elements in the trie.
			// Then the root node is the only node. We
			// make its shortcut point to the root node
			// itself, and make the element point to the
			// one-past-end of the 'dataSet_', so that the
			// insert() algorithm flows without special
			// cases.
			Node* root = &result.first->second;
			root->element_ = dataSet_.end();
			root->shortcut_ = root;
		}

		//! Returns the lowest ancestor node of the given key.
		/*!
		Time complexity: 
		O(log(Bits)) average, if the key does not exist in the trie,
		O(1) average, otherwise.
		
		Exception safety: 
		nothrow
		*/
		auto lowestAncestor(const Key& key)
		-> std::pair<Node_Iterator, integer>
		{
			// Check whether 'key' already exists in the trie.
			// If so, the result can be returned in O(1)
			// average time.
			{
				Node_Iterator iter = 
					nodeSetSet_.front().find(
						Key(key, beginBit_, endBit_));
				if (iter != nodeSetSet_.front().end())
				{
					return std::make_pair(iter, beginBit_);
				}
			}

			// We will be searching the key from the
			// level-search structures by doing a 
			// a binary search over the levels.
			Node_Iterator lowest;
			
			// We already know from above that the key is not 
			// contained at level 'beginBit_'.
			integer bottom = beginBit_ + 1;
			integer top = endBit_;
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
				
				// Find out whether the key-suffix can be
				// found at the 'middle' level.
				Node_Iterator iter =
					nodeSetSet_[middle - beginBit_].find(
						Key(key, middle, endBit_));
				if (iter == nodeSetSet_[middle - beginBit_].end())
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
					lowest = iter;
				}
			}
			
			// Since we already checked that the tree
			// is not empty, the lowest ancestor must
			// exist.

			// The node can not have both children;
			// otherwise it would not be the lowest 
			// ancestor.
			ASSERT(!lowest->second.splitsBoth());

			// Return the lowest ancestor.
			return std::make_pair(lowest, bottom);
		}

		//! The bits to use in a key.
		/*!
		Preconditions:
		0 <= beginBit_ < endBit_ <= Bits

		The indices of the bits to use are in the range 
		[beginBit_, endBit_).
		*/
		integer beginBit_;
		integer endBit_;

		//! Nodes stored in level-search structures.
		/*!
		Each node represents integers with a specified
		bit-suffix. The root node represents all 
		integers, since they all share the same empty 
		bit-suffix. The root node always exists.
		Other than the root, if a node exists, then it 
		is because there exists at least one element 
		in the trie with that suffix. The nodes form
		a binary tree with height bits() + 1. The leaf 
		nodes are all at level 0, and the root node is
		at level bits(). The elements in the subtree
		determined by a node a level i are required 
		to match in the [i, endBit_) index-range of bits. 
		Apart from actually storing the nodes at level 
		(beginBit_ + i), the nodeSetSet[i] can be used 
		to efficiently answer whether the trie contains a
		n element with a [beginBit_ + i, endBit_)-suffix.
		*/
		NodeSetSet nodeSetSet_;

		//! The elements stored in increasing order.
		DataSet dataSet_;
	};
	
}

namespace Pastel
{

	template <integer Bits_, typename Value_>
	class XFastTrie_Map_Settings
	{
	public:
		static const integer Bits = Bits_;
		using Value = Value_;
	};

	template <integer Bits, typename Value>
	using XFastTrie_Map = XFastTrie<XFastTrie_Map_Settings<Bits, Value>>;

	template <integer Bits> 
	using XFastTrie_Set_Settings = XFastTrie_Map_Settings<Bits, void>;

	template <integer Bits>
	using XFastTrie_Set = XFastTrie<XFastTrie_Set_Settings<Bits>>;

}

#endif
