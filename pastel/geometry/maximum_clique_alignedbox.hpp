#ifndef PASTEL_MAXIMUM_CLIQUE_ALIGNEDBOX_HPP
#define PASTEL_MAXIMUM_CLIQUE_ALIGNEDBOX_HPP

#include "pastel/geometry/maximum_clique_alignedbox.h"

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/countingiterator.h"
#include "pastel/sys/nulliterator.h"
#include "pastel/sys/unorderedmap.h"

#include <boost/type_traits/is_same.hpp>

namespace Pastel
{

	namespace MaximumCliqueAlignedBox_Detail
	{

		class EventType
		{
		public:
			enum Enum
			{
				// These numbers were carefully
				// chosen to make the <-relation
				// matrix of this enum to have a 
				// certain form.
				OpenMin = 0,
				ClosedMin = 1,
				OpenMax = 2,
				ClosedMax = 3
			};
		};

		template <typename Real, typename ConstIterator>
		class Event
		{
		public:
			Event()
				: position(0)
				, index(0)
				, label(0)
				, type(EventType::ClosedMin)
				, box()
			{
			}

			Event(const Real& position_,
				integer index_,
				integer label_,
				EventType::Enum type_,
				const ConstIterator& box_)
				: position(position_)
				, index(index_)
				, label(label_)
				, type(type_)
				, box(box_)
			{
			}

			bool compareType(
				EventType::Enum left,
				EventType::Enum right) const
			{
				// Less-than is an irreflexive, asymmetric, and
				// transitive relation. Thus logically we need only 
				// explain the following cases:
				//
				//        r
				//     ( [ ) ]
				//   ( F      
				// l [ T F    
				//   ) T T F  
				//   ] T F T F
				//
				// This relation matrix is transitive,
				// which I checked with Matlab.
				//
				// 1) Irreflexivity explains the diagonal.
				//
				// 2) If there is OpenMin and ClosedMin,
				// then we can choose either way. We use
				// the opportunity to make the relation
				// matrix simpler by placing true values
				// to lower-right.
				//
				// 3) If both are closed, but not equal, 
				// the minimum one should come first to
				// cause an intersection.
				//
				// 4) If both are open, but not equal,
				// the maximum one should come first to
				// not cause an intersection.
				//
				// 5) If there is OpenMin and ClosedMax,
				// or ClosedMin and OpenMax, the closing
				// one should come first to not cause an
				// intersection.
				//
				// Full relation matrix:
				//
				//        r
				//     ( [ ) ]
				//   ( F F F F
				// l [ T F F T   
				//   ) T T F F 
				//   ] T F T F

				if (left == EventType::ClosedMax &&
					right == EventType::ClosedMin)
				{
					return false;
				}
				if (left == EventType::ClosedMin &&
					right == EventType::ClosedMax)
				{
					return true;
				}
				
				return right < left;
			}

			bool operator<(
				const Event& that) const
			{
				if (position != that.position)
				{
					return position < that.position;
				}
				
				if (type != that.type)
				{
					if (label == that.label)
					{
						// If the labels are equal,
						// then we treat them specially
						// so that two closed event-points
						// won't cause an intersection.
						return that.type < type;
					}

					return compareType(type, that.type);
				}

				// We would rather want to compare
				// the iterators, so that we didn't need
				// to store the 'index' field. However, only
				// random-access iterators are guaranteed
				// to have a less-than comparison.
				return index < that.index;
			}

			bool min() const
			{
				return type <= 1;
			}

			Real position;
			integer index;
			integer label;
			EventType::Enum type;
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
				const integer v = iter->key().min() ? 1 : -1;

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

		template <typename Iterator>
		bool cliqueHere(const Iterator& iter)
		{
			const integer v = iter->key().min() ? 1 : -1;

			if (iter->value().maxCliqueSize == 
				iter.left()->value().actives + v)
			{
				return true;
			}
		
			return false;
		}

		template <typename Iterator>
		bool cliqueOnLeft(const Iterator& iter)
		{
			if (iter->value().maxCliqueSize == 
				iter.left()->value().maxCliqueSize)
			{
				return true;
			}
		
			return false;
		}

	}

	template <
		typename AlignedBox_ConstIterator,
		typename AlignedBox_ConstIterator_Iterator>
		typename std::iterator_traits<AlignedBox_ConstIterator>::value_type 
		maximumClique(const ForwardRange<AlignedBox_ConstIterator>& boxSet,
		MaximumClique_BoxType::Enum boxType,
		AlignedBox_ConstIterator_Iterator result)
	{
		using namespace MaximumCliqueAlignedBox_Detail;

		typedef typename std::iterator_traits<AlignedBox_ConstIterator>::
			value_type Box;
		typedef typename Box::Real_ Real;

		PASTEL_STATIC_ASSERT(Box::N_ == 2 || Box::N_ == Dynamic);

		typedef Event<Real, AlignedBox_ConstIterator> Event;
		typedef MaximumClique_RbtPolicy RbtPolicy;
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

		const EventType::Enum minType = 
			boxType == MaximumClique_BoxType::Closed ?
			EventType::ClosedMin : EventType::OpenMin;

		const EventType::Enum maxType = 
			boxType == MaximumClique_BoxType::Closed ?
			EventType::ClosedMax : EventType::OpenMax;

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
					Event(iter->min().y(), i, i, minType, iter));
				eventSet.push_back(
					Event(iter->max().y(), i, i, maxType, iter));

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
		Real maxHeight = 0;

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
					e.box->min().x(), e.index, e.label, minType, e.box);
				const Event maxEvent(
					e.box->max().x(), e.index, e.label, maxType, e.box);

				if (e.min())
				{
					// If this event begins a new box, then
					// we insert the x-endpoints of that box 
					// into the tree.
					tree.insert(minEvent);
					tree.insert(maxEvent);
					
					if (j == 0)
					{
						// Primarily, we want to maximize the size of
						// the maximum clique.
						const integer maxCliqueSize = 
							tree.root()->value().maxCliqueSize;
						
						// Secondarily, we want to maximize the height
						// of the maximum clique box.
						const Real height = 
							eventSet[i + 1].position - e.position;

						if (maxCliqueSize > maxMaxCliqueSize ||
							(maxCliqueSize == maxMaxCliqueSize && height > maxHeight))
						{
							maxMaxCliqueSize = maxCliqueSize;
							maxIndex = i;
							maxHeight = height;
						}
					}
					else if (i == maxIndex)
					{
						// We have now reconstructed a tree which 
						// contains a maximum clique rectangle.
						// Although there could be several maximum 
						// cliques, we are content with just one (this 
						// also saves some computation).
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

				const integer v = cliqueIter->key().min() ? 1 : -1;

				// There can be many maximum cliques in the tree.
				// However, we need just one of these, and we
				// prefer the nodes in order current, left, and
				// right.

				if (cliqueHere(cliqueIter))
				{
					// This is a maximum clique node.
					break;
				}

				cliqueIter = cliqueOnLeft(cliqueIter) ? 
					cliqueIter.left() : cliqueIter.right();
			}

			const Real xMin = cliqueIter->key().position;
			++cliqueIter;
			const Real xMax = cliqueIter->key().position;

			clique.set(xMin, yMin, xMax, yMax);

			const bool reportBoxes = 
				!boost::is_same<AlignedBox_ConstIterator_Iterator, NullIterator>::value;

			if (reportBoxes)
			{
				// Do a linear scan to find out which boxes
				// are active in our chosen maximum clique.

				typedef UnorderedMap<integer, AlignedBox_ConstIterator>
					ActiveSet;
				typedef typename ActiveSet::const_iterator Active_ConstIterator;

				UnorderedMap<integer, AlignedBox_ConstIterator> activeSet;
				
				// Find out all the participating boxes.
				{
					Event_ConstIterator iter = tree.begin();
					while(iter != cliqueIter)
					{
						if (iter->key().min())
						{
							activeSet.insert(
								std::make_pair(iter->key().index, iter->key().box));
						}
						else
						{
							activeSet.erase(iter->key().index);
						}
						++iter;
					}
				}

				// Report all the participating boxes.
				{
					Active_ConstIterator iter = activeSet.begin();
					while(iter != activeSet.end())
					{
						*result = iter->second;
						++result;
						++iter;
					}
				}
			}
		}

		return clique;
	}	

	template <typename AlignedBox_ConstIterator>
	typename std::iterator_traits<AlignedBox_ConstIterator>::value_type 
		maximumClique(
		const ForwardRange<AlignedBox_ConstIterator>& boxSet,
		MaximumClique_BoxType::Enum boxType)
	{
		return Pastel::maximumClique(boxSet, boxType, NullIterator());
	}

}

#endif
