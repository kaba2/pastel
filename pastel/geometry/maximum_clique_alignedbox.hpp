#ifndef PASTEL_MAXIMUM_CLIQUE_ALIGNEDBOX_HPP
#define PASTEL_MAXIMUM_CLIQUE_ALIGNEDBOX_HPP

#include "pastel/geometry/maximum_clique_alignedbox.h"

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/countingiterator.h"
#include "pastel/sys/nulliterator.h"
#include "pastel/sys/unorderedmap.h"
#include "pastel/sys/random_uniform.h"

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
					integer maxCliqueSize_ = -1)
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
					std::max(
					iter.left()->value().maxCliqueSize,
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
			if (!iter.left().sentinel() && 
				iter->value().maxCliqueSize == 
				iter.left()->value().maxCliqueSize)
			{
				return true;
			}
		
			return false;
		}

		template <typename Iterator>
		bool cliqueOnRight(const Iterator& iter)
		{
			const integer v = iter->key().min() ? 1 : -1;

			if (!iter.right().sentinel() &&
				iter->value().maxCliqueSize == 
				iter.left()->value().actives + v +
				iter.right()->value().maxCliqueSize)
			{
				return true;
			}
		
			return false;
		}

		class Direction
		{
		public:
			enum Enum
			{
				Current,
				Left,
				Right
			};
		};

		template <typename Iterator, typename Direction_Iterator>
		Iterator findMaximumClique(
			const Iterator& root,
			const ForwardIterator_Range<Direction_Iterator>& directionSet)
		{
			Iterator iter = root;
			Direction_Iterator directionIter = directionSet.begin();
			const Direction_Iterator directionEnd = directionSet.end();
			while(directionIter != directionEnd)
			{
				const Direction::Enum direction =
					*directionIter;

				ASSERT(
					direction == Direction::Left ||
					direction == Direction::Right);

				if (direction == Direction::Left)
				{
					iter = iter.left();
				}
				else
				{
					iter = iter.right();
				}
				++directionIter;
			}
			
			return iter;
		}

		template <typename Iterator, typename Direction_Iterator>
		Iterator findSomeMaximumClique(
			const Iterator& root,
			Direction_Iterator result)
		{
			Direction::Enum candidateSet[3];

			ASSERT(!root.sentinel());

			Iterator iter = root;
			while(!iter.left().sentinel() ||
				!iter.right().sentinel()) 
			{
				// There can be many maximum cliques in the tree.
				// We choose one randomly.

				integer candidates = 0;
				if (cliqueHere(iter))
				{
					candidateSet[candidates] = Direction::Current;
					++candidates;
				}
				if (cliqueOnLeft(iter))
				{
					candidateSet[candidates] = Direction::Left;
					++candidates;
				}
				if (cliqueOnRight(iter))
				{
					candidateSet[candidates] = Direction::Right;
					++candidates;
				}

				ASSERT_OP(candidates, >, 0);

				integer index = 0;
				if (candidates == 2)
				{
					index = randomInteger() & 1;
				}
				else if (candidates == 3)
				{
					index = randomInteger() % 3;
				}

				const Direction::Enum direction = candidateSet[index];

				if (direction == Direction::Current)
				{
					break;
				}

				iter = (direction == Direction::Left) ? 
					iter.left() : iter.right();

				*result = direction;
				++result;
			}

			return iter;
		}

	}

	template <
		typename AlignedBox_ConstIterator,
		typename AlignedBox_ConstIterator_Iterator>
		typename std::iterator_traits<AlignedBox_ConstIterator>::value_type 
		maximumClique(
		const ForwardIterator_Range<AlignedBox_ConstIterator>& boxSet,
		MaximumClique_BoxType::Enum boxType,
		integer sweepDirection,
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

		ENSURE_OP(sweepDirection, >=, 0);
		ENSURE_OP(sweepDirection, <, 2);

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

		const integer y = sweepDirection;
		const integer x = !y;

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
					Event(iter->min()[y], i, i, minType, iter));
				eventSet.push_back(
					Event(iter->max()[y], i, i, maxType, iter));

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
		Real maxSize = 0;
		Real maxArea = 0;
		std::vector<Direction::Enum> maxDirectionSet;

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
					e.box->min()[x], e.index, e.label, minType, e.box);
				const Event maxEvent(
					e.box->max()[x], e.index, e.label, maxType, e.box);

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

						if (maxCliqueSize > 1 && maxCliqueSize >= maxMaxCliqueSize)
						{
							// Secondarily, we want to maximize the area
							// of the maximum clique box.
							std::vector<Direction::Enum> directionSet;
							Event_ConstIterator cliqueIter = 
								findSomeMaximumClique(tree.root(),
								std::back_inserter(directionSet));

							const Real xMinNew = cliqueIter->key().position;
							++cliqueIter;
							const Real xMaxNew = cliqueIter->key().position;

							const Real yMinNew = e.position;
							const Real yMaxNew = eventSet[i + 1].position;

							const Real width = xMaxNew - xMinNew;
							const Real height = yMaxNew - yMinNew;

							const Real area = width * height;

							// Tertiarily, we want the maximum width of the 
							// maximum clique box.
							const Real size = std::max(width, height);

							if (maxCliqueSize > maxMaxCliqueSize ||
								(maxCliqueSize == maxMaxCliqueSize && area > maxArea) ||
								(maxCliqueSize == maxMaxCliqueSize && area == maxArea &&
								size > maxSize))
							{
								maxMaxCliqueSize = maxCliqueSize;
								maxIndex = i;
								maxArea = area;
								maxSize = size;
								maxDirectionSet.swap(directionSet);
							}
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

			Event_ConstIterator cliqueIter = 
				findMaximumClique(
				tree.root(),
				range(maxDirectionSet.begin(), 
				maxDirectionSet.end()));

			const Real xMin = cliqueIter->key().position;
			++cliqueIter;
			const Real xMax = cliqueIter->key().position;

			clique.min()[x] = xMin;
			clique.min()[y] = yMin;
			clique.max()[x] = xMax;
			clique.max()[y] = yMax;

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
		const ForwardIterator_Range<AlignedBox_ConstIterator>& boxSet,
		MaximumClique_BoxType::Enum boxType,
		integer sweepDirection)
	{
		return Pastel::maximumClique(
			boxSet, boxType, sweepDirection, NullIterator());
	}

}

#endif
