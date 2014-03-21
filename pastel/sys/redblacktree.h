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
	Space complexity: O(size())
	*/
	template <
		typename Settings_,
		template <typename> class Customization_ = Empty_RedBlackTree_Customization>
	class RedBlackTree
		: public Customization_<Settings_>
	{
	public:
		// See redblacktree_fwd.h for the documentation
		// for the following types.
		using Settings = Settings_;
		using Fwd = RedBlackTree_Fwd<Settings>;
		using Customization = Customization_<Settings_>;

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

		PASTEL_FWD(Insert_Return);
		PASTEL_FWD(FindEqual_Return);
		PASTEL_FWD(FindInsert_Return);

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

		//! Returns the black-height of the tree.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		
		In a red-black tree, the paths from a given node to
		its leaves have the same number of black nodes,
		which is called the black-height of the node. The
		black-height of the tree is the black-height of the
		root. 
		*/
		integer blackHeight() const
		{
			return blackHeight_;
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
		Insert_Return insert(
			Key key, Value&&... value);

		//! Removes an element from the tree by its iterator.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow

		If that == cend(), then nothing happens. This is
		so that erase(find(key)) works even if the key is
		not stored in the tree.

		returns:
		std::next(that)
		*/
		Iterator erase(const ConstIterator& that);

		//! Removes elements in a range.
		/*!
		Time complexity: O(k log(size()))
		where
		k is the number of removed elements.

		Exception safety: nothrow

		returns:
		cast(range.end())
		*/
		Iterator erase(const ConstRange& range);

		//! Removes elements in a range.
		/*!
		This is a convenience function which calls
		erase(ConstRange(begin, end)).
		*/
		Iterator erase(
			const ConstIterator& begin,
			const ConstIterator& end);

		//! Removes all elements equivalent to the key.
		/*!
		This is a convenience function which calls
		erase(equalRange(key)).
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
		Insert_Return splice(
			RedBlackTree& that, 
			const ConstIterator& thatFrom);

		//! Returns whether an element is contained in the tree.
		/*!
		This is a convenience function which returns
		findEqual(key) != cend().
		*/
		bool exists(const Key& key) const
		{
			return findEqual(key) != cend();
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

		//! Returns the top-most element equivalent to the key.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow

		Equivalently, this is the first element equivalent to
		the key when traversing the tree in pre-order. This
		is the fastest way to find some equivalent element.
		*/
		ConstIterator findEqual(const Key& key) const
		{
			return findEqualAndUpper(key).equal;
		}

		Iterator findEqual(const Key& key)
		{
			return cast(addConst(*this).findEqual(key));
		}

		//! Finds the top-most element equivalent to key, and an upper bound.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow

		The upper bound is the smallest upper-bound located above the
		top-most element.
		*/
		FindEqual_Return findEqualAndUpper(const Key& key) const;

		//! Finds the node under which to insert the key.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow

		If there are multiple equivalent elements, then
		the insertion position is chosen as the last in 
		the in-order sequence of equivalent keys. Thus
		the insertion always retains the order in which
		the equivalent keys were added.

		returns:
		See the documentation for FindInsert_Return
		in redblacktree_fwd.h
		*/
		FindInsert_Return findInsert(
			const Key& key, 
			const FindEqual_Return& equalRoot) const;

		//! Returns the elements equivalent to the given key.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow
		*/
		ConstRange equalRange(const Key& key) const
		{
			return equalRange(key, findEqualAndUpper(key), Both).range;
		}

		std::pair<ConstIterator, ConstIterator> 
			equal_range(const Key& key) const
		{
			ConstRange range = equalRange(key).range;
			return std::make_pair(range.begin(), range.end());
		}

		//! Returns the elements equivalent to the given key.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow
		*/
		Range equalRange(const Key& key)
		{
			return cast(addConst(*this).equalRange(key));
		}

		std::pair<Iterator, Iterator> 
			equal_range(const Key& key)
		{
			Range range = equalRange(key);
			return std::make_pair(range.begin(), range.end());
		}

		//! Return the first element with key >= 'key'.
		/*!
		This is a convenience function which calls
		equalRange(key).first
		*/
		ConstIterator lowerBound(const Key& key) const
		{
			return equalRange(key, findEqualAndUpper(key), OnlyLowerBound).range.begin();
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

		//! Returns the first element with key >= 'key'.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow
		*/
		ConstIterator lowerBound(
			const Key& key,
			const FindEqual_Return& equalAndUpper) const
		{
			return equalRange(key, equalAndUpper, OnlyLowerBound).range.begin();
		}

		Iterator lowerBound(
			const Key& key,
			const FindEqual_Return& equalAndUpper)
		{
			return cast(addConst(*this).lowerBound(key, equalAndUpper));
		}

		//! Searches for the first element with key > 'key'.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow
		*/
		ConstIterator upperBound(const Key& key) const
		{
			return equalRange(key, findEqualAndUpper(key), OnlyUpperBound).range.end();
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

		//! Returns the first element with key > 'key'.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow
		*/
		ConstIterator upperBound(
			const Key& key,
			const FindEqual_Return& equalAndUpper) const
		{
			return equalRange(key, equalAndUpper, OnlyUpperBound).range.end();
		}

		Iterator upperBound(
			const Key& key,
			const FindEqual_Return& equalAndUpper)
		{
			return cast(addConst(*this).upperBound(key, equalAndUpper));
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

		//! Casts away range constness.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Range cast(const ConstRange& that)
		{
			return Range(
				cast(that.begin()),
				cast(that.end()));			
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
		PASTEL_ITERATOR_FUNCTIONS(root, rootNode PASTEL_CALL_BRACKETS);

	private:
		PASTEL_FWD(Node_Base);
		PASTEL_FWD(Node);

		enum EqualRange
		{
			OnlyLowerBound,
			OnlyUpperBound,
			Both
		};

		struct EqualRange_Return
		{
			ConstRange range;
			ConstIterator equal;
		};

		template <typename Type, bool MultipleKeys>
		class As_Insert_Return
		{
		public:
			Type operator()(
				const Iterator& that, bool success) const
			{
				return that;
			}
		};

		template <typename Type>
		class As_Insert_Return<Type, false>
		{
		public:
			Type operator()(
				const Iterator& that, bool success) const
			{
				return Type(that, success);
			}
		};

		Insert_Return insertReturnType(
			const Iterator& that, bool success) const
		{
			return As_Insert_Return<Insert_Return, Settings::MultipleKeys>()(that, success);
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

		//! Attaches a new node into the tree and rebalances.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow

		Preconditions:
		node is not attached to any tree

		node:
		The node to attach into the tree.

		parent, right:
		The initial attachment position as provided
		by findInsertParent().
		*/
		void attach(
			Node* node,
			Node* parent,
			bool right);

		//! Rebalances the red-black tree after attaching a node.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow
		*/
		void rebalanceAfterAttach(Node* node);

		//! Detaches a node from the tree and rebalances.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow

		returns:
		The successor of the detached node.
		*/
		Node* detach(Node* node);

		//! Rebalances the red-black tree after detaching a node.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow
		*/
		void rebalanceAfterDetach(
			Node* parent, bool right);

		//! Returns the elements equivalent to the given key.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow
		*/
		EqualRange_Return equalRange(
			const Key& key, 
			const FindEqual_Return& equalAndUpper,
			EqualRange compute) const;

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

		//! An optimized call for updating hierarchical information.
		void update(Iterator element)
		{
			this->updateHierarchical(element);
		}

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
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void link(Node* parent, Node* child, bool linkRight);

		//! Rotates the tree around the 'node'.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Node* rotate(Node* node, bool rotateRight);

		//! Sets the root node.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void setRoot(Node* node)
		{
			sentinel_->right() = node;
		}

		//! Returns the root node.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Node* rootNode() const
		{
			return sentinel_->right();
		}

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
		Preconditions:
		node->isLocalMaximum()

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

		//! The minimum node of the tree.
		Node* minimum_;

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
		The right child of the sentinel node is 
		the root node.
		*/
		Node_Base* sentinel_;

		//! The number of stored elements in the tree.
		integer size_;

		//! The black-height of the tree.
		/*!
		In a red-black tree, the paths from a given node to 
		its leaves have the same number of black nodes,
		which is called the black-height of the node. The
		black-height of the tree is the black-height of the
		root. Tracing the black-height of the tree is 
		important because it allows to compute the join
		and union of two red-black trees in logarithmic
		time. Interestingly, the black-height of the tree 
		can be traced incrementally with almost no overhead.
		*/
		integer blackHeight_;
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
	template <typename Settings, template <typename> class Customization>
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
		template <typename> class Customization = Empty_RedBlackTree_Customization>
	using RedBlack_Map = 
		RedBlackTree<RedBlack_Settings<Key, Data, Compare, DereferenceType_, false>, Customization>;

	template <
		typename Key, 
		typename Data,
		typename Compare = LessThan,
		integer DereferenceType_ = RedBlackTree_Dereference_Default,
		template <typename> class Customization = Empty_RedBlackTree_Customization>
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
		template <typename> class Customization = Empty_RedBlackTree_Customization>
	using RedBlack_Set = 
		RedBlackTree<RedBlack_Settings<Key, void, Compare, DereferenceType_, false>, Customization>;

	template <
		typename Key, 
		typename Compare = LessThan,
		integer DereferenceType_ = RedBlackTree_Dereference_Default,
		template <typename> class Customization = Empty_RedBlackTree_Customization>
	using RedBlack_MultiSet = 
		RedBlackTree<RedBlack_Settings<Key, void, Compare, DereferenceType_, true>, Customization>;

}

#include "pastel/sys/redblacktree.hpp"
#include "pastel/sys/redblacktree_copy.hpp"
#include "pastel/sys/redblacktree_erase.hpp"
#include "pastel/sys/redblacktree_insert.hpp"
#include "pastel/sys/redblacktree_invariants.hpp"
#include "pastel/sys/redblacktree_private.hpp"
#include "pastel/sys/redblacktree_search.hpp"

#endif
