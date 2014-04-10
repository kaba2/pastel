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
		PASTEL_STATIC_ASSERT((!std::is_same<Key, void>::value));

		PASTEL_FWD(Propagation);
		PASTEL_FWD(Propagation_Class);
		PASTEL_FWD(Data);
		PASTEL_FWD(Data_Class);
		PASTEL_FWD(Less);

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
		RedBlackTree() = default;
		
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
			, end_(new Sentinel_Node(bottomNode()->propagation()))
		{
			*this = std::move(that);
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

		Preserves the end node, shares 
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
			if (hasSeparateSentinels())
			{
				deallocateSentinel(end_);
			}
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
			const Key& key, 
			const Data_Class& data = Data_Class(),
			const Propagation_Class& propagation = Propagation_Class());

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
		ConstIterator findEqual(
			const Key& key,
			const ConstIterator& start) const
		{
			return findEqualAndUpper(key, start).equal;
		}

		Iterator findEqual(
			const Key& key,
			const ConstIterator& start)
		{
			return cast(addConst(*this).findEqual(key, start));;
		}

		ConstIterator findEqual(const Key& key) const
		{
			return findEqual(key, croot());
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
		FindEqual_Return findEqualAndUpper(
			const Key& key, const ConstIterator& start) const;

		//! Finds the top-most element equivalent to key, and an upper bound.
		/*!
		This is a convenience function which returns
		findEqualAndUpper(key, croot()).
		*/
		FindEqual_Return findEqualAndUpper(
			const Key& key) const
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
			const Key& key, 
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
		ConstRange equalRange(const Key& key) const
		{
			auto equalAndUpper = findEqualAndUpper(key);
			return ConstRange(
				lowerBound(key, equalAndUpper), 
				upperBound(key, equalAndUpper));
		}

		std::pair<ConstIterator, ConstIterator> 
			equal_range(const Key& key) const
		{
			ConstRange range = equalRange(key);
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
			return lowerBound(key, findEqualAndUpper(key));
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
			const FindEqual_Return& equalAndUpper) const;

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
			return upperBound(key, findEqualAndUpper(key));
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
			return findInsert(key, equalAndUpper).upper;
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

		Time complexity: O(log(n_2 / n_1))
		Exception safety: nothrow

		The sentinel nodes are preserved.
		*/
		RedBlackTree& join(RedBlackTree& that);

		//! Splits the tree into two.
		/*!
		Time complexity: O(log(size()) + 2))
		Exception safety: nothrow

		returns:
		A red-black tree which contains all the elements
		greater-than-or-equal-to 'rightBegin', and which 
		shares the bottom node with this tree.

		The sentinel nodes are preserved.
		*/
		RedBlackTree split(const ConstIterator& rightBegin);

		//! Splits the tree into two.
		/*!
		This is a convenience function which returns
		split(lowerBound(key)).
		*/
		RedBlackTree split(const Key& key);

		//! Returns the number of equivalent elements.
		/*!
		Time complexity: O(log(size()))
		Exception safety: nothrow
		*/
		integer count(const Key& key) const;

		//! Splits the sentinel node.
		/*!
		Preconditions:
		empty() || hasSeparateSentinels()

		Time complexity: O(1)
		Exception safety: strong

		The single sentinel node is split into an
		end node and a bottom node.
		This allows the bottom node to be
		shared, which in turned allows trees to be
		joined.
		*/
		void splitSentinels();

		//! Merges the sentinel nodes.
		/*!
		Preconditions:
		!sharesBottom()

		Time complexity: O(1)
		Exception safety: nothrow

		The end node is removed and replaced
		with the bottom node.
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

			splitSentinels();
			bottom_ = that.bottom_;
		}

		//! Returns whether the tree shares its bottom node.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		bool sharesBottom() const
		{
			return bottom_.use_count() > 1;
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

		//! Returns an iterator to the smallest element.
		PASTEL_ITERATOR_FUNCTIONS(begin, minNode());

		//! Returns an iterator to the end node.
		PASTEL_ITERATOR_FUNCTIONS(end, endNode());

		//! Returns the iterator to the greatest element.
		PASTEL_ITERATOR_FUNCTIONS(last, maxNode());

		//! Returns an iterator to the root node.
		PASTEL_ITERATOR_FUNCTIONS(root, rootNode());

		//! Returns an iterator to the bottom node.
		PASTEL_ITERATOR_FUNCTIONS(bottom, bottomNode());

		//! Returns an iterator range.
		PASTEL_RANGE_FUNCTIONS(range, begin, end);

		//! Returns a key-iterator to the smallest element.
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Key_, keyBegin, minNode());

		//! Returns a key-iterator to the end node.
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Key_, keyEnd, endNode());

		//! Returns a key-iterator range.
		PASTEL_RANGE_FUNCTIONS_PREFIX(Key_, keyRange, keyBegin, keyEnd);

		//! Returns a data-iterator to the smallest element.
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Data_, dataBegin, minNode());

		//! Returns a data-iterator to the end node.
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Data_, dataEnd, endNode());

		//! Returns a data-iterator range.
		PASTEL_RANGE_FUNCTIONS_PREFIX(Data_, dataRange, dataBegin, dataEnd);

	private:
		PASTEL_FWD(Node);
		PASTEL_FWD(Sentinel_Node);
		PASTEL_FWD(EndPtr);
		PASTEL_FWD(BottomPtr);

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
			const BottomPtr& bottom);

		//! Allocates a data-node.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		Node* allocateNode(
			const Key& key,
			const Data_Class& data,
			const Propagation_Class& propagation);

		//! Deallocates a data-node.
		/*!
		Preconditions:
		!node->isSentinel()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void deallocateNode(Node* node);

		//! Allocates a sentinel node.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		EndPtr allocateSentinel()
		{
			return new Sentinel_Node();
		}

		//! Deallocates a sentinel node.
		/*!
		Preconditions:
		node->isSentinel()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void deallocateSentinel(EndPtr node)
		{
			ASSERT(node->isSentinel());
			delete node;
		}

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

		node:
		The node to attach into the tree.

		parent, right:
		The attachment position.
		*/
		void attach(
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
		*/
		void rebalanceBlackLoss(
			Node* parent, bool right);

		//! Fixes a red-red violation.
		/*!
		Preconditions:
		* The subtree rooted at 'node' is a red-black tree,
		with propagation data up-to-date.
		* node->red()
		* !node->isSentinel()

		Time complexity: O(log(size()))
		Exception safety: nothrow
		*/
		void rebalanceRedViolation(Node* node);

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

		//! Updates propagation data.
		/*!
		Time complexity: updatePropagation()
		Exception safety: nothrow
		*/
		void update(const Iterator& element)
		{
			ASSERT(!element.isSentinel());

			Node* node = element.base();
			node->setSize(
				node->left()->size() + 1 +
				node->right()->size());

			this->updatePropagation(
				element,
				(Propagation_Class&)element.propagation());
		}

		//! Returns the result of comparing keys.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		bool less(const Key& left, const Key& right) const
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

		Time complexity: O(1)
		Exception safety: nothrow

		Replaces 'node' with 'middle', makes
		'node' as the '!thatRight' child of 'middle',
		and 'that' as the 'thatRight' child of 'middle'.
		Rebalances any red-violations. In case 'thatRight'
		is true, the maximum will not be updated. Similarly
		for the minimum if 'thatRight' is false.
		*/
		void join(
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

		//! Returns the end node.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Node* endNode() const
		{
			return (Node*)end_;
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
		BottomPtr bottom_ = new Sentinel_Node();

		// FIX: Visual Studio 2013 has a bug which does not allow 
		// std::make_shared to be used as a member initializer.
		// BottomPtr bottom_ = std::make_shared<Sentinel_Node>();

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
		EndPtr end_ = bottom_.get();

		//! The root node.
		Node* root_ = (Node*)end_;

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

	//! Returns whether the red-black tree invariants hold for the tree.
	/*!
	Time complexity: O(tree.size())
	Exception safety: nothrow

	This function is useful only for testing. For a correct implementation
	this function will always return true.
	*/
	template <typename Settings, template <typename> class Customization>
	bool testInvariants(const RedBlackTree<Settings, Customization>& tree);

	//! Returns the alpha-quantile of the elements.
	/*!
	Time complexity:
	O(log(tree.size())), if the alpha-quantile is not cbegin() or clast(),
	O(1), otherwise

	returns:
	An element with the index closest to floor(alpha * tree.size()). This 
	corresponds to assigning an element with an index i the quantile 
	f(i) = (i + 0.5) / tree.size(), and then returning the element with
	the closest quantile. For example, if the elements are [a, b, c, d, e],
	then (-inf, 0.2) ==> a, [0.2, 0.4) ==> b, [0.4, 0.6) ==> c, [0.6, 0.8) ==> d,
	[0.8, inf) ==> e.
	*/
	template <typename Settings, template <typename> class Customization>
	typename RedBlackTree<Settings, Customization>::ConstIterator
	quantile(
		const RedBlackTree<Settings, Customization>& tree,
		real alpha);

}

namespace Pastel
{

	template <
		typename Key_, 
		typename Data_ = void,
		typename Less_ = LessThan,
		typename Propagation_ = void,
		typename SentinelData_ = void,
		bool MultipleKeys_ = false>
	class RedBlack_Settings
	{
	public:
		using Key = Key_;
		using Data = Data_;
		using Less = Less_;
		using Propagation = Propagation_;
		using SentinelData = SentinelData_;
		PASTEL_CONSTEXPR bool MultipleKeys = MultipleKeys_;
	};

}

// Map

namespace Pastel
{

	template <
		typename Key, 
		typename Data,
		typename Less = LessThan,
		typename Propagation = void,
		typename SentinelData = void,
		template <typename> class Customization = Empty_RedBlackTree_Customization>
	using RedBlack_Map = 
		RedBlackTree<RedBlack_Settings<Key, Data, Less, Propagation, SentinelData, false>, 
		Customization>;

	template <
		typename Key, 
		typename Data,
		typename Less = LessThan,
		typename Propagation = void,
		typename SentinelData = void,
		template <typename> class Customization = Empty_RedBlackTree_Customization>
	using RedBlack_MultiMap = 
		RedBlackTree<RedBlack_Settings<Key, Data, Less, Propagation, SentinelData, true>, 
		Customization>;

}

// Set

namespace Pastel
{

	template <
		typename Key,
		typename Less = LessThan,
		typename Propagation = void,
		typename SentinelData = void,
		template <typename> class Customization = Empty_RedBlackTree_Customization>
	using RedBlack_Set = 
		RedBlackTree<RedBlack_Settings<Key, void, Less, Propagation, SentinelData, false>, 
		Customization>;

	template <
		typename Key, 
		typename Less = LessThan,
		typename Propagation = void,
		typename SentinelData = void,
		template <typename> class Customization = Empty_RedBlackTree_Customization>
	using RedBlack_MultiSet = 
		RedBlackTree<RedBlack_Settings<Key, void, Less, Propagation, SentinelData, true>, 
		Customization>;

}

#include "pastel/sys/redblacktree.hpp"
#include "pastel/sys/redblacktree_copy.hpp"
#include "pastel/sys/redblacktree_count.hpp"
#include "pastel/sys/redblacktree_erase.hpp"
#include "pastel/sys/redblacktree_insert.hpp"
#include "pastel/sys/redblacktree_invariants.hpp"
#include "pastel/sys/redblacktree_join.hpp"
#include "pastel/sys/redblacktree_rebalance_black_loss.hpp"
#include "pastel/sys/redblacktree_rebalance_red_violation.hpp"
#include "pastel/sys/redblacktree_search.hpp"
#include "pastel/sys/redblacktree_sentinels.hpp"
#include "pastel/sys/redblacktree_splice.hpp"
#include "pastel/sys/redblacktree_split.hpp"
#include "pastel/sys/redblacktree_swap.hpp"
#include "pastel/sys/redblacktree_quantile.hpp"

#endif
