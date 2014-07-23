#ifndef PASTELGEOMETRY_MAXIMUM_CLIQUE_ALIGNEDBOX_HPP
#define PASTELGEOMETRY_MAXIMUM_CLIQUE_ALIGNEDBOX_HPP

#include "pastel/geometry/maximum_clique_alignedbox.h"

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/counting_iterator.h"
#include "pastel/sys/null_output.h"
#include "pastel/sys/push_back_output.h"
#include "pastel/sys/random_uniform.h"

#include <boost/type_traits/is_same.hpp>

#include <unordered_map>

namespace Pastel
{

	namespace MaximumCliqueAlignedBox_
	{

		enum class EventType : integer
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

		inline bool compareType(
			EventType left,
			EventType right)
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
				EventType type_,
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
			EventType type;
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

		template <typename Settings>
		class MaximumClique_Customization
			: public Empty_RedBlackTree_Customization<Settings>
		{
		protected:
			using Fwd = RedBlackTree_Fwd<Settings>;

			PASTEL_FWD(Iterator);
			PASTEL_FWD(ConstIterator);
			PASTEL_FWD(Propagation);

			void swap(MaximumClique_Customization& that) {}

			void updatePropagation(
				const Iterator& iter,
				Propagation& propagation)
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

				integer v = iter.key().min() ? 1 : -1;

				// The difference between the number of ending 
				// points and the number of starting points in
				// a subtree is easily computed recursively.
				propagation.actives = 
					iter.left().propagation().actives +
					iter.right().propagation().actives +
					v;

				// The difference in the current node
				// is simply given by the difference in the
				// left node plus the 'v'-value of the current 
				// node.
				integer currentDifference =
					iter.left().propagation().actives + v;

				// The current difference might not mark
				// the largest difference, since there can be 
				// larger differences in the left and right 
				// subtrees.
								
				// The maximum difference in the left subtree,
				// given our expanded view from a parent node,
				// is unchanged.
				const integer leftDifference =
					iter.left().propagation().maxCliqueSize;

				// The maximum difference in the right subtree,
				// given our expanded view from a parent node, is 
				// the sum of the current difference and the 
				// maximum difference in the right subtree.

				// Note that maxCliqueSize is -1 in the
				// sentinel node, so it does not always hold
				// that rightDifference >= currentDifference.
				const integer rightDifference =
					currentDifference +
					iter.right().propagation().maxCliqueSize;

				// Finally, the maximum difference in the
				// current subtree is the maximum among the
				// three differences.
				propagation.maxCliqueSize =
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
			integer v = iter.key().min() ? 1 : -1;

			// There is a maximum clique in the current node, 
			// if the maxCliqueSize was computed as it is for
			// the current node.
			if (iter.propagation().maxCliqueSize == 
				iter.left().propagation().actives + v)
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
			if (!iter.left().isSentinel() && 
				iter.propagation().maxCliqueSize ==
				iter.left().propagation().maxCliqueSize)
			{
				return true;
			}
		
			return false;
		}

		template <typename Iterator>
		bool cliqueOnRight(const Iterator& iter)
		{
			integer v = iter.key().min() ? 1 : -1;

			// There is a maximum clique in the right subtree, 
			// if the maxCliqueSize was computed as it is for
			// the right subtree.

			if (!iter.right().isSentinel() &&
				iter.propagation().maxCliqueSize ==
				iter.left().propagation().actives + v +
				iter.right().propagation().maxCliqueSize)
			{
				return true;
			}
		
			return false;
		}

		enum class Direction : integer
		{
			Current,
			Left,
			Right
		};

		template <typename Iterator, typename Direction_Range>
		Iterator findMaximumClique(
			const Iterator& root,
			const Direction_Range& directionSet)
		{
			typedef typename boost::range_iterator<Direction_Range>::type
				Direction_Iterator;

			Iterator iter = root;
			Direction_Iterator directionIter = directionSet.begin();
			Direction_Iterator directionEnd = directionSet.end();
			while(directionIter != directionEnd)
			{
				ASSERT(!iter.isSentinel());

				Direction direction =

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

		template <typename Iterator, typename Direction_Output>
		Iterator findSomeMaximumClique(
			const Iterator& root,
			const Direction_Output& report)
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

			Direction candidateSet[3];

			ASSERT(!root.isSentinel());

			// Start from the root of the tree.
			Iterator iter = root;

			// While we are not in a leaf node...
			while(!iter.left().isSentinel() ||
				!iter.right().isSentinel()) 
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
				if (candidates >= 2)
				{
					// If there is more one choice, pick one randomly.
					index = randomInteger(candidates);
				}
				
				Direction direction = candidateSet[index];
				if (direction == Direction::Current)
				{
					// There is a maximum clique in this node,
					// and it was chosen.
					break;
				}

				// Traverse into the chosen subtree.
				iter = (direction == Direction::Left) ? 
					iter.left() : iter.right();

				// Report the chosen subtree into the
				// direction set.
				report(direction);
			}
			
			// Return the result.
			return iter;
		}

	}

	template <
		typename AlignedBox_ConstRange,
		typename AlignedBox_Output>
		typename boost::range_value<AlignedBox_ConstRange>::type 
		maximumClique(

		const AlignedBox_ConstRange& boxSet,
		integer sweepDirection,
		const AlignedBox_Output& report)
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

		typedef typename boost::range_iterator<AlignedBox_ConstRange>::type
			AlignedBox_ConstIterator;
		using Box = typename boost::range_value<AlignedBox_ConstRange>::type;
		using Real = typename Box::Real_;

		PASTEL_STATIC_ASSERT(Box::N_ == 2 || Box::N_ == Dynamic);

		using Event = Event<Real, AlignedBox_ConstIterator>;
		using Data = MaximumCliqueAlignedBox_::Data;
		using Tree = RedBlackTree_Set<Event, void, LessThan, Data, void, false, MaximumClique_Customization>;
		using Event_ConstIterator = typename Tree::ConstIterator;

		ENSURE_OP(sweepDirection, >=, 0);
		ENSURE_OP(sweepDirection, <, 2);

		// This will take linear time for an iterator
		// that is not random access. However, it is
		// worth it, since we can then reserve the size
		// of 'eventSet' below beforehand.
		integer n = boxSet.size();

		// We allow the sweeping direction to be chosen freely.
		// For convenience, we shall call the sweep direction the 
		// y-axis, and the other axis the x-axis. This allows us,
		// at the same time, to be generic and have a specific 
		// geometric situtation in mind.
		integer y = sweepDirection;
		const integer x = !y;

		// Insert the y-endpoints of each box
		// into an event list.
		std::vector<Event> eventSet;

		eventSet.reserve(2 * n);
		{
			AlignedBox_ConstIterator iter = boxSet.begin();
			AlignedBox_ConstIterator iterEnd = boxSet.end();

			integer i = 0;
			while(iter != iterEnd)
			{
				PENSURE(iter->n() == 2);

				// First the minimum y-point.

				EventType minType = 
					iter->minTopology()[y] == Topology::Closed ?
					EventType::ClosedMin : EventType::OpenMin;

				eventSet.push_back(
					Event(iter->min()[y], i, minType, iter));

				// Then the maximum y-point.

				const EventType maxType = 
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
		std::vector<Direction> maxDirectionSet;

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
				EventType minType = 
					e.box->minTopology()[x] == Topology::Closed ?
					EventType::ClosedMin : EventType::OpenMin;

				Event minEvent(
					e.box->min()[x], e.index, minType, e.box);

				// Then the maximum x-point.
				const EventType maxType = 
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
							tree.root().propagation().maxCliqueSize;


						if (maxCliqueSize > 1 && maxCliqueSize >= maxMaxCliqueSize)
						{
							// Secondarily, we want to maximize the area
							// of the maximum clique box.
							std::vector<Direction> directionSet;
							Event_ConstIterator cliqueIter = 
								findSomeMaximumClique(tree.root(),
								pushBackOutput(directionSet));

							Real xMinNew = cliqueIter.key().position;
							++cliqueIter;
							Real xMaxNew = cliqueIter.key().position;

							const Real yMinNew = e.position;
							const Real yMaxNew = eventSet[i + 1].position;

							const Real width = xMaxNew - xMinNew;
							const Real height = yMaxNew - yMinNew;


							const Real area = width * height;

							// Tertiarily, we want the maximum width of the 
							// maximum clique box.
							Real size = std::max(width, height);

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

		Real xMin = cliqueIter.key().position;
		++cliqueIter;
		Real xMax = cliqueIter.key().position;

		clique.min()[x] = xMin;
		clique.min()[y] = yMin;
		clique.max()[x] = xMax;
		clique.max()[y] = yMax;

		const bool reportBoxes = 
			!std::is_same<AlignedBox_Output, Null_Output>::value;
		if (!reportBoxes)
		{
			return clique;
		}

		// Do a linear scan to find out which boxes
		// are active in our chosen maximum clique.

		typedef std::unordered_map<integer, AlignedBox_ConstIterator>
			ActiveSet;
		using Active_ConstIterator = typename ActiveSet::const_iterator;

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
				report(iter->second);
				++iter;
			}
		}

		return clique;
	}	

	template <typename AlignedBox_ConstRange>
	typename boost::range_value<AlignedBox_ConstRange>::type 
		maximumClique(

		const AlignedBox_ConstRange& boxSet,
		integer sweepDirection)
	{
		return Pastel::maximumClique(
			boxSet, sweepDirection, Null_Output());
	}

}

#endif
