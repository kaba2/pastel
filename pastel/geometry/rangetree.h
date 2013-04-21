// Description: Range tree

#ifndef PASTELGEOMETRY_RANGETREE_H
#define PASTELGEOMETRY_RANGETREE_H

#include "pastel/geometry/rangetree_concepts.h"

#include <boost/range/algorithm/sort.hpp>

#include <memory>
#include <vector>

namespace Pastel
{

	template <typename RangeTree_Types>
	class RangeTree
	{
	public:
		typedef typename RangeTree_Types::PointRep PointRep;
		typedef typename PointRep::Real Real;
		typedef typename PointRep::Point Point;
		typedef typename std::unique_ptr<RangeTree> TreePtr;
		typedef std::vector<Point> PointSet;
		typedef typename PointSet::iterator Point_Iterator;
		typedef typename PointSet::const_iterator Point_ConstIterator;

		explicit RangeTree(const PointRep& pointRep)
			: pointRep_(pointRep)
			, maxBucketSize_(8)
		{
		}

		class Node
		{
		public:
			explicit Node(
				TreePtr&& nextTree,
				bool leaf)
				: nextTree_(std::move(nextTree))
				, leaf_(leaf)
			{
			}

			bool leaf() const
			{
				return leaf_;
			}

		private:
			// Next axis.
			TreePtr nextTree_;
			// Whether this is a leaf node 
			// (and not a split node).
			bool leaf_;
		};

		struct Node_Delete
		{
			void operator()(Node* node)
			{
				if (node->leaf())
				{
					delete (LeafNode*)node;
				}
				else
				{
					delete (SplitNode*)node;
				}
			}
		};

		typedef std::unique_ptr<Node, Node_Delete> NodePtr;

		class SplitNode
			: public Node
		{
		public:
			SplitNode(
				Real splitPosition,
				NodePtr&& left,
				NodePtr&& right,
				TreePtr&& nextTree)
				: Node(std::move(nextTree), false)
				, splitPosition_(splitPosition)
				, left_(std::move(left))
				, right_(std::move(right))
			{
			}

			// Split position on the current axis.
			Real splitPosition_;
			// Points with position < splitPosition_.
			NodePtr left_;
			// Points with position >= splitPosition_.
			NodePtr right_;
		};

		class LeafNode
			: public Node
		{
		public:
			LeafNode(
				integer begin,
				integer end,
				TreePtr&& nextTree)
				: Node(std::move(nextTree), true)
				, begin_(begin)
				, end_(end)
			{
			}

			integer begin_;
			integer end_;
		};

		template <typename Point_Input>
		void construct(Point_Input pointInput)
		{
			construct(pointInput, 0);
		}

	private:
		template <typename Point_Input>
		void construct(Point_Input pointInput, integer axis)
		{
			pointSet_.reserve(pointInput.nHint());
			while(!pointInput.empty())
			{
				Point point = pointInput();
				pointSet_.push_back(point);
			}
			
			integer n = pointSet_.size();
			root_ = construct(0, n, axis);
		}

		NodePtr construct(integer begin, integer end, integer axis)
		{
			ASSERT_OP(begin, <=, end);
			ASSERT_OP(axis, >=, 0);
			ASSERT_OP(axis, <, pointRep_.n());

			TreePtr nextTree;
			if (axis < pointRep_.n() - 1)
			{
				nextTree.reset(new RangeTree(pointRep_));
				nextTree->construct(
					rangeInput(
					pointSet_.begin() + begin,
					pointSet_.begin() + end), axis + 1);
			}

			NodePtr node;
			integer n = end - begin;
			if (n <= maxBucketSize_)
			{
				// Create a leaf node.				
				node.reset(new LeafNode(begin, end, std::move(nextTree)));
			}
			else
			{
				// Split the node into two.

				auto coordinateLess = 
					[&](const Point& left, const Point& right)
				{
					return pointRep_(left, axis) < pointRep_(right, axis);
				};

				integer iMedian = begin + n / 2;
				std::nth_element(
					pointSet_.begin() + begin, 
					pointSet_.begin() + iMedian, 
					pointSet_.begin() + end, 
					coordinateLess);

				Real median = pointRep_(pointSet_[iMedian], axis);

				NodePtr left;
				if (begin != iMedian)
				{
					left = construct(
						begin, iMedian, axis);
				}

				NodePtr right;
				if (iMedian != end)
				{
					right = construct(
						iMedian, end, axis);
				}

				node.reset(new SplitNode(
					median, 
					std::move(left), std::move(right), 
					std::move(nextTree)));
			}
		
			return node;
		}

		PointSet pointSet_;
		NodePtr root_;
		PointRep pointRep_;
		integer maxBucketSize_;
	};

}

#include "pastel/geometry/rangetree.hpp"

#endif
