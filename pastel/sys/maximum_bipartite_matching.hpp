#ifndef PASTEL_MAXIMUM_BIPARTITE_MATCHING_HPP
#define PASTEL_MAXIMUM_BIPARTITE_MATCHING_HPP

#include "pastel/sys/maximum_bipartite_matching.h"
#include "pastel/sys/unorderedmap.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/keyvalue.h"

#include <vector>
#include <queue>

namespace Pastel
{

	namespace MaximumBipartiteMatching
	{

		class Algorithm
		{
		public:
			void work()
			{
				// This is the Hopcroft-Karp algorithm for 
				// maximum bipartite matching.

				initialize();

				while(findAugmentingPaths())
				{
					// As long as there are augmenting paths...
					const integer leftVertices = leftSet.size();
					for (integer i = 0;i < leftVertices;++i)
					{
						const integer leftIndex = leftSet[i];
						Vertex& left = vertexSet_[leftIndex];
						if (!left.paired())
						{
							// ... flip the augmenting paths to 
							// give a larger matching.
							flipPossibleAugmentingPath(leftIndex);
						}
					}

				}
			}

		private:
			bool findAugmentingPaths()
			{
				std::queue<integer> eventSet;

				vertexSet[Sentinel].unvisit();

				const integer leftVertices = leftSet.size();
				for (integer i = 0;i < leftVertices;++i)
				{
					Vertex& vertex = 
						vertexSet_[leftSet[i]];
					if (!vertex.paired())
					{
						vertex.visit(0);
						eventSet.push(i);
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
					Vertex& left = 
						vertexSet[eventSet.front()];
					eventSet.pop();

					// As an invariant, 'left' is not paired.
					// Thus any edge leaving from 'left' is
					// not part of the match yet.
					ASSERT(!left.paired());

					const integer edges = vertex.edges();
					for (integer i = 0;i < edges;++i)
					{
						Vertex& right =
							adjacent(vertex, i);

						// The 'nextLeft' is either a sentinel
						// node or a left vertex. If it is a 
						// sentinel (i.e. 'right' is not paired),
						// then it means we have found at least 
						// one augmenting path.
						Vertex& nextLeft = 
							right.pair();

						if (!nextLeft.visited())
						{
							// If 'nextLeft' is not visited,
							// visit it now.
							nextLeft.visit(left.level() + 1);

							// And continue the breadth-first
							// traversal...
							const integer nextLeftIndex =
								right.pairIndex();
							eventSet.push(nextLeftIndex);
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
				return vertexSet[Sentinel].visited();
			}

			bool flipPossibleAugmentingPath(integer leftIndex)
			{
				if (index == Sentinel)
				{
					return true;
				}

				Vertex& left = vertexSet_[leftIndex];
				const integer edges = vertex.edges();
				for (integer i = 0;i < edges;++i)
				{
					Vertex& right = adjacent(left, i);
					Vertex& nextLeft = right.pair();
					if (nextLeft.level() == left.level() + 1)
					{
						const integer nextLeftIndex =
							right.pairIndex();
						if (depthFirst(nextLeftIndex))
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

			void initialize()
			{
				// * Throw away duplicate edges.

				typedef UnorderedMap<Type, integer> VertexMap;

				// The left vertices and right vertices
				// are disjoint sets. Since we allow to use equal
				// objects for both vertex sets, the disjointness
				// must be guaranteed by separate identification 
				// maps.

				// Associates each left vertex with an integer.
				VertexMap leftMap;
				// Associates each right vertex with an integer.
				VertexMap rightMap;
				// Associates each index with a vertex.
				std::vector<Type> inverseMap;

				// Directed edges between integers.
				std::vector<KeyValue<integer, integer> > edgeSet;

				// The 0 index is reserved to the sentinel 
				// vertex here.
				integer vertices = 1;
				while(...)
				{
					const Type& left = ...;
					const Type& right = ...;

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
						inverseMap.push_back(left);
						leftSet.push_back(i);
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
						inverseMap.push_back(right);
						++vertices;
					}

					ASSERT_OP(i, !=, j);

					// Add an edge to both directions.
					edgeSet.push_back(keyValue(i, j));
					edgeSet.push_back(keyValue(j, i));
				}

				// Sort the edge-set of each vertex to an interval.
				std::sort(edgeSet.begin(), edgeSet.end());

				// This is the number of _directed_ edges.
				const integer edges = edgeSet.size();

				// Reserving memory in advance here is important to
				// avoid pointer-invalidating reallocations later.
				adjacencySet_.reserve(edges);
				// No such worries here, this is only for performance.
				vertexSet_.reserve(vertices);

				// Create the sentinel vertex.
				{
					const integer* edgeBegin = 
						&adjacencySet_.front() + adjacencySet.size();
					const integer adjacencies = 0;
					vertexSet_.push_back(
						Vertex(edgeBegin, adjacencies));
				}

				integer edge = 0;
				for (integer i = 0;i < vertices;++i)
				{
					// The pointer remains valid, since there will be no
					// resizes thanks to an early reserve() call.
					const integer* edgeBegin = 
						&adjacencySet_.front() + adjacencySet.size();

					integer adjacencies = 0;
					// Since edgeSet is sorted primarily according to
					// the first vertex, the edges of a vertex are
					// grouped in an interval.
					while(edge < edges && edgeSet[edge].key() == i)
					{
						// No resizing occurs because of the reserve() call.
						adjacencySet_.push_back(edgeSet[edge].value());
						++adjacencies;
						++edge;
					}

					// Each vertex has at least one adjacent edge,
					// since the vertices are induced by the input edge-set.
					ASSERT_OP(adjacencies, >, 0);

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
					ASSERT_OP(edges, >, 0);
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

				integer edgeBegin() const
				{
					return edgeBegin_;
				}

				integer edges() const
				{
					return edges_;
				}

				integer edge(const Vertex& vertex, integer index)
				{
					ASSERT_OP(index, <, vertex.edges());
					ASSERT_OP(index, >=, 0);

					return edgeBegin_[index];
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
		};

	}

	void maximumBipartiteMatching()
	{
		MaximumBipartiteMatching::Algorithm algorithm;

		algorithm.work();
	}

}

#endif
