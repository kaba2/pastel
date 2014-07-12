// Description: Temporal kd-tree cursor

#ifndef PASTELGEOMETRY_TDTREE_CURSOR_H
#define PASTELGEOMETRY_TDTREE_CURSOR_H

#include "pastel/geometry/tdtree.h"
#include "pastel/geometry/tdtree_fwd.h"

#include "pastel/sys/range_input.h"
#include "pastel/sys/transform_input.h"

#include <boost/operators.hpp>

namespace Pastel
{

	template <typename Settings>
	class TdTree_Fwd<Settings>::Cursor
		: boost::less_than_comparable<Cursor
		, boost::equality_comparable<Cursor
		> >
	{
	public:
		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		Cursor()
			: node_(0)
		{
		}

		bool operator<(const Cursor& that) const
		{
			return node_ < that.node_;
		}

		bool operator==(const Cursor& that) const
		{
			return node_ == that.node_;
		}

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

		class EntryAsPoint
		{
		public:
			Point_ConstIterator operator()(const Entry& entry) const
			{
				return entry.point();
			}
		};

		using A_Input = Range_Input<Entry_ConstRange>;
		using B_Input = Transform_Input<A_Input, EntryAsPoint>;

		// FIX: Replace with decltype(auto) after Visual Studio 2013
		// fixes its bugs.
		B_Input pointSetAsInput() const
		{
			return transformInput(
				rangeInput(node_->entryRange()),
				EntryAsPoint());
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
