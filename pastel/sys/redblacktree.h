// Description: Red-black tree

#ifndef PASTELSYS_REDBLACKTREE_H
#define PASTELSYS_REDBLACKTREE_H

#include "pastel/sys/redblacktree_fwd.h"
#include "pastel/sys/redblacktree_concepts.h"
#include "pastel/sys/redblacktree_node.h"
#include "pastel/sys/redblacktree_iterator.h"
#include "pastel/sys/all_downfilter.h"

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
		PASTEL_FWD(Key_Class);

		PASTEL_FWD(Propagation);
		PASTEL_FWD(Propagation_Class);
		PASTEL_FWD(Data);
		PASTEL_FWD(Data_Class);
		PASTEL_FWD(Less);

		PASTEL_FWD(Iterator);
		PASTEL_FWD(ConstIterator);
		PASTEL_FWD(Range);
		PASTEL_FWD(ConstRange);

		PASTEL_FWD(Key_Node);
		PASTEL_FWD(Node);

		PASTEL_FWD(Insert_Return);
		PASTEL_FWD(FindEqual_Return);
		PASTEL_FWD(FindInsert_Return);

		using iterator = Iterator;
		using const_iterator = ConstIterator;

		//! Constructs an empty tree.
		/*!
		Time complexity: O(1)
		Exception safety: strong

		The tree is given a single sentinel node.
		The sentinel's propagation data, if exists, 
		is default-constructed.
		*/
		RedBlackTree()
		{
			onConstruction();
		}
		
		//! Copy-constructs from another tree.
		/*!
		Time complexity: O(that.size())
		Exception safety: strong

		The tree is given separate sentinel nodes.
		The bottom node is shared with 'that'.
		The end node's propagation data, if exists, 
		is copy-constructed from 'that'.
		*/
		RedBlackTree(const RedBlackTree& that)
		: RedBlackTree(that, that.bottom_)
		{
		}

		//! Move-constructs from another tree.
		/*!
		Time complexity: O(1)
		Exception safety: strong

		The tree is given separate sentinel nodes.
		The bottom node is shared with 'that'.
		The end node's propagation data, if exists,
		is copy-constructed from 'that'.
		*/
		RedBlackTree(RedBlackTree&& that)
			: bottom_(that.bottom_)
			, end_(std::make_shared<Sentinel_Node>())
		{
			*this = std::move(that);
			onConstruction();
		}

		//! Constructs from a list of keys.
		/*!
		Time complexity: O(dataSet.size())
		Exception safety: strong

		The tree is given a single sentinel node.
		The user-data is default-initialized.
		The sentinel's propagation, if exists, 
		is default-constructed.
		*/
		template <typename Key_>
		RedBlackTree(std::initializer_list<Key_> dataSet)
		{
			*this = dataSet;
			onConstruction();
		}

		//! Constructs from a list of key-value pairs.
		/*!
		Time complexity: O(dataSet.size())
		Exception safety: strong

		The tree is given a single sentinel node.
		The sentinel's propagation, if exists, 
		is default-constructed.
		*/
		RedBlackTree(std::initializer_list<std::pair<Key, Data_Class>> dataSet)
		{
			*this = dataSet;
		}

		//! Copy-assigns from another tree.
		/*!
		Time complexity: O(size() + that.size())
		Exception safety: strong

		Preserves sentinels.
		*/
		RedBlackTree& operator=(const RedBlackTree& that)
		{
			RedBlackTree copy(that, bottom_);
			swapElements(copy);
			return *this;
		}

		//! Move-assigns from another tree.
		/*!
		Time complexity: O(size())
		Exception safety: nothrow

		Preserves the end-node, shares 
		the bottom node with 'that'.
		*/
		RedBlackTree& operator=(RedBlackTree&& that)
		{
			clear();
			useBottomFrom(that);
			swapElements(that);
			return *this;
		}

		//! Assigns a list of keys.
		/*!
		Time complexity: O(size() + dataSet.size())
		Exception safety: strong

		Preserves sentinels.
		The user-data will be default-initialized.
		*/
		template <typename Key_>
		RedBlackTree& operator=(std::initializer_list<Key_> dataSet)
		{
			auto construct = [&](RedBlackTree& copy)
			{
				for (auto&& key : dataSet)
				{
					copy.insert(key);
				}
			};

			return constructAndSwap(construct);
		}

		//! Assigns a list of key-value pairs.
		/*!
		Time complexity: O(size() + dataSet.size())
		Exception safety: strong

		Preserves sentinels.
		*/
		RedBlackTree& operator=(std::initializer_list<std::pair<Key, Data_Class>> dataSet)
		{
			auto construct = [&](RedBlackTree& copy)
			{
				for (auto&& keyValue : dataSet)
				{
					copy.insert(keyValue.first, keyValue.second);
				}
			};

			return constructAndSwap(construct);
		}

		//! Destructs the tree.
		/*!
		Time complexity: O(size())
		Exception safety: nothrow
		*/
		~RedBlackTree()
		{
			clear();
		}

		//! Swaps two trees.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void swap(RedBlackTree& that);

		//! Swaps elements with another tree.
		/*!
		Preconditions:
		sharesBottom(that)

		Time complexity: O(1)
		Exception safety: nothrow

		Preserves the sentinels in both trees, 
		but swaps everything else.
		*/
		void swapElements(RedBlackTree& that);

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
			return croot().size();
		}

		//! Returns true if the tree is empty.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		bool empty() const
		{
			return croot().isSentinel();
		}

		//! Returns the black-height of the tree.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow

		The black-height of the tree is the black-height
		of the root node. If the tree is empty, then the
		black-height is zero. Note that, since we do not
		require the root to be black, a tree can have
		black-height zero without being empty.
		*/
		integer blackHeight() const
		{
			return blackHeight_;
		}

		//! Returns the black-height of a node.
		/*!
		Time complexity: O(d)
		where
		d is the depth of the the node.

		Exception safety: nothrow

		The black-height of a node is the number of black nodes
		on a downward path from that node to a sentinel node. This
		is well-defined by the red-black invariants.
		*/
		integer blackHeight(const ConstIterator& node) const
		{
			if (node->isSentinel())
			{
				return 0;
			}

			integer result = blackHeight_;
			while(!node->parent()->isSentinel())
			{
				node = node->parent();
				result -= node->black();
			}

			return result;
		}

		//! Inserts an element into the tree.
		/*!
		Time complexity: O(log(size())) * updatePropagation() + onInsert()
		Exception safety: strong + onInsert()

		returns:
		If multiple keys are allowed, an iterator to the new element.
		Otherwise an iterator-bool pair, where the boolean tells whether
		the element was inserted. In case the element is not inserted,
		the iterator points to the existing equivalent element.
		*/
		Insert_Return insert(
			const Key_Class& key, 
			const Data_Class& data = Data_Class());

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
		Iterator erase(const Key_Class& key);

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
		find(key, filter) != cend().
		*/
		template <typename DownFilter = All_DownFilter>
		bool exists(
			const Key_Class& key,
			const DownFilter& filter = DownFilter()) const
		{
			return find(key, filter) != cend();
		}

		//! Searches for the first element with key == 'key'.
		/*!
		Preconditions:
		The subtree-indicator holds for a subtree if and only 
		if the element-indicator holds for some element in the
		subtree.

		Time complexity: O(log(size()))
		Exception safety: nothrow

		If there are multiple elements equivalent to 'key',
		then the first of them is returned (same as lowerBound()).
		*/
		template <typename DownFilter = All_DownFilter>
		ConstIterator find(
			const Key_Class& key,
			const DownFilter& filter = DownFilter()) const;

		template <typename DownFilter = All_DownFilter>
		Iterator find(
			const Key_Class& key,
			const DownFilter& filter = DownFilter())
		{
			return cast(addConst(*this).find(
				key, filter));
		}

		//! Finds the top-most element equivalent to key, and an upper bound.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow

		The upper bound is the smallest upper-bound located above the
		top-most element.
		*/
		FindEqual_Return findEqualAndUpper(
			const Key_Class& key,
			const ConstIterator& start) const;

		//! Finds the top-most element equivalent to key, and an upper bound.
		/*!
		This is a convenience function which returns
		findEqualAndUpper(key, croot()).
		*/
		FindEqual_Return findEqualAndUpper(
			const Key_Class& key) const
		{
			return findEqualAndUpper(key, croot());
		}

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
			const Key_Class& key, 
			const FindEqual_Return& equalRoot) const;

		//! Finds the node at which to join another tree.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow
		*/
		ConstIterator findJoin(
			integer joinBlackHeight,
			bool right) const;

		//! Returns the elements equivalent to the given key.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow
		*/
		template <typename DownFilter = All_DownFilter>
		ConstRange equalRange(
			const Key_Class& key,
			const DownFilter& filter = DownFilter()) const
		{
			auto equalAndUpper = findEqualAndUpper(key);
			return ConstRange(
				lowerBound(key, equalAndUpper, filter),
				upperBound(key, equalAndUpper, filter));
		}

		std::pair<ConstIterator, ConstIterator> 
			equal_range(const Key_Class& key) const
		{
			ConstRange range = equalRange(key);
			return std::make_pair(range.begin(), range.end());
		}

		//! Returns the elements equivalent to the given key.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow
		*/

		template <typename DownFilter = All_DownFilter>
		Range equalRange(
			const Key_Class& key,
			const DownFilter& filter = DownFilter())
		{
			return cast(addConst(*this).equalRange(key, filter));
		}

		std::pair<Iterator, Iterator> 
			equal_range(const Key_Class& key)
		{
			Range range = equalRange(key);
			return std::make_pair(range.begin(), range.end());
		}

		//! Return the first element with key >= 'key'.
		/*!
		This is a convenience function which calls
		equalRange(key).first
		*/
		template <typename DownFilter>
		ConstIterator lowerBound(
			const Key_Class& key,
			const DownFilter& filter) const
		{
			return lowerBound(key, findEqualAndUpper(key), filter);
		}

		template <typename DownFilter = All_DownFilter>
		Iterator lowerBound(
			const Key_Class& key,
			const DownFilter& filter = DownFilter())
		{
			return cast(addConst(*this).lowerBound(key, filter));
		}

		ConstIterator lower_bound(const Key_Class& key) const
		{
			return lowerBound(key);
		}

		Iterator lower_bound(const Key_Class& key)
		{
			return lowerBound(key);
		}

		//! Searches for the first element with key > 'key'.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow
		*/
		template <typename DownFilter = All_DownFilter>
		ConstIterator upperBound(
			const Key_Class& key,
			const DownFilter& filter = DownFilter()) const
		{
			return upperBound(key, findEqualAndUpper(key), filter);
		}

		template <typename DownFilter = All_DownFilter>
		Iterator upperBound(
			const Key_Class& key,
			const DownFilter& filter = DownFilter())
		{
			return cast(addConst(*this).upperBound(key, filter));
		}

		ConstIterator upper_bound(const Key_Class& key) const
		{
			return upperBound(key);
		}

		Iterator upper_bound(const Key_Class& key)
		{
			return upperBound(key);
		}

		//! Returns the first equivalent element below.
		/*!
		Time complexity: O(log(below.size()))
		Exception safety: nothrow
		*/
		template <typename DownFilter>
		ConstIterator findFirstEquivalentBelow(
			const ConstIterator& below,
			const DownFilter& filter) const;
		
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

			//! Appends the elements of another tree.
		/*!
		Preconditions:
		sharesBottom(that)
		empty() || 
		that.empty() ||
		last().key() <= that.begin().key() ||
		that.last().key() <= begin().key()

		If multiple keys are not allowed, then the 
		comparisons above are required to hold strictly (<).

		Time complexity: 
		O(log(n_1 + 2) - log(n_2 + 2))
		where
		n_1 = std::max(size(), that.size()),
		n_2 = std.:min(size(), that.size()).

		Exception safety: nothrow

		The sentinel nodes are preserved.
		*/
		RedBlackTree& join(RedBlackTree& that);

		//! Splits the tree into two.
		/*!
		Time complexity: O(log(size()) + 2))
		Exception safety: strong

		returns:
		A red-black tree which contains all the elements
		greater-than-or-equal-to 'rightBegin', and which 
		shares the bottom node with this tree.

		The sentinel nodes are preserved.
		*/
		RedBlackTree split(const ConstIterator& rightBegin);

		//! Splits the tree into two by a key.
		/*!
		This is a convenience function which returns
		split(lowerBound(key, filter)).
		*/
		template <typename DownFilter = All_DownFilter>
		RedBlackTree split(
			const Key_Class& key,
			const DownFilter& filter = DownFilter())
		{
			return split(lowerBound(key, filter));
		}

		//! Returns the number of equivalent elements.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow
		*/
		integer count(const Key_Class& key) const;

		//! Splits the sentinel node.
		/*!
		Preconditions:
		empty()

		Time complexity: O(1)
		Exception safety: strong

		The single sentinel node is split into an
		end node and a bottom node. This allows the 
		bottom node to be shared, which in turn allows 
		trees to be	joined. The end-node is preserved.
		*/
		void splitSentinels();

		//! Merges the sentinel nodes.
		/*!
		Preconditions:
		!sharesBottom()

		Time complexity: O(1)
		Exception safety: nothrow

		If the tree is empty, the bottom-node is replaced
		with the end-node, thus preserving the end-node.
		Otherwise the end-node is replaced with the bottom 
		node, thus preserving the bottom node.
		*/
		void mergeSentinels();

		//! Returns whether the tree has separate sentinels.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow

		By default, the tree has only one sentinel node,
		which works as both the bottom node and the end node.
		*/
		bool hasSeparateSentinels() const
		{
			return endNode() != bottomNode();
		}

		//! Shares the bottom node with another tree.
		/*!
		Preconditions:
		empty()

		Time complexity: O(1)
		
		Exception safety: 
		nothrow, if hasSeparateSentinels()
		strong, otherwise
		*/
		void useBottomFrom(const RedBlackTree& that) 
		{
			ENSURE(empty());
			bottom_ = that.bottom_;
		}

		//! Returns whether the tree shares its bottom node.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		bool sharesBottom() const
		{
			if (hasSeparateSentinels())
			{
				return bottom_.use_count() > 1;
			}

			return bottom_.use_count() > 2;
		}

		//! Returns whether the trees share the same bottom node.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		bool sharesBottom(const RedBlackTree& that) const
		{
			return bottom_ == that.bottom_;
		}

		//! Sets the root black.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void setRootBlack()
		{
			if (rootNode()->black())
			{
				return;
			}

			rootNode()->setBlack();
			update(rootNode());
			++blackHeight_;
		}

		//! Updates propagation data on the path to root.
		/*!
		Preconditions:
		* The propagation data of the children of 'node'
		are up-to-date.
		* The propagation data of a sibling in the path
		from 'node' to the root is up-to-date.

		Time complexity: O(log(size()))
		Exception safety: nothrow

		Calls updatePropagation(node) for each node in 
		the path to the root, including 'node' itself.

		This function is needed whenever you change that
		data in 'node' from which the propagation data
		is computed.
		*/
		void updateToRoot(const ConstIterator& node)
		{
			updateToRoot((Node*)node.base());
		}

		//! Updates propagation data on the paths to the root.
		/*!
		Preconditions:
		* The propagation data of the children of the nodes
		in 'updateSet' are up-to-date.
		* The propagation data of a sibling in a path
		from a node in 'updateSet' to the root is up-to-date,
		provided that sibling is not contained in another
		such path.

		Time complexity: O(m) * updatePropagation()
		where
		m is the number of nodes on the paths to the root.

		Exception safety: nothrow

		Calls updatePropagation(node) for each node in 
		the paths to the root, in correct order and exactly 
		once. This allows to make multiple changes to that 
		data from which the propagation data is computed,
		and then to update the propagation data all at once.
		*/
		template <typename ConstIterator_Range>
		void updateToRootMany(const ConstIterator_Range& updateSet);

		void updateToRootMany(
			const std::initializer_list<ConstIterator>& updateSet)
		{
			updateToRootMany<std::initializer_list<ConstIterator>>(updateSet);
		}

		//! Returns the smallest element.
		PASTEL_ITERATOR_FUNCTIONS(begin, minNode());

		//! Returns the end node.
		PASTEL_ITERATOR_FUNCTIONS(end, endNode());

		//! Returns the greatest element.
		PASTEL_ITERATOR_FUNCTIONS(last, maxNode());

		//! Returns the root node.
		PASTEL_ITERATOR_FUNCTIONS(root, rootNode());

		//! Returns the bottom node.
		PASTEL_ITERATOR_FUNCTIONS(bottom, bottomNode());

		//! Returns the element range.
		PASTEL_RANGE_FUNCTIONS(range, begin, end);

		//! Returns an extremum element.
		ConstIterator extremum(bool right) const
		{
			return ConstIterator(extremumNode(right));
		}

		Iterator extremum(bool right)
		{
			return cast(addConst(*this).extremum(right));
		}

		ConstIterator cextremum(bool right) const
		{
			return extremum(right);
		}

	private:
		PASTEL_FWD(Node);
		PASTEL_FWD(Sentinel_Node);
		PASTEL_FWD(SentinelPtr);

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

		RedBlackTree(
			const RedBlackTree& that,
			const SentinelPtr& bottom);

		//! Allocates a data-node.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		Key_Node* allocateNode(
			const Key_Class& key,
			const Data_Class& data);

		//! Deallocates a data-node.
		/*!
		Preconditions:
		!node->isSentinel()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void deallocateNode(Key_Node* node);

		//! Attaches a new subtree into the tree.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow

		Preconditions:
		!node->isSentinel()
		!node->parent()
		!parent->child(right)->isSentinel()
		size > 0
		Attaching the subtree preserves the binary search property.
		The attached subtree shares the bottom node with this tree.

		node:
		The root of the subtree to attach into the tree.

		parent, right:
		The attachment position.

		size:
		The number of elements in the attached subtree.

		No rebalancing or propagation updates are done 
		after attaching.
		*/
		void attachSubtree(
			Node* node,
			Node* parent,
			bool right,
			integer size);

		//! Attaches a new node into the tree and rebalances.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow

		Preconditions:
		!node->parent()
		!node->left()
		!node->right()
		!parent->child(right)->isSentinel()
		
		Attaching the node preserves the binary search property.
		The propagation data will not be updated.

		node:
		The node to attach into the tree.

		parent, right:
		The attachment position.
		*/
		Node* attach(
			Node* node,
			Node* parent,
			bool right);

		//! Detaches a node from the tree and rebalances.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow

		returns:
		The successor of the detached node.
		*/
		Node* detach(Node* node);

		//! Fixes a black-height loss.
		/*!
		Preconditions:
		* The left subtree and the right subtree of 'parent' are
		red-black trees, with propagation data up-to-date.
		* The number of black nodes on a downward path starting from
		'parent', or its ancestor, and ending in a sentinel node in the 
		'right' subtree of 'parent' is one less than it is on
		a path that does not end in the 'right' subtree of 'parent'.

		Time complexity: O(log(size()))
		Exception safety: nothrow

		The rebalancing uses either 0, 1, 2, or 3 rotations.
		If it uses 2 rotations, then it is because of a double-rotation.
		If it uses 3 rotations, then it is because of a rotation and
		a double-rotation. 

		returns:
		The node at which the black-loss rebalancing completed.
		The propagation data must be updated from this node upwards.
		*/
		Node* rebalanceBlackLoss(
			Node* parent, bool right);

		//! Fixes a red-red violation.
		/*!
		Preconditions:
		* The subtree rooted at 'node' is a red-black tree,
		with propagation data up-to-date.
		* node->red()
		* !node->isSentinel()

		Time complexity: 
		O(log(size()))
		Omega(d)
		where
		d is the depth of 'node'.

		Exception safety: nothrow

		The rebalancing uses either 0, 1, or 2 rotations.
		If it uses 2 rotations, then it is because of a double-rotation.

		returns:
		The node at which the red-red rebalancing completed.
		The propagation data must be updated from this node upwards.
		*/
		Node* rebalanceRedViolation(Node* node);

		template <typename DownFilter>
		ConstIterator lowerBound(
			const Key_Class& key,
			const FindEqual_Return& equalAndUpper,
			const DownFilter& filter) const;

		template <typename DownFilter>
		Iterator lowerBound(
			const Key_Class& key,
			const FindEqual_Return& equalAndUpper,
			const DownFilter& filter)
		{
			return cast(addConst(*this).lowerBound(key, equalAndUpper, filter));
		}

		template <typename DownFilter>
		ConstIterator upperBound(
			const Key_Class& key,
			const FindEqual_Return& equalAndUpper,
			const DownFilter& filter) const
		{
			ConstIterator upper = 
				findInsert(key, equalAndUpper).upper;
			if (!filter.element(upper))
			{
				upper = upper.next(filter);
			}
			return upper;
		}

		template <typename DownFilter>
		Iterator upperBound(
			const Key_Class& key,
			const FindEqual_Return& equalAndUpper,
			const DownFilter& filter)
		{
			return cast(addConst(*this).upperBound(key, equalAndUpper, filter));
		}

		//! Returns the elements equivalent to the given key.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow
		*/
		EqualRange_Return equalRange(
			const Key_Class& key, 
			const FindEqual_Return& equalAndUpper,
			EqualRange compute) const;

		//! Returns the top-most element equivalent to the key.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow

		Equivalently, this is the first element equivalent to
		the key when traversing the tree in pre-order.
		*/
		ConstIterator findEqual(
			const Key_Class& key,
			const ConstIterator& start) const
		{
			return findEqualAndUpper(key, start).equal;
		}

		Iterator findEqual(
			const Key_Class& key,
			const ConstIterator& start)
		{
			return cast(addConst(*this).findEqual(key, start));;
		}

		//! Updates propagation data.
		/*!
		Time complexity: updatePropagation()
		Exception safety: nothrow

		The update is performed only if both children
		of the node have valid propagation data.

		returns:
		Whether the update was performed.
		*/
		bool update(const Iterator& element);

		bool update(Node* node)
		{
			return update(Iterator(node));
		}

		//! Updates propagation data on the path to root.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow

		Calls updatePropagation(node) for each node in 
		the path to the root, including 'node' itself.
		However, if a node has a child with invalid
		propagation data, then the update is terminated.

		returns:
		The node at which the update was terminated,
		due to reaching a node which either the sentinel
		node, or a node with a child with invalid
		propagation data.
		*/
		Node* updateToRoot(Node* node);

		//! Invalidates propagation data.
		/*!
		Time complexity: 
		O(d),
		where
		d is the number of steps needed to
		reach an invalidated ancestor node.

		Exception safety: nothrow

		This function is useful as a way to amortize the
		cost of multiple changes to propagation data. The
		idea is to mark the path to the root as invalid.
		This marking is complete as soon as an invalidated 
		node is encountered. Later, the invalidated paths
		can be updated, using updateToRoot(), without
		performing redundant updates. This decreases the
		complexity of split() from O(log^2) to O(log).

		returns:
		Either the end-node, or the first ancestor node
		which is already invalidated.
		*/
		Node* invalidateToRoot(Node* node);

		Iterator invalidateToRoot(const ConstIterator& node)
		{
			return Iterator(invalidateToRoot((Node*)node.base()));
		}

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

		//! Assigns a constructed tree.
		/*!
		Time complexity: construct()
		Exception safety: strong

		This function assigns a tree constructed with
		the passed-in construct() function. The purpose
		of this function is to guarantee that the
		sentinel nodes are preserved in the process.
		*/
		template <typename Construct>
		RedBlackTree& constructAndSwap(Construct construct)
		{
			RedBlackTree copy;
			copy.useBottomFrom(*this);
			construct(copy);
			swapElements(copy);
			return *this;
		}

		//! Returns the result of comparing keys.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		bool less(const Key_Class& left, const Key_Class& right) const
		{
			return Less()(left, right);
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

		//! Joins 'that' subtree to this tree.
		/*!
		Preconditions:
		* The black-height of 'that' equals the black-height of 'node'.
		* The operation preserves the binary-search property.
		that->black()
		node->black()
		!middle->isSentinel()
		!middle->parent()
		!middle->left()
		!middle->right()
		middle->red()

		Time complexity: O(1) amortized 
		Exception safety: nothrow

		Replaces 'node' with 'middle', makes
		'node' as the '!right' child of 'middle',
		and 'that' as the 'right' child of 'middle'.
		Rebalances any red-violations. In case 'right'
		is true, the maximum will not be updated. Similarly
		for the minimum if 'right' is false. The propagation
		data will not be updated from 'middle' upwards, and
		therefore must be done later.

		The time-complexity arises from calling the rebalancing
		algorithm for the red-red violations.
		*/
		Node* join(
			Node* that, 
			integer thatBlackHeight,
			Node* parent, bool right,
			Node* middle);

		//! Release the ownership of the nodes.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void forget()
		{
			minNode() = endNode();
			maxNode() = endNode();
			rootNode() = endNode();

			end_->isolateSelf();
			blackHeight_ = 0;
		}

		//! Returns whether the tree can be joined with 'that'.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow

		Trees can be joined if
		* either tree is empty, or
		* multiple keys are not allowed and
		[last.key() < that.begin().key() or
		that.last().key() < begin().key()], or
		* multiple keys are allowed and 
		[last.key() <= that.begin().key() or
		that.last().key() <= begin().key()].
		*/
		bool canJoin(const RedBlackTree& that) const;

		//! Returns the root node.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Node*& rootNode() const
		{
			return (Node*&)root_;
		}

		//! Returns the minimum node.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Node*& minNode() const
		{
			return (Node*&)end_->right();
		}

		//! Returns the maximum node.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Node*& maxNode() const
		{
			return (Node*&)end_->parent();
		}

		//! Returns an extremum node.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Node*& extremumNode(bool right) const
		{
			return right ? maxNode() : minNode();
		}

		//! Returns the end node.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Node* endNode() const
		{
			return (Node*)end_.get();
		}

		//! Returns the bottom node.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Node* bottomNode() const
		{
			return (Node*)bottom_.get();
		}

		//! The bottom node.
		/*!
		The bottom node is used to denote a missing child node.
		The only exception is that the right child of the maximum
		node is denoted by the end node. The parent, the left
		child, and the right child are the bottom node itself.
		*/
		SentinelPtr bottom_ = new Sentinel_Node();

		// FIX: Visual Studio 2013 has a bug which does not allow 
		// std::make_shared to be used as a member initializer.
		// SentinelPtr bottom_ = std::make_shared<Sentinel_Node>();

		//! The end node.
		/*!
		The parent of the root node and the right child
		of the maximum node are denoted by the end node.
		The parent of the end node is the maximum 
		element, and the left child is the end node
		itself. The right child of the end node 
		is the minimum node. The end node then works as 
		the one-past-last node (end() iterator). 
		*/
		SentinelPtr end_ = bottom_;

		//! The root node.
		Node* root_ = (Node*)end_.get();

		//! The black-height of the tree.
		/*!
		In a red-black tree, the downward paths from a given 
		node to a sentinel node have the same number of black 
		nodes, which is called the black-height of the node. 
		The black-height of the tree is the black-height of the
		root. Tracing the black-height of the tree is 
		important because it allows to compute the join
		and union of two red-black trees in logarithmic
		time. Interestingly, the black-height of the tree 
		can be traced incrementally with almost no overhead.
		*/
		integer blackHeight_ = 0;
	};

}

namespace Pastel
{

	// Map

	template <
		typename Key = void, 
		typename Data = void,
		typename Less = LessThan,
		typename Propagation = void,
		typename SentinelData = void,
		bool UserDataInSentinelNodes = false,
		template <typename> class Customization = Empty_RedBlackTree_Customization>
	using RedBlackTree_Set = 
		RedBlackTree<RedBlackTree_Set_Settings<
		Key, Data, Less, Propagation, SentinelData, false, UserDataInSentinelNodes>, 
		Customization>;

	// Multi-map

	template <
		typename Key = void, 
		typename Data = void,
		typename Less = LessThan,
		typename Propagation = void,
		typename SentinelData = void,
		bool UserDataInSentinelNodes = false,
		template <typename> class Customization = Empty_RedBlackTree_Customization>
	using RedBlackTree_MultiSet = 
		RedBlackTree<RedBlackTree_Set_Settings<
		Key, Data, Less, Propagation, SentinelData, true, UserDataInSentinelNodes>, 
		Customization>;

}

#include "pastel/sys/redblacktree.hpp"
#include "pastel/sys/redblacktree_copy.hpp"
#include "pastel/sys/redblacktree_count.hpp"
#include "pastel/sys/redblacktree_erase.hpp"
#include "pastel/sys/redblacktree_find_below.hpp"
#include "pastel/sys/redblacktree_insert.hpp"
#include "pastel/sys/redblacktree_invariants.h"
#include "pastel/sys/redblacktree_join.hpp"
#include "pastel/sys/redblacktree_rebalance_black_loss.hpp"
#include "pastel/sys/redblacktree_rebalance_red_violation.hpp"
#include "pastel/sys/redblacktree_search.hpp"
#include "pastel/sys/redblacktree_sentinels.hpp"
#include "pastel/sys/redblacktree_splice.hpp"
#include "pastel/sys/redblacktree_split.hpp"
#include "pastel/sys/redblacktree_swap.hpp"
#include "pastel/sys/redblacktree_quantile.h"

#endif
