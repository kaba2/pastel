#ifndef PASTEL_POINTKDTREE_NODE_H
#define PASTEL_POINTKDTREE_NODE_H

#include "pastel/geometry/pointkdtree.h"

namespace Pastel
{

	template <typename Real, int N, typename ObjectPolicy>
	class PointKdTree<Real, N, ObjectPolicy>::Node
	{
	public:
		Node(
			Node* parent,
			Node* right,
			Node* left,
			const ConstObjectIterator& first,
			const ConstObjectIterator& last,
			integer objects,
			integer splitAxis,
			const Real& splitPosition,
			const Real& min,
			const Real& max)
			: parent_(parent)
			, right_(right)
			, left_(left)
			, first_(first)
			, last_(last)
			, objects_(objects)
			, splitAxis_(splitAxis)
			, splitPosition_(splitPosition)
			, min_(min)
			, max_(max)
		{
			ASSERT_OP(objects, >=, 0);
			ASSERT_OP(splitAxis, >=, 0);
			ASSERT(min <= splitPosition);
			ASSERT(max >= splitPosition);
		}

		// Tree

		Node* parent() const
		{
			return parent_;
		}

		void setRight(Node* right)
		{
			right_ = right;
		}

		Node* right() const
		{
			return right_;
		}

		void setLeft(Node* left)
		{
			left_ = left;
		}

		Node* left() const
		{
			return left_;
		}

		void setBucket(Node* bucket)
		{
			ASSERT(leaf());
			ASSERT(bucket);
			bucket_ = bucket;
		}

		Node* bucket() const
		{
			ASSERT(leaf());
			return bucket_;
		}

		bool leaf() const
		{
			return left_ == 0;
		}

		bool isBucket() const
		{
			if (objects_ == 0)
			{
				return leaf();
			}

			return (this == first_->leaf().node_->bucket_);
		}

		// Objects

		void setFirst(const ConstObjectIterator& first)
		{
			first_ = first;
		}

		const ConstObjectIterator& first() const
		{
			return first_;
		}

		void setLast(const ConstObjectIterator& last)
		{
			last_ = last;
		}

		const ConstObjectIterator& last() const
		{
			return last_;
		}

		ConstObjectIterator end() const
		{
			ConstObjectIterator result = last_;
			if (objects_ > 0)
			{
				++result;
			}

			return result;
		}

		void setObjects(integer objects)
		{
			ASSERT_OP(objects, >=, 0);

			objects_ = objects;
		}

		integer objects() const
		{
			return objects_;
		}

		bool empty() const
		{
			return objects_ == 0;
		}

		void erase(
			const ConstObjectIterator& iter,
			const ConstObjectIterator& end)
		{
			if (iter == first_)
			{
				if (iter == last_)
				{
					first_ = end;
					last_ = end;
				}
				else
				{
					++first_;
				}
			}
			else if (iter == last_)
			{
				--last_;
			}
			--objects_;
		}

		void insert(
			const ConstObjectIterator& first,
			const ConstObjectIterator& last,
			integer objects,
			const ConstObjectIterator& end)
		{
			first_ = first;

			if (last_ == end)
			{
				// If there are currently no
				// objects in the node, set the 'last' iterator.
				last_ = last;
			}

			objects_ += objects;
		}

		// Splitting plane

		void setSplitAxis(integer splitAxis)
		{
			splitAxis_ = splitAxis;
		}

		integer splitAxis() const
		{
			return splitAxis_;
		}

		void setSplitPosition(const Real& splitPosition)
		{
			splitPosition_ = splitPosition;
		}

		const Real& splitPosition() const
		{
			return splitPosition_;
		}

		// Bounds

		void setMin(const Real& min)
		{
			min_ = min;
		}

		const Real& min() const
		{
			return min_;
		}

		void setMax(const Real& max)
		{
			max_ = max;
		}

		const Real& max() const
		{
			return max_;
		}

	private:
		// Tree

		Node* parent_;
		union
		{
			Node* right_;
			Node* bucket_;
		};
		Node* left_;

		// Objects

		ConstObjectIterator first_;
		ConstObjectIterator last_;
		uint32 objects_;

		// Splitting plane

		uint32 splitAxis_;
		Real splitPosition_;

		// Bounds

		Real min_;
		Real max_;
	};

}

#endif
