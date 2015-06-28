#ifndef PASTELSYS_MAXIMUM_BIPARTITE_MATCHING_HPP
#define PASTELSYS_MAXIMUM_BIPARTITE_MATCHING_HPP

#include "pastel/sys/graph/matching/maximum_bipartite_matching.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/output/null_output.h"

#include <vector>
#include <queue>

namespace Pastel
{

	template <
		typename ForEachAdjacentToA,
		typename... ArgumentSet>
	integer maximumBipartiteMatching(
		integer nA,
		integer nB,
		const ForEachAdjacentToA& forEachAdjacentToA,
		ArgumentSet&&... argumentSet)
	{
		// "An n^(5/2) algorithm for maximum matchings in bipartite graphs",
		// Hopcroft, John E.; Karp, Richard M., 
		// SIAM Journal on Computing 2 (4): 225–231, 1973.

		ENSURE_OP(nA, >=, 0);
		ENSURE_OP(nB, >=, 0);

		// Let G = (V, E) be a simple directed graph, such that
		// 
		//     A = {0} x [0, nA[,
		//     B = {1} x [0, nB[,
		//     V = A union B,
		//     E subset A x B.
		//
		// The G is a bipartite graph. A _matching_ in G is M subset E,
		// such that 
		//
		//     (a_1, b_1), (a_2, b_2) in M ==> a_1 != a_2 and b_1 != b_2,
		//
		// for all a_1, a_2 in A, b_1, b_2 in B. We wish to find a 
		// matching with maximum cardinality |M|.

		if (nA == 0 || nB == 0)
		{
			// Since either A or B is empty, there are no matchings 
			// in the graph.
			return 0;
		}

		auto&& report = PASTEL_ARG_S(report, nullOutput());

		// We store sentinel vertices for both sets
		// at the last index.
		integer A_Sentinel = nA;
		integer B_Sentinel = nB;

		// The A-set of vertices.
		struct A_Vertex
		{
			// In the current matching, the vertex 
			// in the B-set this vertex is paired to.
			integer pair;
			// The shortest distance to an unpaired
			// vertex in the A-set. 
			integer distance; 
		};

		// The A-set of vertices + A-sentinel.
		std::vector<A_Vertex> aSet(nA + 1);
		for (auto&& aVertex : aSet)
		{
			aVertex.pair = B_Sentinel;
			aVertex.distance = infinity<integer>();
		}

		A_Vertex& aSentinel = aSet[A_Sentinel];

		// The B-set of vertices.
		struct B_Vertex
		{
			// In the current matching, the vertex 
			// in the A-set this vertex is paired to.
			integer pair;
		};
		
		// The B-set of vertices + B-sentinel.
		std::vector<B_Vertex> bSet(nB + 1);
		for (auto&& bVertex : bSet)
		{
			bVertex.pair = A_Sentinel;
		}

		// Conceptually, the A-sentinel (B-Sentinel)
		// is connected to every vertex in the A-set (B-Set)
		// (and not any other vertex). An _augmenting-path_ is
		// a path which starts at the A-sentinel, alternates 
		// between an edge in M and an edge not in M, and 
		// ends at the B-sentinel. 

		// For each vertex reachable from an unpaired
		// vertex of A, computes the shortest distance
		// to an unpaired vertex of A. Returns whether
		// the A-sentinel was reached or not.
		auto computeDistances = [&]() -> bool
		{
			// This is a breadth-first search; the
			// queue keeps up the order of the search.
			std::queue<integer> queue;

			// Initialize the queue with unpaired
			// vertices of A, and initialize the
			// distances in A.
			for (integer a = 0;a < nA;++a)
			{
				// This initialization is only for 
				// non-sentinel vertices.
				ASSERT(a != A_Sentinel);

				if (aSet[a].pair == B_Sentinel)
				{
					// The 'a' is an unpaired vertex of A.
					aSet[a].distance = 0;
					queue.push(a);
				}
				else
				{
					// The 'a' is a paired vertex of A.
					// Set its distance to infinity; this
					// denotes that it has not been computed yet.
					// The distance to 'a' stays at infinity
					// if 'a' cannot be reached from 
					// an unpaired vertex of A, or it takes
					// too many steps.
					aSet[a].distance = infinity<integer>();
				}
			}

			// Mark the A-sentinel distance as not computed yet.
			aSentinel.distance = infinity<integer>();
			while(!queue.empty())
			{
				integer a = queue.front();
				ASSERT(a != A_Sentinel);

				queue.pop();

				if (aSet[a].distance >= aSentinel.distance)
				{
					// At each phase, we accept only shortest paths
					// leading to the A-sentinel. This is important
					// for algorithm correctness. Since at 'a' the 
					// path already is as long as a shortest path, 
					// it cannot produce new shortest paths.
					continue;
				}

				forEachAdjacentToA(a, [&](integer b)
				{
					PENSURE_RANGE(b, 0, nB);

					integer aNext = bSet[b].pair;

					// Using < instead of != here triggers 
					// a bug in Visual Studio 2015 RC, in which
					// the compiler hangs.
					if (aSet[aNext].distance != infinity<integer>())
					{
						// The distance to 'aNext' has already been
						// computed.
						return  true;
					}

					// Compute the distance to 'aNext'.	
					aSet[aNext].distance = aSet[a].distance + 1;

					if (aNext != A_Sentinel)
					{
						// Recurse back to 'aNext' later, provided
						// it is not the A-sentinel.
						queue.push(aNext);
					}

					// Proceed to the next vertex adjacent to 'a'.
					return true;
				});
			}

			// Return whether the A-sentinel was reached or not.
			// Using < here instead of != triggers a bug in g++ 5.1.0.
			return aSentinel.distance != infinity<integer>();
		};

		// Using the distances computed by 'computeDistances',
		// flips a shortest augmenting-path to produce
		// a larger matching. Returns whether an
		// augmenting path was found.
		auto flipAugmentingPath = [&](
			auto&& self, integer a) -> bool
		{
			// Here we use a generic lambda to
			// pass 'flipAugmentingPath' to 'self', so that it
			// can be recursively called.
			ASSERT_RANGE(a, 0, nA + 1);
			
			if (a == A_Sentinel)
			{
				// Since we reached the A-sentinel, 
				// we found an augmenting path.
				return true;
			}

			bool foundPath = false;
			forEachAdjacentToA(a, [&](integer b)
			{
				PENSURE_RANGE(b, 0, nB);
				// This line triggers a bug in Visual Studio 2015 RC.
				//ASSERT(aSet[a].distance < infinity<integer>());

				bool UserStoppedCallingWhenRequested = !foundPath;
				PENSURE(UserStoppedCallingWhenRequested);
				unused(UserStoppedCallingWhenRequested);

				integer aNext = bSet[b].pair;
				if (aSet[aNext].distance != aSet[a].distance + 1)
				{
					// The vertex 'aNext' is not on a shortest 
					// path; do not follow it.

					// Next adjacent vertex.
					return true;
				}

				// Follow the shortest path recursively;
				// this is a depth-first search.
				foundPath = self(self, aNext);
				if (foundPath)
				{
					// We are on a shortest augmenting path.
					// Pair the edges on it as we back off.
					aSet[a].pair = b;
					bSet[b].pair = a;

					// Stop visiting adjacent vertices.
					return false;
				}

				// Next adjacent vertex.
				return true;
			});

			if (foundPath)
			{
				// Mark 'aStart' so that it will not be
				// searched again.
				aSet[a].distance = infinity<integer>();
			}

			return foundPath;
		};

		// The algorithm is finished when the A-sentinel cannot
		// be reached; then there are no augmenting-paths.
		while(computeDistances())
		{
			integer flips = 0;
			for (integer a = 0;a < nA;++a)
			{
				if (aSet[a].pair != B_Sentinel)
				{
					continue;
				}

				// This is an unpaired vertex of A.
				// Find a shortest augmenting-path 
				// starting from this vertex, and flip it.
				if (flipAugmentingPath(flipAugmentingPath, a))
				{
					++flips;
				}
			}
			ASSERT(flips > 0);
		}

		// Report and count the edges in the matching.
		integer nMatch = 0;
		for (integer a = 0;a < nA;++a)
		{
			integer b = aSet[a].pair;
			if (b != B_Sentinel)
			{
				ASSERT_OP(bSet[b].pair, ==, a);
				report(a, b);
				++nMatch;
			}
		}

		// Return the number of edges in the matching.
		return nMatch;
	};

}

#endif
