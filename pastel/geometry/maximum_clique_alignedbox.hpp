#ifndef PASTEL_MAXIMUM_CLIQUE_ALIGNEDBOX_HPP
#define PASTEL_MAXIMUM_CLIQUE_ALIGNEDBOX_HPP

#include "pastel/geometry/maximum_clique_alignedbox.h"

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/countingiterator.h"
#include "pastel/sys/nulliterator.h"

namespace Pastel
{

	namespace MaximumCliqueAlignedBox_Detail
	{

		template <typename Real, typename ConstIterator>
		class Event
		{
		public:
			Event()
				: position(0)
				, index(0)
				, min(true)
				, box()
			{
			}

			Event(const Real& position_,
				integer index_,
				bool min_,
				const ConstIterator& box_)
				: position(position_)
				, index(index_)
				, min(min_)
				, box(box_)
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

				// We would rather want to compare
				// the iterators, so that we didn't need
				// to store the 'index' field. However, only
				// random-access iterators are guaranteed
				// to have a less-than comparison.
				return index < that.index;
			}

			Real position;
			integer index;
			bool min;
			ConstIterator box;
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

	template <
		typename AlignedBox_ConstIterator,
		typename AlignedBox_ConstIterator_Iterator>
		typename std::iterator_traits<AlignedBox_ConstIterator>::value_type 
		maximumClique(const ForwardRange<AlignedBox_ConstIterator>& boxSet,
		AlignedBox_ConstIterator_Iterator result)
	{
		typedef typename std::iterator_traits<AlignedBox_ConstIterator>::
			value_type Box;
		typedef typename Box::Real_ Real;

		PASTEL_STATIC_ASSERT(Box::N_ == 2 || Box::N_ == Dynamic);

		typedef MaximumCliqueAlignedBox_Detail::
			Event<Real, AlignedBox_ConstIterator> Event;
		typedef MaximumCliqueAlignedBox_Detail::
			MaximumClique_RbtPolicy RbtPolicy;
		typedef RedBlackTree<Event, std::less<Event>, RbtPolicy> Tree;
		typedef typename Tree::ConstIterator Event_ConstIterator;
		typedef RbtPolicy::ValueType Value;

		// This is a sweepline algorithm to compute an
		// aligned box of maximum overlap in a set of aligned 
		// boxes. The paper
		//
		// "Finding the Connected Components and a Maximum Clique
		// of an Intersection Graph of Rectangles in the Plane",
		// Hiroshi Imai, Takao Asano, 
		// Journal of Algorithms, pp. 310-323, 1983.
		//
		// essentially describes the algorithm here. However, whereas
		// they augment 2-3-trees, we augment red-black trees. The
		// point here is to have a balanced tree to guarantee O(log n)
		// augmented insertion and deletion.

		// This will take linear time for an iterator
		// that is not random access. However, it is
		// worth it, since we can then reserve the size
		// of 'eventSet' below beforehand.
		const integer n = boxSet.size();

		// Insert the y-endpoints of each box
		// into an event list.
		std::vector<Event> eventSet;
		eventSet.reserve(2 * n);
		{
			AlignedBox_ConstIterator iter = boxSet.begin();
			const AlignedBox_ConstIterator iterEnd = boxSet.end();

			integer i = 0;
			while(iter != iterEnd)
			{
				PENSURE(iter->dimension() == 2);

				eventSet.push_back(
					Event(iter->min().y(), i, true, iter));
				eventSet.push_back(
					Event(iter->max().y(), i, false, iter));

				++i;
				++iter;
			}
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
					e.box->min().x(), e.index, true, e.box);
				const Event maxEvent(
					e.box->max().x(), e.index, false, e.box);

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

		Box clique(2);

		if (maxMaxCliqueSize > 1)
		{
			// We have now reconstructed a tree which 
			// contains a maximum clique rectangle.
			// We already know its y-range. Now we need
			// to find its x-range.

			Event_ConstIterator cliqueIter = tree.root();
			while(true)
			{
				ASSERT(!cliqueIter.sentinel());

				const integer v = cliqueIter->key().min ? 1 : -1;

				if (cliqueIter->value().maxCliqueSize == 
					cliqueIter.left()->value().actives + v)
				{
					// This is a maximum clique node.
					break;
				}
				else if (cliqueIter->value().maxCliqueSize == 
					cliqueIter.left()->value().maxCliqueSize)
				{
					cliqueIter = cliqueIter.left();
				}
				else
				{
					cliqueIter = cliqueIter.right();
				}
			}

			const Real xMin = cliqueIter->key().position;
			++cliqueIter;
			const Real xMax = cliqueIter->key().position;

			clique.set(xMin, yMin, xMax, yMax);

			// All of the boxes in the tree participate
			// in the intersection.
			Event_ConstIterator iter = tree.begin();
			while(iter != cliqueIter)
			{
				if (iter->key().min)
				{
					// We want to avoid reporting each box
					// twice, so we report it only on the
					// minimum event. This will also
					// report the boxes in sorted order
					// based on their minimum y-points.
					*result = iter->key().box;
					++result;
				}
				++iter;
			}
		}

		return clique;
	}	

	template <typename AlignedBox_ConstIterator>
	typename std::iterator_traits<AlignedBox_ConstIterator>::value_type 
		maximumClique(const ForwardRange<AlignedBox_ConstIterator>& boxSet)
	{
		return Pastel::maximumClique(boxSet, NullIterator());
	}

}

#endif
