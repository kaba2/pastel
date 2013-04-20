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

		explicit RangeTree(const PointRep& pointRep)
			: pointRep_(pointRep)
		{
		}

		class Node;
		typedef std::unique_ptr<Node> NodePtr;

		class Node
		{
		public:
			Node(Real splitPosition,
				NodePtr&& left,
				NodePtr&& right)
				: splitPosition_(splitPosition)
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

			// Next axis.
			Node* down_;
		};

		template <typename Point_Input>
		void construct(Point_Input pointInput)
		{
			pointSet_.reserve(pointInput.nHint());
			while(!pointInput.empty())
			{
				Point point = pointInput();
				pointSet_.push_back(point);
			}
			
			integer n = pointSet_.size();
			root_ = construct(0, n, 0);
		}
	
	private:
		NodePtr construct(integer begin, integer end, integer axis)
		{
			ASSERT_OP(begin, <=, end);
			ASSERT_OP(axis, >=, 0);
			ASSERT_OP(axis, <, pointRep_.n());

			auto coordinateLess = [&](const Point& left, const Point& right)
			{
				return pointRep_(left, axis) < pointRep_(right, axis);
			};

			integer iMedian = begin + (end - begin) / 2;
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

			NodePtr node(new Node(median, std::move(left), std::move(right)));
			
			return node;
		}

		std::vector<Point> pointSet_;
		NodePtr root_;
		PointRep pointRep_;
	};

}

#include "pastel/geometry/rangetree.hpp"

#endif
