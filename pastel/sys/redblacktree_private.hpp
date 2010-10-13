#ifndef PASTEL_REDBLACKTREE_PRIVATE_HPP
#define PASTEL_REDBLACKTREE_PRIVATE_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::allocateSentinel(
		const Key& sentinelKey,
		const ValueType* sentinelValue)
	{
		// The sentinel is not allocated from the allocator,
		// since we want to keep its identity even if the
		// whole tree was freed with clear().

		integer rollBackIndex = 0;
		try
		{
			// The balancing algorithms assume that the
			// color of the sentinel is black.
			sentinel_ = new Node(sentinelKey, 0, 0, 0, false);
			sentinel_->parent() = sentinel_;
			sentinel_->left() = sentinel_;
			sentinel_->right() = sentinel_;
			++rollBackIndex;

			if (Node::ValueExists && sentinelValue)
			{
				// Construct the value.
				new(sentinel_->valuePtr()) ValueType(*sentinelValue);
				++rollBackIndex;
			}
		}
		catch(...)
		{
			switch(rollBackIndex)
			{
			case 1:
				delete sentinel_;
				break;
			};

			throw;
		}
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::deallocateSentinel()
	{
		destructNode(sentinel_);
		delete sentinel_;
		sentinel_ = 0;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::initialize()
	{
		// This function is called both in construction
		// and in clear().

		sentinel_->parent() = sentinel_;
		sentinel_->left() = sentinel_;
		sentinel_->right() = sentinel_;

		setMinimum(sentinel_);
		setMaximum(sentinel_);
		
		root_ = sentinel_;
		size_ = 0;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Node*
	RedBlackTree<Key, Compare, RbtPolicy>::allocateNode(
	const Key& key, const ValueType* value, 
	Node* parent, bool red)
	{
		Node* node = 0;
		integer rollBackIndex = 0;
		try
		{
			node = (Node*)allocator_.allocate();
			++rollBackIndex;

			// Construct the node.
			new(node) Node(key, parent, sentinel_, sentinel_, red);
			++rollBackIndex;

			if (Node::ValueExists && value)
			{
				// Construct the value.
				new(node->valuePtr()) ValueType(*value);
			}
		}
		catch(...)
		{
			switch(rollBackIndex)
			{
			case 2:
				node->~Node();
				// Fall-through.
			case 1:
				allocator_.deallocate(node);
				break;
			};

			throw;
		}

		return node;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::destructNode(Node* node)
	{
		if (Node::ValueExists)
		{
			// Destruct the value.
			node->valuePtr()->~ValueType();
		}
		
		// Destruct the node.
		node->~Node();
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Node*
	RedBlackTree<Key, Compare, RbtPolicy>::insert(
	const Key& key, const ValueType* value, Node* node,
	Node* parent, bool fromLeft, Node*& newNode)
	{
		if (node == sentinel_)
		{
			// The key does not exist, so insert it as a leaf node.
			// New nodes are created red.
			newNode = allocateNode(key, value, parent, true);
			++size_;

			if (parent != sentinel_)
			{
				if (fromLeft)
				{
					parent->left() = newNode;
				}
				else
				{
					parent->right() = newNode;
				}
			}

			if (empty() || (parent == minimum() && fromLeft))
			{
				// This is the new minimum element.
				setMinimum(newNode);
			}

			if (empty() || (parent == maximum() && !fromLeft))
			{
				// This is the new maximum element.
				setMaximum(newNode);
			}

			policy_.updateHierarchical(
				Iterator(newNode));

			return newNode;
		}

		// On the way down we visit each node
		// on the path.

		policy_.updateInsertDown(
			Iterator(node));

		if (compare_(key, node->key()))
		{
			// Smaller elements are located at the left child.
			node->left() = insert(
				key, value, node->left(), 
				node, true, newNode);
		}
		else if (compare_(node->key(), key))
		{
			// Greater elements are located at the right child.
			node->right() = insert(
				key, value, node->right(), 
				node, false, newNode);
		}
		else
		{
			// The key already exists. 
			// We return it.

			newNode = node;
			return newNode;
		}

		policy_.updateHierarchical(
			Iterator(node));

		if (node->left()->black() &&
			node->right()->red())
		{
			// We want a left-leaning red-black tree.
			// This fixes that locally.
			node = rotateLeft(node);

			policy_.updateHierarchical(
				Iterator(node->left()));
			policy_.updateHierarchical(
				Iterator(node));
		}

		if (node->left()->red() &&
			node->left()->left()->red())
		{
			// An invariant of a red-black tree is that
			// there are no two subsequent red nodes.
			// This fixes that locally.
			node = rotateRight(node);

			policy_.updateHierarchical(
				Iterator(node->right()));
			policy_.updateHierarchical(
				Iterator(node));
		}

		if (node->left()->red() &&
			node->right()->red())
		{
			flipColors(node);

			policy_.updateHierarchical(
				Iterator(node->left()));
			policy_.updateHierarchical(
				Iterator(node->right()));
			policy_.updateHierarchical(
				Iterator(node));
		}

		return node;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Node* 
		RedBlackTree<Key, Compare, RbtPolicy>::erase(
		Node* node)
	{
		if (node == sentinel_)
		{
			// Nothing to do.
			return node;
		}

		Iterator next(node);
		++next;
		Node* successor = next.node_;

		if (node == minimum())
		{
			// The removed node is the minimum node.
			// Set the minimum to the next smallest node.

			setMinimum(successor);
		}

		if (node == maximum())
		{
			// The removed node is the maximum node.

			// Set the maximum to the next greatest node.
			Iterator prev(node);
			--prev;
			setMaximum(prev.node_);
		}

		// The different cases for removal are:
		// 1) The removed node is red and has at most one child.
		// 2) The removed node is black and has no left child.
		// 3) The removed node is black, has a left child, and
		//    does not have a right child.
		// 4) The removed node has two children.

		integer removalCase = 4;
		if (node->left() == sentinel_)
		{
			removalCase = node->red() ? 1 : 2;
		}
		else if (node->right() == sentinel_)
		{
			removalCase = node->red() ? 1 : 3;
		}

		if (removalCase == 4)
		{
			// Case 4:
			// The removed node has two children.
			// Then the 'node' and its successor are swapped (along with 
			// their colors). Now 'node' does not have a left child,
			// and thus the problem is reduced to either case 1 or case 2.

			// Swap the node and its successor.
			swapNodes(node, successor);

			// Reduce the problem.
			removalCase = node->red() ? 1 : 2;

			// The 'successor' will get called updateHierarchical()
			// at some point, because it is at the path from 
			// 'node' to the root.
		}

		// In all of the cases 1 to 3 we want to
		// remove the node now.

		Node* parent = node->parent();

		// The deleted node has at most one child.
		Node* child = 
			(node->left() != sentinel_) ? 
			node->left() : node->right();

		// Detach the deleted node from the tree. 
		const bool nodeOnLeft = 
			(parent->left() == node);
		if (nodeOnLeft)
		{
			linkLeft(parent, child);
		}
		else
		{
			linkRight(parent, child);
		}

		// Free the memory of 'node'.
		destructNode(node);
		allocator_.deallocate(node);
		--size_;		

		switch(removalCase)
		{
		case 1:
			// Case 1:
			// The removed node is red and has at most one child.
			// Link the black parent and the black child together.
			// There are no violations of the invariants. 
			//
			//      |B          |B
			//      p           p
			//    R/ \B  ==>  B/ \B
			//    n           
			//  B/               
			
			updateToRoot(parent);
			break;
		case 2:
			// Case 2:
			// The removed node is black and has no left child.
			// Link the parent and the possible black right
			// child together. Then the path from 'parent'
			// that went through 'node' has a black-depth one 
			// too low, and thus must be rebalanced.
			//
			//      |R/B        |R/B
			//      p           p
			//    B/ \B  ==>  B/ \B
			//    n           
			//     \B

			rebalance(parent, nodeOnLeft);
			break;
		case 3:
			// Case 3: 
			// The removed node is black, has a left child, and
			// does not have a right child. Link the parent and
			// the left child together...

			if (child->red())
			{
				// If the left child is red, then it is simply 
				// turned to black to resolve any invariant violations.
				//
				//      |R/B  remove(n)   |R/B
				//      p     switch(l)   p
				//    B/ \B   ==>       B/ \B
				//    n           
				//  R/

				child->flipColor();
				// We update starting from 'child'
				// since we flipped its color.
				updateToRoot(child);
			}
			else
			{
				// If the left child is black, the path from 'parent'
				// that went through 'node' has a black-depth one too low,
				// and thus must be rebalanced.
				//
				//      |R/B          |R/B
				//      p    remove   p
				//    B/ \B  ==>    B/ \B
				//    n           
				//  B/

				rebalance(parent, nodeOnLeft);
			}
			break;
		};

		// Return the successor of the deleted element.
		return successor;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::updateToRoot(
		Node* node)
	{
		if (node == sentinel_)
		{
			return;
		}

		Node* child = node;
		while(node != sentinel_)
		{
			policy_.updateHierarchical(
				Iterator(node));
			child = node;
			node = node->parent();
		}

		root_ = child;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::swapNodes(
		Node* aNode, Node* bNode)
	{
		ASSERT(aNode != sentinel_);
		ASSERT(bNode != sentinel_);

		if (aNode == root_)
		{
			root_ = bNode;
		}
		else if (bNode == root_)
		{
			root_ = aNode;
		}

		const bool aLeft = 
			(aNode->parent()->left() == aNode);
		const bool bLeft = 
			(bNode->parent()->left() == bNode);

		Node* aNodeLeft = bNode->left();
		if (aNodeLeft == aNode)
		{
			aNodeLeft = bNode;
		}
		Node* aNodeRight = bNode->right();
		if (aNodeRight == aNode)
		{
			aNodeRight = bNode;
		}
		Node* aNodeParent = bNode->parent();
		if (aNodeParent == aNode)
		{
			aNodeParent = bNode;
		}

		Node* bNodeLeft = aNode->left();
		if (bNodeLeft == bNode)
		{
			bNodeLeft = aNode;
		}
		Node* bNodeRight = aNode->right();
		if (bNodeRight == bNode)
		{
			bNodeRight = aNode;
		}
		Node* bNodeParent = aNode->parent();
		if (bNodeParent == bNode)
		{
			bNodeParent = aNode;
		}

		if (bLeft)
		{
			linkLeft(aNodeParent, aNode);
		}
		else
		{
			linkRight(aNodeParent, aNode);
		}
		linkLeft(aNode, aNodeLeft);
		linkRight(aNode, aNodeRight);

		if (aLeft)
		{
			linkLeft(bNodeParent, bNode);
		}
		else
		{
			linkRight(bNodeParent, bNode);
		}
		linkLeft(bNode, bNodeLeft);
		linkRight(bNode, bNodeRight);
		
		const bool bRed = bNode->red();
		bNode->setRed(aNode->red());
		aNode->setRed(bRed);
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::rebalance(
		Node* toRebalance, bool leftLowOnBlack)
	{
		if (toRebalance == sentinel_)
		{
			// Nothing to do.
			return;
		}

		// Keep in mind when reading this that:
		// * All right children are black, per the convention
		// that all 3-nodes (one red child) are left-leaning 
		// and there are no 4-nodes (two red childs).
		// * There are no two consecutive red children.
		// * With respect to updating the hierarchical data,
		// 'node' is always considered changed.

		Node* parent = toRebalance;
		while(true)
		{
			// As an invariant, 'parent' is not the
			// sentinel node.

			Node* grandParent = parent->parent();

			// If there is a red node in the close neighborhood, then we can 
			// regain balance in constant time. If there is no such node, 
			// we can turn one node into a red node to decrement the 
			// black-depth of the other path also, such that the problem 
			// is propagated up the tree.

			if (leftLowOnBlack)
			{
				Node* sibling = parent->right();
				Node* node = parent->left();
				Node* left = sibling->left();
				Node* right = sibling->right();
				
				if (node->red())
				{
                    //   |B           |B    
                    //   p            p    
                    // R/ \B   ==>  B/ \B
					// n   s        n   s

					// Because 'node' is red, it is
					// not the sentinel node.
					node->flipColor();

					// The color of 'node' has changed.
					policy_.updateHierarchical(
						Iterator(node));
					// The left child of 'parent' has changed.
					policy_.updateHierarchical(
						Iterator(parent));
					break;
				}
				else if (sibling == sentinel_)
				{
					if (parent->red())
					{
						//   |R        |B
						//   p   ==>   p
						// B/        B/
						// n         n
						parent->flipColor();

						// The color of 'parent' has changed,
						// as well as its left child.
						policy_.updateHierarchical(
							Iterator(parent));
						break;
					}
					else
					{
						//   |B 
						//   p  
						// B/   
						// n    

						// Propagate the problem up the tree.

						// The left child of 'parent' has changed.
						policy_.updateHierarchical(
							Iterator(parent));
					}
				}
				else if (left->red())
				{
					// Note: because of the testing order,
					// we know at this point that 'node' is
					// black.

					//   |B/R          |B/R     
                    //   p      r(s)   p
                    // B/ \B    ==>  B/ \B 
                    // n   s         n   l
                    //   R/ \B         B/ \R    
                    //   l   r             s	
                    // B/ \B             B/ \B
					//                       r
					//
					//           |B/R    flip(l)      |B/R
					// l(p)      l       switch(l)    l    
					// ==>   R/     \R   ==>      B/     \B  
					//       p       s            p       s  
					//     B/ \B   B/ \B        B/ \B   B/ \B
					//     n           r        n           r
					// 
					  
					// 'left' exists because it is red.
					// 'sibling' and exists because 'left' is 
					// its descendant.

					rotateRight(sibling);
					rotateLeft(parent);
					flipColors(left);
					left->flipColor();

					// The children of 'parent' have changed.
					policy_.updateHierarchical(
						Iterator(parent));
					// The children of 'sibling' have changed.
					policy_.updateHierarchical(
						Iterator(sibling));
					// The children of 'left' have changed.
					policy_.updateHierarchical(
						Iterator(left));
					break;
				}
				else if (parent->red())
				{
					// Note: because of the testing order,
					// we know at this point that 'left' is
					// black.

					//   |R            |R       
					//   p     l(p)    s
					// B/ \B   ==>   R/ \B
					// n   s         p   r 
					//   B/ \B     B/ \B
					//   l   r     n   l
					//
					//             |B
					// switch(s)   s
					// ==>       R/ \B
					//           p   r
					//         B/ \B
					//         n   l

					rotateLeft(parent);
					sibling->flipColor();

					// The children of 'parent' have changed.
					policy_.updateHierarchical(
						Iterator(parent));
					// The children of 'sibling' have changed.
					policy_.updateHierarchical(
						Iterator(sibling));
					break;
				}
				else
				{
					// There is no red node nearby.
					// In this case the problem is propagated
					// upwards in the tree by taking one black
					// out from the sibling path.

                    //   |B            |B       
                    //   p     l(p)    s
                    // B/ \B   ==>   R/ \B 
                    // n   s         p   r
                    //   B/ \B     B/ \B
					//   l   r     n   l

					rotateLeft(parent);

					// The children of 'parent' have changed
					// (as well as its color).
					policy_.updateHierarchical(
						Iterator(parent));
					// The children of 'sibling' have changed.
					policy_.updateHierarchical(
						Iterator(sibling));
				}
			}
			else
			{
				Node* sibling = parent->left();
				Node* left = sibling->left();
				Node* right = sibling->right();
				
				if (sibling == sentinel_)
				{
					if (parent->red())
					{
						//   |R        |B
						//   p   ==>   p
						//    \B        \B
						//     n         n
						parent->flipColor();

						// The color of 'parent' has changed,
						// as well as its right child.
						policy_.updateHierarchical(
							Iterator(parent));
						break;
					}
					else
					{
						// |B 
						// p  
						//  \B   
						//   n    

						// Propagate the problem up the tree.

						// The right child of 'parent' has changed.
						policy_.updateHierarchical(
							Iterator(parent));
					}
				}
				else if (left->red())
				{
					//     |B/R         |B/R
					//     p     r(p)   s
					//   B/ \B   ==>  R/ \R
					//   s   n        l   p
					// R/ \B            B/ \B
					// l   r            r   n
					//
					// flip(s)     |B/R
					// switch(s)   s
					// ==>       B/ \B
					//           l   p
					//             B/ \B
					//             r   n

					rotateRight(parent);
					flipColors(sibling);
					sibling->flipColor();

					// The color of 'left' has changed.
					policy_.updateHierarchical(
						Iterator(left));
					// The children of 'parent' have changed.
					policy_.updateHierarchical(
						Iterator(parent));
					// The children of 'sibling' have changed.
					policy_.updateHierarchical(
						Iterator(sibling));
					break;
				}
				else if (parent->red())
				{
					// Note: because of the testing order,
					// we know at this point that 'left' is
					// black.

					//     |R    switch(s)     |B    
					//     p     switch(p)     p  
					//   B/ \B   ==>         R/ \B
					//   s   n               s   n
					// B/ \B               B/ \B  
					// l   r               l   r  

					sibling->flipColor();
					parent->flipColor();

					// The color of 'sibling' has changed.
					policy_.updateHierarchical(
						Iterator(sibling));
					// The color of 'parent' has changed
					// (as well as its both children).
					policy_.updateHierarchical(
						Iterator(parent));
					break;
				}
				else if (sibling->red())
				{
					Node* rightLeft = right->left();

					if (rightLeft->black())
					{
						//     |B           |B  
						//     p     r(p)   s
						//   R/ \B   ==>  B/ \R
						//   s   n        l   p
						// B/ \B            B/ \B
						// l   r            r   n
						//   B/ \B        B/ \B
						//
						//  switch(p)   |B  
						//  switch(r)   s  
						//  ==>       B/ \B
						//            l   p
						//              R/ \B
						//              r   n
						//            B/ \B

						rotateRight(parent);
						parent->flipColor();
						if (right != sentinel_)
						{
							right->flipColor();

							// The color of 'right' has changed.
							policy_.updateHierarchical(
								Iterator(right));
						}
						// The color of 'parent' has changed
						// (as well as its both children).
						policy_.updateHierarchical(
							Iterator(parent));
						// The right child of 'sibling' has
						// changed.
						policy_.updateHierarchical(
							Iterator(sibling));
					}
					else
					{
                        //     |B               |B  
                        //     p     l(s)       p
                        //   R/ \B   ==>      B/ \B
                        //   s   n    	      r   n
                        // B/ \B            R/ \B   
                        // l   r            s   b     
                        //   R/ \B        B/ \R
                        //   a   b        l   a
                        //
                        //                |B  
                        //   r(p)         r
                        //   ==>      R/     \R
                        //            s       p
                        //          B/ \R   B/ \B
                        //          l   a   b   n
                        //
                        // switch(p)     |B  
                        // switch(a)     r
                        // ==>       R/     \B
                        //           s       p
                        //         B/ \B   B/ \B
                        //         l   a   b   n

						// 'rightLeft' exists because it is red.
						// 'right' and 'sibling' exist because 
						// 'rightLeft' exists.

						rotateLeft(sibling);
						rotateRight(parent);
						parent->flipColor();
						rightLeft->flipColor();

						// The color 'rightLeft' has changed.
						policy_.updateHierarchical(
							Iterator(rightLeft));
						// The right child of 'sibling' has changed.
						policy_.updateHierarchical(
							Iterator(sibling));
						// The children of 'parent' has changed.
						policy_.updateHierarchical(
							Iterator(parent));
						// The children of 'right' have changed.
						policy_.updateHierarchical(
							Iterator(right));
					}
					break;
				}
				else
				{
					// There is no red node nearby.
					// Here we propagate the
					// problem up the tree by taking
					// one black out from the left
					// path.
					
					//     |B                  |B 
					//     p     switch(s)     p  
					//   B/ \B   ==>         R/ \B
					//   s   n               s   n
					// B/ \B               B/ \B  
					// l   r               l   r  

					sibling->setRed();

					// The color of 'sibling' has changed.
					policy_.updateHierarchical(
						Iterator(sibling));
					// The children of 'parent' have changed.
					policy_.updateHierarchical(
						Iterator(parent));
				}
			}

			if (grandParent == sentinel_)
			{
				break;
			}

			leftLowOnBlack = 
				(grandParent->left() == parent);
			parent = grandParent;
		}

		if (parent->parent() == sentinel_)
		{
			root_ = parent;
		}
		else
		{
			updateToRoot(parent->parent());
		}
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Node* 
		RedBlackTree<Key, Compare, RbtPolicy>::copyConstruct(
		Node* parent, Node* thatNode)
	{
		if (thatNode->sentinel())
		{
			return sentinel_;
		}

		integer rollBackIndex = 0;
		Node* node = 0;
		try
		{
			node = allocateNode(
				thatNode->key(), thatNode->valuePtr(), 
				parent, thatNode->red());
			++rollBackIndex;

			// Copy construct the left child.
			node->left() = copyConstruct(node, thatNode->left());
			++rollBackIndex;

			// Copy construct the right child.
			node->right() = copyConstruct(node, thatNode->right());
			++rollBackIndex;
		}
		catch(...)
		{
			switch(rollBackIndex)
			{
			case 3:
				clear(node->right());
				// Fall-through.
			case 2:
				clear(node->left());
				// Fall-through.
			case 1:
				// No need to deallocate 'node',
				// we will do it later at the
				// copy constructor all at once.
				break;
			};

			throw;
		}

		return node;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::clear(
		Node* node)
	{
		// The sentinel should not be destructed
		// by clear().
		if (node == sentinel_)
		{
			return;
		}

		clear(node->left());
		clear(node->right());
		
		destructNode(node);

		// The node deallocation is left to
		// be done at higher level all at once.
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::linkLeft(
		Node* parent, Node* child)
	{
		if (parent != sentinel_)
		{
			parent->left() = child;
		}
		else
		{
			root_ = child;
		}
		if (child != sentinel_)
		{
			child->parent() = parent;
		}
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::linkRight(
		Node* parent, Node* child)
	{
		if (parent != sentinel_)
		{
			parent->right() = child;
		}
		else
		{
			root_ = child;
		}
		if (child != sentinel_)
		{
			child->parent() = parent;
		}
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Node*
		RedBlackTree<Key, Compare, RbtPolicy>::rotateLeft(
		Node* node)
	{
		ASSERT(node != sentinel_);
		ASSERT(node->right() != sentinel_);

		Node* parent = node->parent();
		Node* x = node->right();

		if (node == parent->left())
		{
			linkLeft(parent, x);
		}
		else
		{
			linkRight(parent, x);
		}

		linkRight(node, x->left());
		linkLeft(x, node);

		x->setRed(node->red());
		node->setRed();

		return x;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Node*
		RedBlackTree<Key, Compare, RbtPolicy>::rotateRight(
		Node* node)
	{
		ASSERT(node != sentinel_);
		ASSERT(node->left() != sentinel_);

		Node* parent = node->parent();
		Node* x = node->left();

		if (node == parent->left())
		{
			linkLeft(parent, x);
		}
		else
		{
			linkRight(parent, x);
		}

		linkLeft(node, x->right());
		linkRight(x, node);

		x->setRed(node->red());
		node->setRed();

		policy_.updateHierarchical(
			Iterator(node));
		policy_.updateHierarchical(
			Iterator(x));

		return x;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Node*
		RedBlackTree<Key, Compare, RbtPolicy>::moveRedLeft(
		Node* node)
	{
		ASSERT(node != sentinel_);
		ASSERT(node->left() != sentinel_);

		flipColors(node);
		if (node->right()->left()->red())
		{
			node->right() = rotateRight(node->right());
			node = node->rotateLeft(node);
			flipColors(node);
		}

		return node;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Node*
		RedBlackTree<Key, Compare, RbtPolicy>::moveRedRight(
		Node* node)
	{
		ASSERT(node != sentinel_);
		ASSERT(node->left() != sentinel_);

		flipColors(node);
		if (node->left()->left()->red())
		{
			node = node->rotateRight(node);
			flipColors(node);
		}

		return node;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::flipColors(
		Node* node)
	{
		ASSERT(node != sentinel_);

		node->flipColor();
		if (node->left() != sentinel_)
		{
			node->left()->flipColor();
		}
		if (node->right() != sentinel_)
		{
			node->right()->flipColor();
		}
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::setMinimum(
		Node* node)
	{
		minimum_ = node;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Node*
		RedBlackTree<Key, Compare, RbtPolicy>::minimum() const
	{
		return minimum_;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::setMaximum(
		Node* node)
	{
		sentinel_->parent() = node;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Node*
		RedBlackTree<Key, Compare, RbtPolicy>::maximum() const
	{
		return sentinel_->parent();
	}

}

#endif
