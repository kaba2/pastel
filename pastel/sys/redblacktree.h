// Description: Red-black tree

#ifndef PASTELSYS_REDBLACKTREE_H
#define PASTELSYS_REDBLACKTREE_H

#include "pastel/sys/redblacktree_fwd.h"
#include "pastel/sys/redblacktree_concepts.h"
#include "pastel/sys/redblacktree_node.h"
#include "pastel/sys/redblacktree_iterator.h"

namespace Pastel
{

	template <typename Settings>
	using Empty_RedBlackTree_Customization =
		RedBlackTree_Concepts::Customization<Settings>;

	//! Red-black tree
	/*!
	Space complexity: O(n)
	where
	n is the number of stored elements.
	*/
	template <
		typename Settings_,
		typename Customization = Empty_RedBlackTree_Customization<Settings_>>
	class RedBlackTree
		: public Customization
	{
	public:
		// See redblacktree_fwd.h for the documentation
		// for the following types.
		using Settings = Settings_;
		using Fwd = RedBlackTree_Fwd<Settings>;

		PASTEL_FWD(Key);
		PASTEL_FWD(Data);
		PASTEL_FWD(Compare);

		PASTEL_FWD(Iterator);
		PASTEL_FWD(ConstIterator);
		PASTEL_FWD(Range);
		PASTEL_FWD(ConstRange);

		PASTEL_FWD(Key_Iterator);
		PASTEL_FWD(Key_ConstIterator);
		PASTEL_FWD(Key_Range);
		PASTEL_FWD(Key_ConstRange);

		PASTEL_FWD(Data_Iterator);
		PASTEL_FWD(Data_ConstIterator);
		PASTEL_FWD(Data_Range);
		PASTEL_FWD(Data_ConstRange);

		PASTEL_FWD(Data_Class);
		PASTEL_FWD(InsertReturnType);

		using iterator = Iterator;
		using const_iterator = ConstIterator;

		//! Constructs an empty tree.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		RedBlackTree();
		
		//! Copy-constructs from another tree.
		/*!
		Time complexity: O(that.size())
		Exception safety: strong
		*/
		RedBlackTree(const RedBlackTree& that);

		//! Move-constructs from another tree.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		RedBlackTree(RedBlackTree&& that)
			: RedBlackTree()
		{
			swap(that);
		}

		//! Constructs from a list of keys.
		/*!
		Time complexity: O(dataSet.size())
		Exception safety: strong

		The user-data will be default-initialized.
		*/
		template <typename That_Key>
		RedBlackTree(std::initializer_list<That_Key> dataSet)
		: RedBlackTree()
		{
			for (auto&& key : dataSet)
			{
				insert(key);
			}
		}

		//! Constructs from a list of key-value pairs.
		/*!
		Time complexity: O(dataSet.size())
		Exception safety: strong
		*/
		RedBlackTree(std::initializer_list<std::pair<Key, Data_Class>> dataSet)
		: RedBlackTree()
		{
			for (auto&& keyValue : dataSet)
			{
				insert(keyValue.first, keyValue.second);
			}
		}

		//! Assigns from another tree.
		/*!
		Time complexity: move/copy-construction
		Exception safety: strong

		Note that cend() is not preserved.
		*/
		RedBlackTree& operator=(RedBlackTree that)
		{
			swap(that);
			return *this;
		}

		//! Destructs the tree.
		/*!
		Time complexity: O(size())
		Exception safety: nothrow
		*/
		~RedBlackTree();

		//! Swaps two trees.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void swap(RedBlackTree& that);

		//! Removes all elements from the tree.
		/*!
		Time complexity: O(size()) + onClear()
		Exception safety: nothrow
		*/
		void clear();

		//! Returns the number of elements in the tree.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer size() const
		{
			return size_;
		}

		//! Returns true if the tree is empty.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		bool empty() const
		{
			return size_ == 0;
		}

		//! Inserts an element into the tree.
		/*!
		Time complexity: O(log(size())) * updateHierarchicalData() + onInsert()
		Exception safety: strong + onInsert()

		returns:
		If multiple keys are allowed, an iterator to the new element.
		Otherwise an iterator-bool pair, where the boolean tells whether
		the element was inserted. In case the element is not inserted,
		the iterator points to the existing equivalent element.
		*/
		template <typename... Value>
		InsertReturnType insert(
			Key key, Value&&... value);

		//! Inserts elements into the tree.
		/*!
		Time complexity: insert() * std::distance(begin, end)
		Exception safety: basic
		*/
		template <typename Key_ConstIterator_>
		void insertMany(
			Key_ConstIterator_ begin,
			Key_ConstIterator_ end);

		//! Removes an element from the tree by its iterator.
		/*!
		Time complexity: O(log(size())) * updateHierarchicalData() + onErase()
		Exception safety: nothrow

		If that == cend(), then nothing happens. This is
		so that erase(find(key)) works even if the key is
		not stored in the tree.
		*/
		Iterator erase(const ConstIterator& that);

		//! Removes an element from the tree by its key.
		/*!
		This is a convenience function which calls
		erase(find(key)).
		*/
		Iterator erase(const Key& key);

		//! Splices an element from 'that' tree to this tree.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow

		The result of the splicing is the same as subsequent
		erase() and insert() calls, however, no constructors or 
		destructors are performed for the data.
		*/
		InsertReturnType splice(
			RedBlackTree& that, 
			const ConstIterator& thatFrom);

		//! Returns whether an element is contained in the tree.
		/*!
		This is a convenience function which returns
		find(key) != cend().
		*/
		bool exists(const Key& key) const
		{
			return find(key) != cend();
		}

		//! Searches for the first element with key == 'key'.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow

		If there are multiple elements equivalent to 'key',
		then the first of them is returned (same as lowerBound()).
		*/
		ConstIterator find(const Key& key) const;

		Iterator find(const Key& key)
		{
			return cast(addConst(*this).find(key));
		}

		//! Returns the elements equivalent to the given key.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow
		*/
		std::pair<ConstIterator, ConstIterator> 
			equalRange(const Key& key) const
		{
				return equalRange(key, Both);
		}

		std::pair<ConstIterator, ConstIterator> 
			equal_range(const Key& key) const
		{
			return equalRange(key);
		}

		//! Returns the elements equivalent to the given key.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow
		*/
		std::pair<Iterator, Iterator> 
			equalRange(const Key& key)
		{
			auto lowerUpper = addConst(*this).equalRange(key);
			return std::make_pair(
				cast(lowerUpper.first), 
				cast(lowerUpper.second));
		}

		std::pair<Iterator, Iterator> 
			equal_range(const Key& key)
		{
			return equalRange(key);
		}

		//! Searches for the first element with key >= 'key'.
		/*!
		This is a convenience function which calls
		equalRange(key).first
		*/
		ConstIterator lowerBound(const Key& key) const
		{
			return equalRange(key, OnlyLowerBound).first;
		}

		ConstIterator lower_bound(const Key& key) const
		{
			return lowerBound(key);
		}

		Iterator lowerBound(const Key& key)
		{
			return cast(addConst(*this).lowerBound(key));
		}

		Iterator lower_bound(const Key& key)
		{
			return lowerBound(key);
		}

		//! Searches for the first element with key > 'key'.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow
		*/
		ConstIterator upperBound(const Key& key) const
		{
			return equalRange(key, OnlyUpperBound).second;
		}

		ConstIterator upper_bound(const Key& key) const
		{
			return upperBound(key);
		}

		Iterator upperBound(const Key& key)
		{
			return cast(addConst(*this).upperBound(key));
		}

		Iterator upper_bound(const Key& key)
		{
			return upperBound(key);
		}

		//! Casts away iterator constness.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Iterator cast(const ConstIterator& that)
		{
			return Iterator((Node*)that.base());			
		}

		//! Returns the iterator to the smallest element.
		PASTEL_ITERATOR_FUNCTIONS(begin, minimum PASTEL_CALL_BRACKETS);

		//! Returns the iterator to the one-past-greatest element.
		PASTEL_ITERATOR_FUNCTIONS(end, (Node*)sentinel_);

		//! Returns an iterator range.
		PASTEL_RANGE_FUNCTIONS(range, begin, end);

		//! Returns the iterator to the smallest element.
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Key_, keyBegin, minimum PASTEL_CALL_BRACKETS);

		//! Returns the iterator to the one-past-greatest element.
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Key_, keyEnd, (Node*)sentinel_);

		//! Returns an iterator range.
		PASTEL_RANGE_FUNCTIONS_PREFIX(Key_, keyRange, keyBegin, keyEnd);

		//! Returns the iterator to the smallest element.
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Data_, dataBegin, minimum PASTEL_CALL_BRACKETS);

		//! Returns the iterator to the one-past-greatest element.
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Data_, dataEnd, (Node*)sentinel_);

		//! Returns an iterator range.
		PASTEL_RANGE_FUNCTIONS_PREFIX(Data_, dataRange, dataBegin, dataEnd);

		//! Returns the iterator to the greatest element.
		PASTEL_ITERATOR_FUNCTIONS(last, maximum PASTEL_CALL_BRACKETS);

		//! Returns the iterator to the root element.
		/*!
		See the documentation for the const version.
		*/
		PASTEL_ITERATOR_FUNCTIONS(root, root_);

	private:
		PASTEL_FWD(Node_Base);
		PASTEL_FWD(Node);

		enum
		{
			Left = 0,
			Right = 1
		};

		enum EqualRange
		{
			OnlyLowerBound,
			OnlyUpperBound,
			Both
		};

		struct FindInsert
		{
			ConstIterator parent;
			bool rightChild;
			ConstIterator upper;
		};

		template <typename Type, bool MultipleKeys>
		class As_InsertReturnType
		{
		public:
			Type operator()(
				const Iterator& that, bool success) const
			{
				return that;
			}
		};

		template <typename Type>
		class As_InsertReturnType<Type, false>
		{
		public:
			Type operator()(
				const Iterator& that, bool success) const
			{
				return Type(that, success);
			}
		};

		InsertReturnType insertReturnType(
			const Iterator& that, bool success) const
		{
			return As_InsertReturnType<InsertReturnType, Settings::MultipleKeys>()(that, success);
		}

		//! Initializes some member variables.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void initialize();

		//! Allocates a data-node, and increases the size.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		template <typename... Value>
		Node* allocateNode(
			Key&& key,
			Value&&... value);

		//! Deallocates a data-node, and decreases the size.
		/*!
		Preconditions:
		!node->isSentinel()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void deallocateNode(Node* node);

		//! Finds the node under which to insert the key.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow

		If there are multiple equivalent elements, then
		the insertion position is chosen as the last in 
		the in-order sequence of equivalent keys. Thus
		the insertion always retains the order in which
		the equivalent keys were added.

		Note that the inserting the key under the returned
		node can break the red-black invariants.

		returns:
		A pair of a node and a boolean, with the first being
		the node under which to insert the key subject only to
		the binary search tree property, and the second being 
		whether to insert under the right child of that node or 
		not. 
		*/
		FindInsert findInsertParent(
			const Key& key, 
			const ConstIterator& node) const;

		//! Attaches a new node into the tree and rebalances.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow

		Preconditions:
		node is not attached to any tree

		node:
		The node to attach into the tree.

		parent, rightChild:
		The initial attachment position as provided
		by findInsertParent().
		*/
		void attach(
			Node* node,
			Node* parent,
			bool rightChild);

		//! Detaches a node from the tree and rebalances.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow

		returns:
		The successor of the detached node.
		*/
		Node* detach(Node* node);

		//! Returns the elements equivalent to the given key.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow
		*/
		std::pair<ConstIterator, ConstIterator>
			equalRoot(const Key& key) const;

		//! Returns the elements equivalent to the given key.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow
		*/
		std::pair<ConstIterator, ConstIterator>
			equalRange(const Key& key, EqualRange compute) const;

		//! Rebalances the red-black tree after detaching a node.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow
		*/
		void rebalanceAfterDetach(
			Node* toRebalance, 
			bool leftLowOnBlack);

		//! Updates hierarhical data on the path to root.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow

		Calls updateHierarhical(node) for each node in 
		the path to the root, including 'node' itself.
		*/
		void updateToRoot(Node* node);

		//! Copy-constructs a subtree.
		/*!
		Exception safety:
		strong, except for not deallocating
		memory in 'allocator', which must be
		done later.
		*/
		Node* copyConstruct(
			Node* parent,
			const RedBlackTree& that,
			Node* thatNode);

		//! Destructs the nodes of a subtree.
		/*!
		Time complexity: O(n)
		where 'n' is the number of nodes in the subtree
		rooted at 'node'.

		Exception safety: nothrow
		*/
		void clear(Node* node);

		//! Links a parent and a child together.
		/*!
		Preconditions:
		d == Left || d == Right

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void link(Node* parent, Node* child, 
			integer direction);

		//! Tree-rotation
		/*!
		Preconditions:
		d == Left || d == Right

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Node* rotate(Node* node, integer direction);

		//! Flips the colors of a node and its children.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void flipColors(Node* node);

		//! Sets the minimum node.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void setMinimum(Node* node)
		{
			minimum_ = node;
		}

		//! Returns the minimum node.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Node* minimum() const
		{
			return minimum_;
		}

		//! Sets the maximum node.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void setMaximum(Node* node)
		{
			sentinel_->parent() = node;
		}

		//! Returns the maximum node.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Node* maximum() const
		{
			return sentinel_->parent();
		}

		//! The root-node of the tree.
		Node* root_;

		//! The sentinel-node of the tree.
		/*!
		The children of leaf nodes and the parent of the
		root node are set to the sentinel node. 
		This simplifies the implementation by guaranteeing 
		that for any node the children and
		the parent exists. It also works as the
		one-past-last node (end() iterator).
		To make it possible to decrement an iterator
		from end(), the parent of the sentinel node
		is the maximum element, and the left child
		is the sentinel node itself (a unique property
		which can be used to identify it).
		The right child is also the sentinel itself.
		*/
		Node_Base* sentinel_;

		//! The minimum node of the tree.
		Node* minimum_;

		//! The number of stored elements in the tree.
		integer size_;
	};

}

namespace Pastel
{

	//! Returns whether the red-black tree invariants hold for the tree.
	/*!
	Time complexity: O(tree.size())
	Exception safety: nothrow

	This function is useful only for testing. For a correct implementation
	this function will always return true.
	*/
	template <typename Settings, typename Customization>
	bool testInvariants(const RedBlackTree<Settings, Customization>& tree);

}

// Map

namespace Pastel
{

	template <
		typename Key_, 
		typename Data_,
		typename Compare_ = LessThan,
		integer DereferenceType_ = RedBlackTree_Dereference_Default,
		bool MultipleKeys_ = false>
	class RedBlack_Settings
	{
	public:
		using Key = Key_;
		using Data = Data_;
		using Compare = Compare_;
		enum
		{
			DereferenceType = DereferenceType_
		};
		enum {MultipleKeys = MultipleKeys_};
	};

	template <
		typename Key, 
		typename Data,
		typename Compare = LessThan,
		integer DereferenceType_ = RedBlackTree_Dereference_Default,
		typename Customization = Empty_RedBlackTree_Customization<
		RedBlack_Settings<Key, Data, Compare, DereferenceType_, false>> >
	using RedBlack_Map = 
		RedBlackTree<RedBlack_Settings<Key, Data, Compare, DereferenceType_, false>, Customization>;

	template <
		typename Key, 
		typename Data,
		typename Compare = LessThan,
		integer DereferenceType_ = RedBlackTree_Dereference_Default,
		typename Customization = Empty_RedBlackTree_Customization<
		RedBlack_Settings<Key, Data, Compare, DereferenceType_, true>> >
	using RedBlack_MultiMap = 
		RedBlackTree<RedBlack_Settings<Key, Data, Compare, DereferenceType_, true>, Customization>;

}

// Set

namespace Pastel
{

	template <
		typename Key, 
		typename Compare = LessThan,
		integer DereferenceType_ = RedBlackTree_Dereference_Default,
		typename Customization = Empty_RedBlackTree_Customization<
		RedBlack_Settings<Key, void, Compare, DereferenceType_, false>>>
	using RedBlack_Set = 
		RedBlackTree<RedBlack_Settings<Key, void, Compare, DereferenceType_, false>, Customization>;

	template <
		typename Key, 
		typename Compare = LessThan,
		integer DereferenceType_ = RedBlackTree_Dereference_Default,
		typename Customization = Empty_RedBlackTree_Customization<
		RedBlack_Settings<Key, void, Compare, DereferenceType_, true>>>
	using RedBlack_MultiSet = 
		RedBlackTree<RedBlack_Settings<Key, void, Compare, DereferenceType_, true>, Customization>;

}

#include "pastel/sys/redblacktree.hpp"
#include "pastel/sys/redblacktree_copy.hpp"
#include "pastel/sys/redblacktree_erase.hpp"
#include "pastel/sys/redblacktree_insert.hpp"
#include "pastel/sys/redblacktree_private.hpp"
#include "pastel/sys/redblacktree_search.hpp"
#include "pastel/sys/redblacktree_invariants.hpp"

#endif
