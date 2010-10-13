#ifndef PASTEL_MAXIMUM_CLIQUE_ALIGNEDBOX_HPP
#define PASTEL_MAXIMUM_CLIQUE_ALIGNEDBOX_HPP

#include "pastel/geometry/maximum_clique_alignedbox.h"

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/countingiterator.h"

namespace Pastel
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
				integer maxActives_ = 0)
				: actives(actives_)
				, maxActives(maxActives_)
			{
			}

			integer actives;
			integer maxActives;
		};

		template <typename Iterator>
		void updateInsertDown(const Iterator& iter)
		{
		}

		template <typename Iterator>
		void updateHierarchical(const Iterator& iter)
		{
			integer actives = 
				iter.left()->value().actives + 
				iter.right()->value().actives;

			if (iter->key().min)
			{
				++actives;
			}
			else
			{
				--actives;
			}

			iter->value().actives = actives;

			iter->value().maxActives = 
				std::max(actives,
				std::max(
				iter.left()->value().actives,
				iter.right()->value().actives));
		}

		void swap(MaximumClique_RbtPolicy& that)
		{
		}
	};

	template <typename Real>
	AlignedBox<Real, 2> maximumClique(
		const std::vector<AlignedBox<Real, 2> >& boxSet)
	{
		typedef Event<Real> Event;

		typedef RedBlackTree<
			Event, 
			std::less<Event>, 
			MaximumClique_RbtPolicy> Tree;
		typedef typename Tree::ConstIterator ConstIterator;
		typedef MaximumClique_RbtPolicy::ValueType Value;
		typedef std::vector<AlignedBox<Real, 2> > BoxSet;

		const integer n = boxSet.size();

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

		std::sort(eventSet.begin(), eventSet.end());

		Tree tree;
		AlignedBox<Real, 2> clique;

		integer maxMaxActives = 0;
		integer maxIndex = 0;
		for (integer j = 0;j < 2;++j)
		{
			tree.clear();

			for (integer i = 0;i < n;++i)
			{
				Event& e = eventSet[i];

				const Event minEvent(
					boxSet[e.index].min().x(), e.index, true);

				const Event maxEvent(
					boxSet[e.index].max().x(), e.index, false);

				if (e.min)
				{
					// Insert a box into the tree.
					tree.insert(minEvent, Value(1, 1));
					tree.insert(maxEvent, Value(-1, -1));
					
					if (j == 0)
					{
						// See if the number of maximum intersections grew.
						if (tree.root()->value().maxActives > maxMaxActives)
						{
							maxMaxActives = tree.root()->value().maxActives;
							maxIndex = i;
						}
					}
					else if (i == maxIndex)
					{
						// We have now reconstructed a tree which 
						// contains a maximum clique rectangle.
						// We already know its y-range. Now we need
						// to find its x-range.

						const Real yMin = e.position;
						const Real yMax = eventSet[i + 1].position;

						ConstIterator iter = tree.root();
						while(true)
						{
							ASSERT(!iter.sentinel());

							if (iter.left()->value().maxActives == maxMaxActives)
							{
								iter = iter.left();
							}
							else if (iter.right()->value().maxActives == maxMaxActives)
							{
								iter = iter.right();
							}
							else
							{
								// This is a maximum clique node.
								break;
							}
						}

						const Real xMin = iter->key().position;
						++iter;
						const Real xMax = iter->key().position;

						clique.set(xMin, yMin, xMax, yMax);
						break;
					}
				}
				else
				{
					// Remove a box from the tree.
					tree.erase(minEvent);
					tree.erase(maxEvent);
				}
			}
		}

		return clique;
	}	

}

#endif
