#ifndef PASTEL_BIHTREE_MORE_H
#define PASTEL_BIHTREE_MORE_H

#include "pastel/geometry/bihtree.h"

#include "pastel/sys/mytypes.h"

#include <boost/operators.hpp>

namespace Pastel
{

	template <typename Real, int N, typename Shape>
	class BihTree<Real, N, Shape>::Node
	{
	public:
		Node(
			Node* children,
			integer splitAxisOrLeafIndex,
			const Real& negativeSplit,
			const Real& positiveSplit)
			: children_(children)
			, splitAxisOrLeafIndex_(splitAxisOrLeafIndex)
			, negativeSplit_(negativeSplit)
			, positiveSplit_(positiveSplit)
		{
		}

		bool isLeaf() const
		{
			return (children_ == 0);
		}

		Node* negative() const
		{
			return children_;
		}

		Node* positive() const
		{
			Node* result = 0;
			if (children_)
			{
				result = children_ + 1;
			}
			return result;
		}

		const Real& negativeSplit() const
		{
			return negativeSplit_;
		}

		const Real& positiveSplit() const
		{
			return positiveSplit_;
		}

		integer splitAxis() const
		{
			return splitAxisOrLeafIndex_;
		}

	private:
		Node* children_;
		integer splitAxisOrLeafIndex_;
		Real negativeSplit_;
		Real positiveSplit_;
	};

	template <typename Real, int N, typename Shape>
	class BihTree<Real, N, Shape>::Leaf
	{
	public:
		Leaf(integer shapeIndex,
			integer shapes)
			: shapeIndex_(shapeIndex)
			, shapes_(shapes)
		{
		}

		integer shapeIndex() const
		{
			return shapeIndex_;
		}

		integer shapes() const
		{
			return shapes_;
		}

	private:
		integer shapeIndex_;
		integer shapes_;
	};

	template <typename Real, int N, typename Shape>
	class BihTree<Real, N, Shape>::ConstCursor
		: boost::less_than_comparable<
			typename BihTree<Real, N, Shape>::ConstCursor
			, boost::equality_comparable<
			typename BihTree<Real, N, Shape>::ConstCursor
			> >
	{
	public:
		friend class BihTree<Real, N, Shape>;

		// Using default copy constructor
		// Using default assignment
		// Using default destructor

		ConstCursor()
			: node_(0)
		{
		}

		bool empty() const
		{
			return (node_ == 0);
		}

		bool isLeaf() const
		{
			bool result = false;
			if (node_)
			{
				result = node_->isLeaf();
			}
			return result;
		}

		const Real& negativeSplit() const
		{
			return node_->negativeSplit();
		}

		const Real& positiveSplit() const
		{
			return node_->positiveSplit();
		}

		integer splitAxis() const
		{
			return node_->splitAxis();
		}

		void nextNegative()
		{
			PENSURE(node_);

			Node* next = node_->negative();

			if (next)
			{
				node_ = next;
			}
		}

		void nextPositive()
		{
			PENSURE(node_);

			Node* next = node_->positive();

			if (next)
			{
				node_ = next;
			}
		}

		bool operator==(const ConstCursor& that) const
		{
			return node_ == that.node_;
		}

		bool operator<(const ConstCursor& that) const
		{
			return node_ < that.node_;
		}

	private:
		explicit ConstCursor(Node* node)
			: node_(node)
		{
		}

		Node* getNode() const
		{
			return node_;
		}

		Node* node_;
	};

}

#endif
