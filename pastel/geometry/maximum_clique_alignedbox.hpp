#ifndef PASTEL_MAXIMUM_CLIQUE_ALIGNEDBOX_HPP
#define PASTEL_MAXIMUM_CLIQUE_ALIGNEDBOX_HPP

#include "pastel/geometry/maximum_clique_alignedbox.h"

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/counting_iterator.h"
#include "pastel/sys/null_iterator.h"
#include "pastel/sys/random_uniform.h"

#include <boost/type_traits/is_same.hpp>

#include <unordered_map>

namespace Pastel
{

	namespace MaximumCliqueAlignedBox_
	{

		class EventType
		{
		public:
			enum Enum
			{
				// These numbers were chosen to make the l < r
				// relation matrix have the following form:
				//
				//        r
				//   < ( [ ] ) 
				//   ( F F F F 
				// l [ T F F F
				//   ] T T F F 
				//   ) T T T F

				OpenMin = 3,
				ClosedMin = 2,
				ClosedMax = 1,
				OpenMax = 0
			};
		};

		inline bool compareType(
			EventType::Enum left,
			EventType::Enum right)
		{
			// A strict order relation < is irreflexive,
			// antisymmetric, and transitive.
			//
			// 1) Irreflexivity gives the diagonal.
			// ( < ( <=> F
			// [ < [ <=> F
			// ] < ] <=> F
			// ) < ) <=> F
			//
			// 2) Otherwise: 
			// [a, oo) < (a, oo)    (intersection)
			// [a, oo) < (-oo, a]   (intersection)
			// (-oo, a] < (a, oo)   (no intersection)
			// (-oo, a) < (a, oo)   (no intersection)
			// (-oo, a) < [a, oo)   (no intersection)
			// (-oo, a) < (-oo, a]  (intersection)
			//
			// 3) Summary:
			//
			//        r
			//   < ( [ ] )
			//   ( F      
			// l [ T F T  
			//   ] T   F  
			//   ) T T T F
			//
			// 5) Full relation matrix by antisymmetry:
			//
			//        r
			//   < ( [ ] ) 
			//   ( F F F F 
			// l [ T F T F
			//   ] T F F F 
			//   ) T T T F
			//
			// This relation is anti-symmetric, as can be
			// checked by the following brute-force Matlab
			// program:
			//
			// A = logical([...
			//     0, 0, 0, 0; ...
			//     1, 0, 1, 0; ...
			//     1, 0, 0, 0; ...
			//     1, 1, 1, 0]);
			//
			// for i = 1 : 4
			//     for j = 1 : 4
			//         for k = 1 : 4
			//             if A(i, j) && A(j, k) && ~A(i, k)
			//                 disp('Oh noes!');
			//             end
			//         end
			//     end
			// end

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
			
			// Otherwise return true if in the
			// lower triangle. Note this relies
			// on the correct ordering of the EventType
			// enum.
			return left < right;
		}

		template <typename Real, typename ConstIterator>
		class Event
		{
		public:
			Event()
				: position(0)
				, index(0)
				, type(EventType::ClosedMin)
				, box()
			{
			}

			Event(const Real& position_,
				integer index_,
				EventType::Enum type_,
				const ConstIterator& box_)
				: position(position_)
				, index(index_)
				, type(type_)
				, box(box_)
			{
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
				return 
					type == EventType::ClosedMin ||
					type == EventType::OpenMin;
			}

			Real position;
			integer index;
			EventType::Enum type;
			ConstIterator box;
		};

		class Data
		{
		public:
			Data(
				integer actives_ = 0,
				integer maxCliqueSize_ = -1)
				: actives(actives_)
				, maxCliqueSize(maxCliqueSize_)
			{
			}

			// The difference between the number of 
			// ending points and the number of starting
			// points in the subtree rooted at the current 
			// node.
			integer actives;
			integer maxCliqueSize;
		};

		template <
			typename Key,
			typename Compare,
			typename Data>
		class MaximumClique_Customization
			: public RedBlackTree_Concepts::Customization<Key, Compare, Data>
		{
		protected:
			typedef RedBlackTree_Fwd<Key, Compare, Data> Tree;
			typedef typename Tree::Iterator
				Iterator;
			typedef typename Tree::ConstIterator
				ConstIterator;

			void swap(MaximumClique_Customization& that) {}

			void updateHierarchical(const Iterator& iter)
			{
				// The size of the clique in the current node
				// is given simply by the number of active
				// intervals. However, this quantity can't
				// be computed hierarchically at each node,
				// so we have to be a little more indirect.

				// The 'actives' node variable traces
				// the difference between the number of ending
				// points and the number of starting points
				// in a subtree.
				// This is _not_ the same as the number of
				// active intervals, except at the root, since 
				// intervals can be starting outside the subtree 
				// rooted at the current node.

				// The point is to trace a path from the root
				// downwards, such that it gives the maximum 
				// amount of 'actives' difference. There can
				// be many such nodes, as the maximum clique
				// is not unique. Interestingly, we are able
				// to encode all of these paths by just using
				// the augmented 'actives' and 'maxCliqueSize'
				// node variables.

				const integer v = iter.key().min() ? 1 : -1;

				// The difference between the number of ending 
				// points and the number of starting points in
				// a subtree is easily computed recursively.
				iter.data().actives = 
					iter.left().data().actives + 
					iter.right().data().actives +
					v;

				// The difference in the current node
				// is simply given by the difference in the
				// left node plus the 'v'-value of the current 
				// node.
				const integer currentDifference =
					iter.left().data().actives + v;

				// The current difference might not mark
				// the largest difference, since there can be 
				// larger differences in the left and right 
				// subtrees.
								
				// The maximum difference in the left subtree,
				// given our expanded view from a parent node,
				// is unchanged.
				const integer leftDifference =
					iter.left().data().maxCliqueSize;

				// The maximum difference in the right subtree,
				// given our expanded view from a parent node, is 
				// the sum of the current difference and the 
				// maximum difference in the right subtree.

				// Note that maxCliqueSize is -1 in the
				// sentinel node, so it does not always hold
				// that rightDifference >= currentDifference.
				const integer rightDifference =
					currentDifference +
					iter.right().data().maxCliqueSize;

				// Finally, the maximum difference in the
				// current subtree is the maximum among the
				// three differences.
				iter.data().maxCliqueSize = 
					std::max(leftDifference,
					std::max(currentDifference,
					rightDifference));

				// Note that at the root of the tree,
				// the maxCliqueSize really is the size of the
				// maximum clique.
			}
		};

		template <typename Iterator>
		bool cliqueHere(const Iterator& iter)
		{
			const integer v = iter.key().min() ? 1 : -1;

			// There is a maximum clique in the current node, 
			// if the maxCliqueSize was computed as it is for
			// the current node.
			if (iter.data().maxCliqueSize == 
				iter.left().data().actives + v)
			{
				return true;
			}
		
			return false;
		}

		template <typename Iterator>
		bool cliqueOnLeft(const Iterator& iter)
		{
			// There is a maximum clique in the left subtree, 
			// if the maxCliqueSize was computed as it is for
			// the left subtree.
			if (!iter.left().sentinel() && 
				iter.data().maxCliqueSize == 
				iter.left().data().maxCliqueSize)
			{
				return true;
			}
		
			return false;
		}

		template <typename Iterator>
		bool cliqueOnRight(const Iterator& iter)
		{
			const integer v = iter.key().min() ? 1 : -1;

			// There is a maximum clique in the right subtree, 
			// if the maxCliqueSize was computed as it is for
			// the right subtree.
			if (!iter.right().sentinel() &&
				iter.data().maxCliqueSize == 
				iter.left().data().actives + v +
				iter.right().data().maxCliqueSize)
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
				ASSERT(!iter.sentinel());

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
			// The augmented red-black tree is traversed
			// from the root downwards to find _a_ maximum clique.
			// Maximum cliques are not unique; there can be
			// many of them. In this case we choose one randomly.
			// The tree consist of the end-points of the
			// projections of boxes on the x-axis. The returned
			// iterator is such that:
			// * It points to a starting point.
			// * The next point is an ending point (since otherwise
			// that point would have a larger clique).
			// * All active boxes before the ending point
			// are part of the maximum clique.

			Direction::Enum candidateSet[3];

			ASSERT(!root.sentinel());

			// Start from the root of the tree.
			Iterator iter = root;

			// While we are not in a leaf node...
			while(!iter.left().sentinel() ||
				!iter.right().sentinel()) 
			{
				// A maximum clique can be in the current node,
				// in the left subtree, or in the right subtree.
				// These are not exclusive; any combination is
				// possible.
				integer candidates = 0;
				if (cliqueHere(iter))
				{
					// There is a maximum clique in this node.
					candidateSet[candidates] = Direction::Current;
					++candidates;
				}
				if (cliqueOnLeft(iter))
				{
					// There is a maximum clique in the left subtree.
					candidateSet[candidates] = Direction::Left;
					++candidates;
				}
				if (cliqueOnRight(iter))
				{
					// There is a maximum clique in the right subtree.
					candidateSet[candidates] = Direction::Right;
					++candidates;
				}

				// Necessarily there is at least one maximum clique
				// in the tree.
				ASSERT_OP(candidates, >, 0);

				// If there is only one choice, pick that.
				integer index = 0;
				if (candidates == 2)
				{
					// If there are two choices, pick one
					// randomly.
					// Here logical-and is used for an efficient
					// mod 2.
					index = randomInteger() & 1;
				}
				else if (candidates == 3)
				{
					// If there are three choices, pick one
					// randomly. Here we must use the
					// remainder operator.
					index = randomInteger() % 3;
				}
				
				const Direction::Enum direction = candidateSet[index];
				if (direction == Direction::Current)
				{
					// There is a maximum clique in this node,
					// and it was chosen.
					break;
				}

				// Traverse into the chosen subtree.
				iter = (direction == Direction::Left) ? 
					iter.left() : iter.right();

				// Write the chosen subtree into the
				// direction set.
				*result = direction;
				++result;
			}
			
			// Return the result.
			return iter;
		}

	}

	template <
		typename AlignedBox_ConstIterator,
		typename AlignedBox_ConstIterator_Iterator>
		typename std::iterator_traits<AlignedBox_ConstIterator>::value_type 
		maximumClique(
		const ForwardIterator_Range<AlignedBox_ConstIterator>& boxSet,
		integer sweepDirection,
		AlignedBox_ConstIterator_Iterator result)
	{
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

		using namespace MaximumCliqueAlignedBox_;

		typedef typename std::iterator_traits<AlignedBox_ConstIterator>::
			value_type Box;
		typedef typename Box::Real_ Real;

		PASTEL_STATIC_ASSERT(Box::N_ == 2 || Box::N_ == Dynamic);

		typedef Event<Real, AlignedBox_ConstIterator> Event;
		typedef MaximumCliqueAlignedBox_::Data Data;
		typedef MaximumClique_Customization<Event, LessThan, Data> Customization;

		typedef RedBlackTree<Event, LessThan, Data, Customization> Tree;
		typedef typename Tree::ConstIterator Event_ConstIterator;

		ENSURE_OP(sweepDirection, >=, 0);
		ENSURE_OP(sweepDirection, <, 2);

		// This will take linear time for an iterator
		// that is not random access. However, it is
		// worth it, since we can then reserve the size
		// of 'eventSet' below beforehand.
		const integer n = boxSet.size();

		// We allow the sweeping direction to be chosen freely.
		// For convenience, we shall call the sweep direction the 
		// y-axis, and the other axis the x-axis. This allows us,
		// at the same time, to be generic and have a specific 
		// geometric situtation in mind.
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

				// First the minimum y-point.

				const EventType::Enum minType = 
					iter->minTopology()[y] == Topology::Closed ?
					EventType::ClosedMin : EventType::OpenMin;

				eventSet.push_back(
					Event(iter->min()[y], i, minType, iter));

				// Then the maximum y-point.

				const EventType::Enum maxType = 
					iter->maxTopology()[y] == Topology::Closed ?
					EventType::ClosedMax : EventType::OpenMax;

				eventSet.push_back(
					Event(iter->max()[y], i, maxType, iter));

				++i;
				++iter;
			}
		}

		// Sort the event list lexicographically.
		// Have a look at the Event::operator<() for the
		// order.
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
			// maximum clique. This is called the 
			// _search phase_.

			// The second time in this loop (j == 1), our 
			// aim is to reconstruct the tree where the
			// maximum clique was found. This is called
			// the _reconstruction phase_.

			tree.clear();

			// For each event...
			for (integer i = 0;i < n;++i)
			{
				// The events come out in increasing y-order. 
				// An event either begins a new box or ends 
				// an existing box.
				Event& e = eventSet[i];

				// Find out the extremities of the box
				// in the _x_ direction.

				// First the minimum x-point.
				const EventType::Enum minType = 
					e.box->minTopology()[x] == Topology::Closed ?
					EventType::ClosedMin : EventType::OpenMin;

				const Event minEvent(
					e.box->min()[x], e.index, minType, e.box);

				// Then the maximum x-point.
				const EventType::Enum maxType = 
					e.box->maxTopology()[x] == Topology::Closed ?
					EventType::ClosedMax : EventType::OpenMax;

				const Event maxEvent(
					e.box->max()[x], e.index, maxType, e.box);

				if (e.min())
				{
					// This event begins a new box.
					
					// Insert the x-endpoints of that box into the tree.
					tree.insert(minEvent);
					tree.insert(maxEvent);
					
					if (j == 0)
					{
						// This is the search phase.

						// Primarily, we want to maximize the size of
						// the maximum clique.
						const integer maxCliqueSize = 
							tree.root().data().maxCliqueSize;

						if (maxCliqueSize > 1 && maxCliqueSize >= maxMaxCliqueSize)
						{
							// Secondarily, we want to maximize the area
							// of the maximum clique box.
							std::vector<Direction::Enum> directionSet;
							Event_ConstIterator cliqueIter = 
								findSomeMaximumClique(tree.root(),
								std::back_inserter(directionSet));

							const Real xMinNew = cliqueIter.key().position;
							++cliqueIter;
							const Real xMaxNew = cliqueIter.key().position;

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
					else 
					{
						// This is the reconstruction phase.
						if (i == maxIndex)
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
				}
				else
				{
					// This event ends an existing box.
					
					// Remove the box from the tree.
					tree.erase(minEvent);
					tree.erase(maxEvent);
				}
			}

			if (maxMaxCliqueSize == 1)
			{
				// All boxes are separated.
				break;
			}
		}

		Box clique(2);
		if (maxMaxCliqueSize < 2)
		{
			// All boxes are separated.
			return clique;
		}

		// We have now reconstructed a tree which 
		// contains a maximum clique rectangle.
		// We already know its y-range. Now we need
		// to find its x-range.
		Event_ConstIterator cliqueIter = 
			findMaximumClique(
			tree.root(),
			range(maxDirectionSet.begin(), 
			maxDirectionSet.end()));

		const Real xMin = cliqueIter.key().position;
		++cliqueIter;
		const Real xMax = cliqueIter.key().position;

		clique.min()[x] = xMin;
		clique.min()[y] = yMin;
		clique.max()[x] = xMax;
		clique.max()[y] = yMax;

		const bool reportBoxes = 
			!std::is_same<AlignedBox_ConstIterator_Iterator, NullIterator>::value;
		if (!reportBoxes)
		{
			return clique;
		}

		// Do a linear scan to find out which boxes
		// are active in our chosen maximum clique.

		typedef std::unordered_map<integer, AlignedBox_ConstIterator>
			ActiveSet;
		typedef typename ActiveSet::const_iterator Active_ConstIterator;

		std::unordered_map<integer, AlignedBox_ConstIterator> activeSet;
		
		// Find out all the participating boxes.
		{
			Event_ConstIterator iter = tree.begin();
			// Note we already advanced cliqueIter by one,
			// so this loop really includes all boxes.
			while(iter != cliqueIter)
			{
				if (iter.key().min())
				{
					activeSet.insert(
						std::make_pair(iter.key().index, iter.key().box));
				}
				else
				{
					activeSet.erase(iter.key().index);
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

		return clique;
	}	

	template <typename AlignedBox_ConstIterator>
	typename std::iterator_traits<AlignedBox_ConstIterator>::value_type 
		maximumClique(
		const ForwardIterator_Range<AlignedBox_ConstIterator>& boxSet,
		integer sweepDirection)
	{
		return Pastel::maximumClique(
			boxSet, sweepDirection, NullIterator());
	}

}

#endif
