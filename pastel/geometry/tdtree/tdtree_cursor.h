// Description: Temporal kd-tree cursor

#ifndef PASTELGEOMETRY_TDTREE_CURSOR_H
#define PASTELGEOMETRY_TDTREE_CURSOR_H

#include "pastel/geometry/tdtree/tdtree.h"
#include "pastel/geometry/tdtree/tdtree_fwd.h"

#include "pastel/sys/range/transformed_range.h"

#include <compare>

namespace Pastel
{

	template <typename Settings>
	class TdTree_Fwd<Settings>::Cursor
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		Cursor()
			: node_(0)
		{
		}

		auto operator<=>(const Cursor& that) const = default;

		explicit operator bool() const
		{
			return node_ != 0;
		}

		// Tree

		Cursor parent() const
		{
			PENSURE(node_);
			return Cursor(node_->parent());
		}

		Cursor child(bool right) const
		{
			PENSURE(node_);
			return Cursor(node_->child(right));
		}

		Cursor right() const
		{
			PENSURE(node_);
			return Cursor(node_->child(true));
		}

		Cursor left() const
		{
			PENSURE(node_);
			return Cursor(node_->child(false));
		}

		bool leaf() const
		{
			PENSURE(node_);
			return node_->leaf();
		}

		// Points

		decltype(auto) pointSet(integer min, integer max) const
		{
			ENSURE_OP(min, >=, 0);
			ENSURE_OP(max, <=, points());
			ENSURE_OP(min, <=, max);

			Entry_ConstRange fullRange = 
				node_->entryRange();

			return transformRange(
				range(
					std::begin(fullRange) + min,
					std::begin(fullRange) + max
				),
				[](const Entry& entry)
				{
					return entry.point();
				}
			);
		}

		integer points() const
		{
			PENSURE(node_);

			return node_->points();
		}

		bool empty() const
		{
			PENSURE(node_);

			return points() == 0;
		}

		// Splitting plane

		const Real& splitPosition() const
		{
			PENSURE(node_);
			return node_->splitPosition();
		}

		integer splitAxis() const
		{
			PENSURE(node_);
			return node_->splitAxis();
		}

		// Bounds

		const Real& min() const
		{
			PENSURE(node_);
			return node_->min();
		}

		const Real& max() const
		{
			PENSURE(node_);
			return node_->max();
		}

		const Real& prevMin() const
		{
			PENSURE(node_);
			return node_->prevMin();
		}

		const Real& prevMax() const
		{
			PENSURE(node_);
			return node_->prevMax();
		}

		// Fractional cascading

		integer cascade(integer index, bool right) const
		{
			ASSERT_OP(index, >=, 0);
			ASSERT_OP(index, <=, node_->entries());

			return node_->entryRange()[index].cascade(right);
		}

		// FIX: This should be private!
		// Moved here due to a bug in gcc 4.0.
		explicit Cursor(Node* node)
			: node_(node)
		{
		}

	private:
		template <typename, template <typename> class>
		friend class TdTree;

		Node* node_;
	};

}

#endif
