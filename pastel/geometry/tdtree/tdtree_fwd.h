// Description: Temporal kd-tree

#ifndef PASTELGEOMETRY_TDTREE_FWD_H
#define PASTELGEOMETRY_TDTREE_FWD_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/range.h"
#include "pastel/sys/locator/locator_concept.h"

#include <vector>

namespace Pastel
{

	template <typename, template <typename> class>
	class TdTree;

	template <typename Settings>
	class TdTree_Fwd
	{
	public:
		using Fwd = Settings;
		PASTEL_FWD(Locator);

		PASTEL_CONCEPT_CHECK(Locator, Locator_Concept);

		//! The type of the contained points.
		/*!
		We denote the set of points by P.
		*/
		using Point = Locator_Point<Locator>;

		//! The type of the point-coordinates.
		using Real = Locator_Real<Locator>;

		//! The compile-time dimension of the points.
		static constexpr int N = Locator_N<Locator>::value;

		//! Temporal point
		/*!
		A temporal point is a pairing of a position
		and a time-instant.
		*/
		class Temporal_Point
		{
		public:
			//! Constructs with default values.
			Temporal_Point()
			: point_()
			, time_()
			{
			}

			//! Constructs with given position and time-instant.
			Temporal_Point(
				const Point& point,
				const Real& time)
			: point_(point)
			, time_(time)
			{
			}

			//! Swaps two temporal points.
			void swap(Temporal_Point& that)
			{
				using std::swap;
				swap(point_, that.point_);
				swap(time_, that.time_);
			}

			//! Returns the position.
			const Point& point() const
			{
				return point_;
			}

			//! Returns the time-instant.
			const Real& time() const
			{
				return time_;
			}

		private:
			Point point_;
			Real time_;
		};

		//! Storage for the temporal points.
		/*!
		The temporal points are stored in an array. The storage is an 
		array, because we need indexing for the fractional cascading.
		The entries in each node refer to this point-array by indices.
		*/
		using PointSet = 
			std::vector<Temporal_Point>;
		using Point_Iterator = 
			typename PointSet::iterator;
		using Point_ConstIterator = 
			typename PointSet::const_iterator;

		using Iterator = Point_Iterator;
		using ConstIterator = Point_ConstIterator;

		//! A node.
		/*!
		Each node Q is associated with 
		* a closed rectangular subset C(Q) of R^d,
		* a node L(Q), the left child-node, such that C(L(Q))) subset C(Q),
		* a node R(Q), the right child-node, such that C(R(Q)) subset C(Q),
		* a set of points P(Q), such that P(Q) = intersect {P(Q), P, C(Q)}.

		It holds that [C(L(Q)) union C(R(Q))] = C(Q).
		*/
		class Node;

		//! An entry.
		/*!
		An entry is a pairing of a point-iterator and two fractional
		cascading indices; one index for each child-node.
		*/
		class Entry;

		//! A cursor.
		/*!
		A cursor is used to identify a node, and to move between nodes.
		*/
		class Cursor;

		//! Storage for the entry-set of a node.
		/*!
		Each node stores, in an array, a set of entries. 
		Given an index-interval [i_0, i_1), the 
		index-interval for the left child-node is given by
		[L(i_0), L(i_1)). Similarly for the right child-node.
		*/
		using EntrySet = std::vector<Entry>;
		using Entry_Iterator = typename EntrySet::iterator;
		using Entry_ConstIterator = typename EntrySet::const_iterator;
		using Entry_Range = ranges::subrange<Entry_Iterator>;
		using Entry_ConstRange = ranges::subrange<Entry_ConstIterator>;
	};

}

namespace Pastel
{

	//! Returns whether Type is an instance of TdTree.
	template <typename Type>
	struct IsTdTree
	: std::false_type
	{};

	template <typename Settings,
		template <typename> class Customization>
	struct IsTdTree<TdTree<Settings, Customization>>
	: std::true_type
	{};

}

namespace Pastel
{

	template <typename TdTree>
	using TdTree_Locator = typename TdTree::Locator;

}

#endif
