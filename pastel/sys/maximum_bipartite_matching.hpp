#ifndef PASTELSYS_MAXIMUM_BIPARTITE_MATCHING_HPP
#define PASTELSYS_MAXIMUM_BIPARTITE_MATCHING_HPP

#include "pastel/sys/maximum_bipartite_matching.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/keyvalue.h"
#include "pastel/sys/tuple_as_pair.h"

#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_map>

namespace Pastel
{

	namespace MaximumBipartiteMatching
	{

		template <
			typename Vertex_Pair_ForwardRange, 
			typename Vertex_Pair_Reporter,
			typename As_Pair,
			typename LeftHash,
			typename RightHash>
		class Algorithm
		{
		public:
			typedef typename Vertex_Pair_ForwardRange::value_type Type;
			typedef typename As_Pair::template ConceptMap<Type> Type_As_Pair;
			typedef typename Type_As_Pair::Left Left;
			typedef typename Type_As_Pair::Right Right;

			typedef std::unordered_map<Left, integer, LeftHash> LeftMap;
			typedef std::unordered_map<Right, integer, RightHash> RightMap;
			typedef std::unordered_map<integer, Left> InverseLeftMap;
			typedef std::unordered_map<integer, Right> InverseRightMap;

			void work(
				Vertex_Pair_ForwardRange range,
				const Vertex_Pair_Reporter& reporter,
				const As_Pair& asPair,
				const LeftHash& leftHash,
				const RightHash& rightHash)
			{
				// This is the Hopcroft-Karp algorithm for 
				// maximum bipartite matching.

				if (range.empty())
				{
					return;
				}

				initialize(range, asPair);

				leftHash_ = leftHash;
				rightHash_ = rightHash;

				while(findAugmentingPaths())
				{
					// As long as there are augmenting paths...
					const integer leftVertices = leftSet_.size();
					for (integer i = 0;i < leftVertices;++i)
					{
						const integer leftIndex = leftSet_[i];
						Vertex& left = vertexSet_[leftIndex];
						if (!left.paired())
						{
							// ... flip the augmenting paths to 
							// give a larger matching.
							flipPossibleAugmentingPath(leftIndex);
						}
					}
				}

				// Report the matching edges.
				const integer leftVertices = leftSet_.size();
				for (integer i = 0;i < leftVertices;++i)
				{
					const integer leftIndex = leftSet_[i];
					Vertex& left = vertexSet_[leftIndex];
					if (left.paired())
					{
						const integer rightIndex = left.pairIndex();

						reporter(
							std::make_pair(
							inverseLeftMap_[leftIndex],
							inverseRightMap_[rightIndex]));
					}
				}
			}

		private:
			bool findAugmentingPaths()
			{
				std::queue<integer> eventSet;

				vertexSet_[Sentinel].unvisit();

				const integer leftVertices = leftSet_.size();
				for (integer i = 0;i < leftVertices;++i)
				{
					const integer leftIndex = leftSet_[i];

					Vertex& vertex = 
						vertexSet_[leftIndex];
					if (!vertex.paired())
					{
						vertex.visit(0);
						eventSet.push(leftIndex);
					}
					else
					{
						vertex.unvisit();
					}
				}

				// The information whether the right vertices
				// are visited or not is not used.

				while(!eventSet.empty())
				{
					// Pick up the next vertex from the queue.
					// This gives a breadth-first ordering for
					// visiting the graph.
					const integer leftIndex = eventSet.front();
					ASSERT_OP(leftIndex, !=, Sentinel);

					Vertex& left = 
						vertexSet_[leftIndex];
					eventSet.pop();

					const integer edges = left.edges();
					for (integer i = 0;i < edges;++i)
					{
						Vertex& right =
							adjacent(left, i);

						// The 'nextLeft' is either a sentinel
						// node or a left vertex. If it is a 
						// sentinel (i.e. 'right' is not paired),
						// then it means we have found at least 
						// one augmenting path.
						Vertex& nextLeft = pair(right);

						if (!nextLeft.visited())
						{
							// If 'nextLeft' is not visited,
							// visit it now.
							nextLeft.visit(left.level() + 1);

							// And continue the breadth-first
							// traversal...
							const integer nextLeftIndex =
								right.pairIndex();
							if (nextLeftIndex != Sentinel)
							{
								eventSet.push(nextLeftIndex);
							}
						}

						// If 'nextLeft' is already visited, the 
						// traversal ends here, since it is not
						// possible to form any more augmenting
						// paths.
					}
				}

				// Whether at least one augmented path was found
				// is seen by whether the sentinel node was visited
				// or not.
				return vertexSet_[Sentinel].visited();
			}

			bool flipPossibleAugmentingPath(integer leftIndex)
			{
				if (leftIndex == Sentinel)
				{
					return true;
				}

				Vertex& left = vertexSet_[leftIndex];
				const integer edges = left.edges();
				for (integer i = 0;i < edges;++i)
				{
					Vertex& right = adjacent(left, i);
					Vertex& nextLeft = pair(right);
					if (nextLeft.level() == left.level() + 1)
					{
						const integer nextLeftIndex =
							right.pairIndex();
						if (flipPossibleAugmentingPath(nextLeftIndex))
						{
							right.setPair(leftIndex);
							const integer rightIndex = 
								left.edge(i);
							left.setPair(rightIndex);

							return true;
						}
					}
				}

				left.unvisit();

				return false;
			}

			void initialize(
				Vertex_Pair_ForwardRange range,
				const As_Pair& ignoreAsPair)
			{
				Type_As_Pair asPair;

				// * Throw away duplicate edges.

				// The left vertices and right vertices
				// are disjoint sets. Since we allow to use equal
				// objects for both vertex sets, the disjointness
				// must be guaranteed by separate identification 
				// maps.

				// Associates each left vertex with an integer.
				LeftMap leftMap(1, leftHash_);
				// Associates each right vertex with an integer.
				RightMap rightMap(1, rightHash_);

				// Directed edges between integers.
				std::vector<KeyValue<integer, integer> > edgeSet;

				// The 0 index is reserved to the sentinel 
				// vertex here.
				integer vertices = 1;
				while(!range.empty())
				{
					const Left& left = asPair.left(range.front());
					const Right& right = asPair.right(range.front());

					integer i = vertices;
					if (leftMap.count(left))
					{
						// We have already identified this left vertex
						// with an integer.
						i = leftMap[left];
					}
					else
					{
						// This is a new left vertex, which we shall
						// identify with the integer i.
						leftMap.insert(
							std::make_pair(left, i));
						inverseLeftMap_.insert(
							std::make_pair(i, left));
						leftSet_.push_back(i);
						++vertices;
					}

					integer j = vertices;
					if (rightMap.count(right))
					{
						// We have already identified this right vertex
						// with an integer.
						j = rightMap[right];
					}
					else
					{
						// This is a new right vertex, which we shall
						// identify with the integer i.
						rightMap.insert(
							std::make_pair(right, j));
						inverseRightMap_.insert(
							std::make_pair(j, right));
						++vertices;
					}

					ASSERT_OP(i, !=, j);

					// Add an edge to both directions.
					edgeSet.push_back(keyValue(i, j));
					edgeSet.push_back(keyValue(j, i));

					range.pop_front();
				}

				// Sort the edge-set of each vertex to an interval.
				std::sort(edgeSet.begin(), edgeSet.end());

				// This is the number of _directed_ edges.
				const integer edges = edgeSet.size();

				// Reserving memory in advance here is important to
				// avoid pointer-invalidating reallocations later.
				// We are using resize() because I don't know whether
				// reserve() guarantees the validity of 
				// &adjacencySet_[edge] on an empty vector.
				adjacencySet_.resize(edges);
				// No such worries here, this is only for performance.
				vertexSet_.reserve(vertices);

				integer edge = 0;
				for (integer i = 0;i < vertices;++i)
				{
					// The pointer remains valid, since there will be no
					// resizes thanks to an early reserve() call.
					integer* edgeBegin = 
						&adjacencySet_[edge];

					integer adjacencies = 0;
					// Since edgeSet is sorted primarily according to
					// the first vertex, the edges of a vertex are
					// grouped in an interval.
					while(edge < edges && edgeSet[edge].key() == i)
					{
						adjacencySet_[edge] = edgeSet[edge].value();
						++adjacencies;
						++edge;
					}

					// Each non-sentinel vertex has at least one adjacent 
					// edge, since the vertices are induced by the input 
					// edge-set.
					ASSERT(
						(i == 0 && adjacencies == 0) || 
						(i > 0 && adjacencies > 0));

					// Create the vertex.
					vertexSet_.push_back(
						Vertex(edgeBegin, adjacencies));
				}
			}

		private:
			enum
			{
				Sentinel = 0
			};

			class Vertex
			{
			public:
				Vertex(integer* edgeBegin, integer edges)
					: pair_(Sentinel)
					, level_(0)
					, edgeBegin_(edgeBegin)
					, edges_(edges)
				{
					ASSERT(edgeBegin);
					ASSERT_OP(edges, >=, 0);
				}

				void visit(integer level)
				{
					ASSERT_OP(level, >=, 0);
					level_ = level;
				}

				void unvisit()
				{
					level_ = -1;
				}

				bool visited() const
				{
					return level_ >= 0;
				}

				bool paired() const
				{
					return pair_ != Sentinel;
				}

				integer* edgeBegin() const
				{
					return edgeBegin_;
				}

				integer edges() const
				{
					return edges_;
				}

				integer edge(integer index)
				{
					ASSERT_OP(index, <, edges());
					ASSERT_OP(index, >=, 0);

					return edgeBegin_[index];
				}

				void setPair(integer pair)
				{
					pair_ = pair;
				}

				integer pairIndex() const
				{
					return pair_;
				};

				integer level() const
				{
					return level_;
				}

			private:
				integer pair_;
				integer level_;
				integer* edgeBegin_;
				integer edges_;
			};

			Vertex& pair(Vertex& vertex)
			{
				return vertexSet_[vertex.pairIndex()];
			}

			Vertex& adjacent(Vertex& vertex, integer i)
			{
				return vertexSet_[vertex.edge(i)];
			}

			std::vector<integer> adjacencySet_;
			std::vector<Vertex> vertexSet_;
			std::vector<integer> leftSet_;

			InverseLeftMap inverseLeftMap_;
			InverseRightMap inverseRightMap_;

			LeftHash leftHash_;
			RightHash rightHash_;
		};

	}

	template <
		typename Vertex_Pair_ForwardRange, 
		typename Vertex_Pair_Reporter,
		typename As_Pair,
		typename LeftHash,
		typename RightHash>
	void maximumBipartiteMatching(
		Vertex_Pair_ForwardRange range,
		const Vertex_Pair_Reporter& reporter,
		const As_Pair& asPair,
		const LeftHash& leftHash,
		const RightHash& rightHash)
	{
		MaximumBipartiteMatching::Algorithm<Vertex_Pair_ForwardRange,
			Vertex_Pair_Reporter, As_Pair, LeftHash, RightHash> algorithm;

		algorithm.work(range, reporter, asPair, leftHash, rightHash);
	}

	template <
		typename Vertex_Pair_ForwardRange, 
		typename Vertex_Pair_Reporter,
		typename As_Pair>
	void maximumBipartiteMatching(
		Vertex_Pair_ForwardRange range,
		const Vertex_Pair_Reporter& reporter,
		const As_Pair& asPair)
	{
		typedef typename Vertex_Pair_ForwardRange::value_type Type;
		typedef typename As_Pair::template ConceptMap<Type> Type_As_Pair;
		typedef typename Type_As_Pair::Left Left;
		typedef typename Type_As_Pair::Right Right;

		Pastel::maximumBipartiteMatching(range, reporter, asPair,
			std::hash<Left>(), std::hash<Right>());
	}

	template <
		typename Tuple2_ForwardRange,
		typename Vertex_Pair_Reporter>
	void maximumBipartiteMatching(
		Tuple2_ForwardRange range,
		const Vertex_Pair_Reporter& reporter)
	{
		Pastel::maximumBipartiteMatching(
			range, reporter, Tuple_As_Pair());
	}

}

#endif
