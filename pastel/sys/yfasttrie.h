// Description: Y-fast trie

#ifndef PASTELSYS_YFASTTRIE_H
#define PASTELSYS_YFASTTRIE_H

#include "pastel/sys/tree.h"
#include "pastel/sys/number_tests.h"

#include <bitset>
#include <array>

namespace Pastel
{

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
	template <int N>
	class YFastTrie
	{
	public:
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

		typedef std::bitset<N> Integer;
		typedef std::unordered_set<Integer> IntegerSet;

		typedef Tree<Integer> IntegerTree;
		typedef typename IntegerTree::ConstIterator
			ConstIterator;
		typedef typename IntegerTree::Iterator
			Iterator;

		//! Inserts an element.
		/*!
		Time complexity:
		O(log(log(Delta))) expected amortized time,	where 
		Delta = that - *lower_bound(that)
		*/
		ConstIterator insert(Integer that)
		{
			ConstIterator newSubtree;

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

		IntegerTree tree_;
		std::array<IntegerSet, N> elementSetSet_;
	};

}

#endif
