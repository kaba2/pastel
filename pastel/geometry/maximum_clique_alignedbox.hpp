#ifndef PASTEL_MAXIMUM_CLIQUE_ALIGNEDBOX_HPP
#define PASTEL_MAXIMUM_CLIQUE_ALIGNEDBOX_HPP

#include "pastel/geometry/maximum_clique_alignedbox.h"

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/countingiterator.h"

namespace Pastel
{

	namespace MaximumCliqueAlignedBox_Detail
	{

		template <typename Real>
		class Event
		{
		public:
			Event()
				: position(0)
				, index(0)
				, min(true)
			{
			}

			Event(const Real& position_,
				integer index_,
				bool min_)
				: position(position_)
				, index(index_)
				, min(min_)
			{
			}

			bool operator<(
				const Event& that) const
			{
				if (position < that.position)
				{
					return true;
				}
				if (that.position < position)
				{
					return false;
				}

				if (min && !that.min)
				{
					return true;
				}
				if (that.min && !min)
				{
					return false;
				}

				return index < that.index;
			}

			Real position;
			integer index;
			bool min;
		};

		class MaximumClique_RbtPolicy
		{
		public:
			class ValueType
			{
			public:
				ValueType(
					integer actives_ = 0,
					integer maxCliqueSize_ = 0)
					: actives(actives_)
					, maxCliqueSize(maxCliqueSize_)
				{
				}

				integer actives;
				integer maxCliqueSize;
			};

			template <typename Iterator>
			void updateInsertDown(const Iterator& iter)
			{
			}

			template <typename Iterator>
			void updateHierarchical(const Iterator& iter)
			{
				const integer v = iter->key().min ? 1 : -1;

				iter->value().actives = 
					iter.left()->value().actives + 
					iter.right()->value().actives +
					v;

				iter->value().maxCliqueSize = 
					std::max(iter.left()->value().maxCliqueSize,
					std::max(
					iter.left()->value().actives + v,
					iter.left()->value().actives + v + 
					iter.right()->value().maxCliqueSize));
			}

			void swap(MaximumClique_RbtPolicy& that)
			{
			}
		};

	}

	template <typename Real>
	AlignedBox<Real, 2> maximumClique(
		const std::vector<AlignedBox<Real, 2> >& boxSet)
	{
		typedef MaximumCliqueAlignedBox_Detail::Event<Real> Event;
		typedef MaximumCliqueAlignedBox_Detail::MaximumClique_RbtPolicy RbtPolicy;
		typedef RedBlackTree<Event, std::less<Event>, RbtPolicy> Tree;
		typedef typename Tree::ConstIterator ConstIterator;
		typedef RbtPolicy::ValueType Value;
		typedef std::vector<AlignedBox<Real, 2> > BoxSet;

		// This is a sweepline algorithm to compute an
		// aligned box of maximum overlap in a set of aligned 
		// boxes.

		const integer n = boxSet.size();

		// Insert the y-endpoints of each box
		// into an event list.
		std::vector<Event> eventSet;
		eventSet.reserve(2 * n);
		for (integer i = 0;i < n;++i)
		{
			const AlignedBox<Real, 2>& box = boxSet[i];

			eventSet.push_back(
				Event(box.min().y(), i, true));
			eventSet.push_back(
				Event(box.max().y(), i, false));
		}

		// Sort the event list lexicographically in the
		// following order of priority:
		// 1) y is non-decreasing
		// 2) minimum comes before maximum
		// 3) index increases
		std::sort(eventSet.begin(), eventSet.end());

		Tree tree;

		Real yMin = 0;
		Real yMax = 0;
		integer maxMaxCliqueSize = 0;
		integer maxIndex = 0;

		for (integer j = 0;j < 2;++j)
		{
			// The first time in this loop (j == 0), our 
			// aim is to find the _number_ of boxes in the
			// maximum clique.

			// The second time in this loop (j == 1), our 
			// aim is to reconstruct the tree where the
			// maximum clique was found.

			tree.clear();

			for (integer i = 0;i < n;++i)
			{
				// The events come out in increasing y-order. 
				// An event either begins a new box or ends 
				// an existing box.
				Event& e = eventSet[i];

				// Find out the extremities of the box
				// in the _x_ direction.
				const Event minEvent(
					boxSet[e.index].min().x(), e.index, true);
				const Event maxEvent(
					boxSet[e.index].max().x(), e.index, false);

				if (e.min)
				{
					// If this event begins a new box, then
					// we insert the x-endpoints of that box 
					// into the tree.
					tree.insert(minEvent);
					tree.insert(maxEvent);
					
					if (j == 0)
					{
						// See if the size of the maximum clique grew.
						if (tree.root()->value().maxCliqueSize > maxMaxCliqueSize)
						{
							maxMaxCliqueSize = tree.root()->value().maxCliqueSize;
							maxIndex = i;
						}
					}
					else if (i == maxIndex)
					{
						// We have now reconstructed a tree which 
						// contains a maximum clique rectangle.
						// Although there could be several maximum 
						// cliques, we are content with the first 
						// one (this also saves some computation).
						yMin = e.position;
						yMax = eventSet[i + 1].position;
						break;
					}
				}
				else
				{
					// If this event ends an existing box, then
					// we remove that box from the tree.
					tree.erase(minEvent);
					tree.erase(maxEvent);
				}
			}

			if (maxMaxCliqueSize == 1)
			{
				// There are no non-singular cliques.
				// That is, all boxes are separated.
				break;
			}
		}

		AlignedBox<Real, 2> clique;

		if (maxMaxCliqueSize > 1)
		{
			// We have now reconstructed a tree which 
			// contains a maximum clique rectangle.
			// We already know its y-range. Now we need
			// to find its x-range.

			ConstIterator iter = tree.root();
			while(true)
			{
				ASSERT(!iter.sentinel());

				const integer v = iter->key().min ? 1 : -1;

				if (iter->value().maxCliqueSize == 
					iter.left()->value().actives + v)
				{
					// This is a maximum clique node.
					break;
				}
				else if (iter->value().maxCliqueSize == 
					iter.left()->value().maxCliqueSize)
				{
					iter = iter.left();
				}
				else
				{
					iter = iter.right();
				}
			}

			const Real xMin = iter->key().position;
			++iter;
			const Real xMax = iter->key().position;

			clique.set(xMin, yMin, xMax, yMax);
		}

		return clique;
	}	

}

#endif
