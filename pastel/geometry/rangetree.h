// Description: Range tree

#ifndef PASTELGEOMETRY_RANGETREE_H
#define PASTELGEOMETRY_RANGETREE_H

#include "pastel/geometry/rangetree_concepts.h"
#include "pastel/geometry/rangetree_fwd.h"
#include "pastel/sys/locator_concept.h"
#include "pastel/sys/vector.h"

#include <boost/range/algorithm/sort.hpp>

#include <memory>
#include <vector>

namespace Pastel
{

	template <typename Settings>
	using Empty_RangeTree_Customization = 
		RangeTree_Concepts::Customization<Settings>;

	//! Range tree
	/*!
	Space complexity: O(n log(n))
	*/
	template <
		typename Settings,
		template <typename> class Customization = Empty_RangeTree_Customization>
	class RangeTree
	{
	public:
		using Fwd = RangeTree_Fwd<Settings>;
		PASTEL_FWD(PointSet);
		PASTEL_FWD(Point_Iterator);
		PASTEL_FWD(Point_ConstIterator);
		PASTEL_FWD(Point);
		PASTEL_FWD(Entry);
		PASTEL_FWD(Node);
		PASTEL_FWD(Real);

		RangeTree()
		: end_(new Node)
		, root_(end_.get())
		, pointSet_()
		{
		}

		/*
		template <
			typename Type, 
			typename = PASTEL_ENABLE_IF((std::is_convertible<Type, Point>), void)>
		RangeTree(const std::initializer_list<Type>& pointSet)
		: RangeTree()
		{
			construct(pointSet)
		}
		*/

		template <
			typename Point_Range_,
			typename Locator>
		RangeTree(
			const Point_Range_& pointSet,
			const Locator& locator)
		: RangeTree()
		{
			std::vector<Vector<Real, 2>> coordinateSet;
			coordinateSet.reserve(boost::size(pointSet));

			for (auto&& point : pointSet)
			{
				coordinateSet.emplace_back(
					locator(point, 0),
					locator(point, 1));
			}

			auto yLess = [](
				const Vector<Real, 2>& left, 
				const Vector<Real, 2>& right)
			-> bool
			{
				return left[1] < right[1];
			};

			boost::sort(coordinateSet, yLess);

			root_ = construct(coordinateSet);
		}

		~RangeTree()
		{
			clear();
		}

		void swap(RangeTree& that)
		{
			end_.swap(that.end_);
			std::swap(root_, that.root_);
			elementSet_.swap(that.elementSet_);
		}

		void clear()
		{
			clear(root_);
		}

	private:
		Node* construct(const std::vector<Vector<Real, 2>>& pointSet)
		{

		}

		void clear(Node* node)
		{
			if (node == end_.get())
			{
				return;
			}
			
			clear(node->child(false));
			clear(node->child(true));
			delete node;
		}

		std::unique_ptr<Node> end_;
		Node* root_;
		PointSet pointSet_;
	};

}

#endif
