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
			explicit Data(Value_Class value)
			: Value_Class(std::move(value))
			{
			}

			const Key& key() const
			{
				return key_;
			}

		private:
			// The key must never be overwritten.
			Data& operator=(Data) PASTEL_DELETE;

			Key key_;
		};

		using DataSet = std::list<Data>;
		using ConstIterator = typename DataSet::const_iterator;
		using Iterator = typename DataSet::iterator;

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
		, root_(0)
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
			deinitialize();
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
			swap(root_, that.root_);
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
			// We preserve beginBit_, endBit_,
			// and root_.

			clearNode(root_->left());
			clearNode(root_->right());

			root_->child_[0] = 0;
			root_->child_[1] = 0;

			dataSet_.clear();
			for (integer i = beginBit_;i < endBit_;++i)
			{
				nodeSetSet_[i].clear();
			}
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

			ASSERT_OP(level, >=, beginBit_);
			ASSERT_OP(level, <, endBit_);

			if (level == beginBit_)
			{
				// The element already exists in the trie. 
				// Return an iterator to that element.
				return node->second.element_;
			}

			// The element does not exist in the trie.
			// We will add the nodes that lead to the new
			// element.

			std::vector<Node_Iterator> nodeSet;
			nodeSet.reserve(level - beginBit_);
			Iterator element;

			try
			{
				for (integer i = beginBit_;i < level;++i)
				{
					auto result = nodeSetSet_[i - beginBit_].insert(
						std::make_pair(key, Node()));
					ASSERT(result.second);
					nodeSet.push_back(result.first);
				}

				auto shortcut = node->second.shortcut();
				ASSERT(shortcut != nodeSetSet_[level].end());
				
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
					position, Data(std::move(value)));
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
				child->second.parent() = &parent->second;
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
				child->second.parent() = &parent->second;
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

	    	Then bits() = 2, log(bits()) = 1, and the number
	    	of search levels is log(bits()) + 1. This is
	    	also the general formula. 
			*/
			integer levels = (integer)std::log((real)bits()) + 1;
			nodeSetSet_.resize(levels);

			root_ = new Node;
		}

		void deinitialize()
		{
			delete root_;
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
			// In this case we are able to return the result in O(1)
			// average time.
			{
				Key keySuffix(key, beginBit_, endBit_);

				Node_Iterator iter = 
					nodeSetSet_.front().find(keySuffix);
				if (iter != nodeSetSet_.front().cend())
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
				
				// Find out the suffix of the key at 
				// the 'middle' level. For example,
				// at level zero the whole key would
				// be the suffix, while at the (Bits - 1):th
				// level only the last bit would be the 
				// suffix.
				Key keySuffix(key, middle, endBit_);

				// Find out whether the key-suffix can be
				// found at the 'middle' level.
				Node_Iterator iter =
					nodeSetSet_[middle - beginBit_].find(keySuffix);
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
			ASSERT(lowest);

			// The node can not have both children;
			// otherwise it would not be the lowest 
			// ancestor.
			ASSERT(!lowest->second.splitsBoth());

			// Return the lowest ancestor.
			return std::make_pair(lowest, bottom);
		}

		void clearNode(Node* node)
		{
			if (!node)
			{
				return;
			}

			if (!node->isLeaf())
			{
				clearNode(node->left());
				clearNode(node->right());
			}

			delete node;
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

		//! The root node of the trie.
		/*!
		The root node represents all the elements in 
		the trie. That is, those elements which have
		the empty sequence as a suffix.
		*/
		Node* root_;

		//! Node-search structures.
		/*!
		There are as many levels as there are bits().
		The nodeSetSet[i] can be used to efficiently
		answer whether the trie contains an element
		with a [beginBit_ + i, endBit_)-suffix.
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
