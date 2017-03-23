// Description: PointKdTree Matlab interface
// Documentation: pointkdtree_matlab.txt

#include "pastel/geometry/pointkdtree/pointkdtree.h"
#include "pastel/geometry/search_nearest.h"
#include "pastel/geometry/nearestset/kdtree_nearestset.h"
#include "pastel/math/normbijection.h"

#include "pastel/sys/allocator/pool_allocator.h"
#include "pastel/sys/indicator/predicate_indicator.h"

#include <boost/range/algorithm/fill.hpp>

#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>

#include <unordered_map>

#include "pastel/matlab/pastelmatlab.h"

void force_linking_pointkdtree() {}

namespace Pastel
{

	namespace
	{

		struct TreePoint
		{
			TreePoint()
				: data(0)
				, id(0)
			{
			}

			TreePoint(real* data_, integer id_)
				: data(data_)
				, id(id_)
			{
			}

			real* data;
			integer id;
		};

		class TreePoint_Locator
		{
		public:
			using Real = real;
			using Point = TreePoint;
			static constexpr integer N = Dynamic;

			explicit TreePoint_Locator(
				integer dimension)
				: dimension_(dimension)
			{
			}

			const Real& operator()(
				const Point& point, integer i) const
			{
				return *(point.data + i);
			}

			integer n() const
			{
				return dimension_;
			}

			integer n(const Point& point) const
			{
				return n();
			}

		private:
			integer dimension_;
		};

		using Settings = PointKdTree_Settings<TreePoint_Locator>;

		using Tree = PointKdTree<Settings>;
		using Point_ConstIterator = Tree::Point_ConstIterator;
		using Point_ConstRange = Tree::Point_ConstRange;
		using IndexMap = std::unordered_map<integer, Point_ConstIterator>;
		using ConstIterator = IndexMap::const_iterator;

		struct KdState
		{
			explicit KdState(integer dimension)
				: tree(TreePoint_Locator(dimension))
				, indexMap()
				, pointAllocator(dimension * sizeof(real))
				, index(1)
			{
			}

			explicit KdState(const KdState& that)
				: tree(that.tree)
				, indexMap()
				, pointAllocator(that.tree.n() * sizeof(real))
				, index(that.index)
			{
				// Remove points, but retain structure.
				tree.erase(true);

				const Tree& thatTree = that.tree;
				integer dimension = thatTree.n();

				// Copy point data.
				{
					Point_ConstRange range = thatTree.range();
					while(!range.empty())
					{

						const TreePoint& treePoint =
							range.front().point();
						
						real* data = (real*)pointAllocator.allocate();
						real* thatData = treePoint.data;

						std::copy(thatData, thatData + dimension, data);

						integer index = treePoint.id;

						Point_ConstIterator iter = tree.insert(TreePoint(data, index));
						indexMap.insert(std::make_pair(index, iter));

						range.pop_front();
					}
				}

				// Copy hidden data.
				{
					Point_ConstRange range = thatTree.hiddenRange();
					while(!range.empty())
					{

						const TreePoint& treePoint =
							range.front().point();
						
						real* data = (real*)pointAllocator.allocate();
						real* thatData = treePoint.data;

						std::copy(thatData, thatData + dimension, data);

						integer index = treePoint.id;

						Point_ConstIterator iter = tree.insert(
							TreePoint(data, index), true);
						indexMap.insert(std::make_pair(index, iter));

						range.pop_front();
					}
				}
			}

			~KdState()
			{
				tree.clear();
				pointAllocator.clear();
			}

			Tree tree;
			// Mapping from integer identifiers to point iterators.
			IndexMap indexMap;
			// Memory region for point data.
			PoolAllocator pointAllocator;
			// The current allocation index for identifiers.
			integer index;
		};

		KdState* asState(const mxArray* matlabArray)
		{
			return *((KdState**)mxGetData(matlabArray));
		}

		void kdAsPoints(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			enum
			{
				State,
				IdSet,
				Inputs
			};

			enum
			{
				PointSet,
				Outputs
			};

			ENSURE_OP(inputs, ==, Inputs);

			KdState* state = asState(inputSet[State]);
			Array<integer> idSet = matlabAsLinearizedArray<integer>(inputSet[IdSet]);

			integer d = state->tree.n();
			integer n = idSet.size();

			Array<real> pointSet = matlabCreateArray<real>(
				n, d, outputSet[PointSet]);
			for (integer i = 0;i < n;++i)
			{
				integer id = idSet(i);
				ConstIterator iter = state->indexMap.find(id);

				if (iter != state->indexMap.end())
				{
					Point_ConstIterator pointIter =
						iter->second;

					std::copy(
						pointIter->point().data,
						pointIter->point().data + d,
						pointSet.columnBegin(i));
				}					
				else
				{
					std::fill(
						pointSet.columnBegin(i),
						pointSet.columnEnd(i),
						(real)Nan());
				}
			}
		}

		void kdConstruct(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			enum
			{
				Dimension,
				Inputs
			};

			ENSURE_OP(inputs, ==, Inputs);
			
			integer dimension = matlabAsScalar<integer>(inputSet[Dimension]);

			// Get enough memory to hold a KdState pointer.

			KdState** rawResult = matlabCreateScalar<KdState*>(outputSet[0]);

			// Create a new KdState and store the pointer to
			// the returned array.
			KdState* state = new KdState(dimension);
			*rawResult = state;
		}

		void kdDestruct(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			enum
			{
				State,
				Inputs
			};

			ENSURE_OP(inputs, ==, Inputs);

			KdState* state = asState(inputSet[State]);
			delete state;
		}

		void kdCopy(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			enum
			{
				State,
				Inputs
			};

			ENSURE_OP(inputs, ==, Inputs);
			
			KdState* state = asState(inputSet[State]);

			// Get enough memory to hold a KdState pointer.
			KdState** rawResult = matlabCreateScalar<KdState*>(outputSet[0]);

			// Create a new KdState and store the pointer to
			// the returned array.
			KdState* newState = new KdState(*state);
			*rawResult = newState;
		}

		void kdCheck(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			enum
			{
				State,
				Inputs
			};

			ENSURE_OP(inputs, ==, Inputs);
			
			KdState* state = asState(inputSet[State]);
			Tree& tree = state->tree;

			ENSURE(testInvariants(tree));
		}

		void kdMerge(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			enum
			{
				State,
				Inputs
			};

			ENSURE_OP(inputs, ==, Inputs);

			KdState* state = asState(inputSet[State]);
			state->tree.merge();
		}

		void kdClear(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			enum
			{
				State,
				Inputs
			};

			ENSURE_OP(inputs, ==, Inputs);

			KdState* state = asState(inputSet[State]);
			state->tree.clear();
		}

		void kdInsert(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			enum
			{
				State,
				PointSet,
				Inputs
			};

			enum
			{
				IdSet,
				Outputs
			};

			ENSURE_OP(inputs, ==, Inputs);

			KdState* state = asState(inputSet[State]);
			Tree& tree = state->tree;

			Array<real> pointSet = matlabAsArray<real>(inputSet[PointSet]);
			integer points = pointSet.width();

			Array<integer> result =
				matlabCreateArray<integer>(points, 1, outputSet[IdSet]);
			for (integer i = 0;i < points;++i)
			{

				real* data = (real*)state->pointAllocator.allocate();
				std::copy(pointSet.cColumnBegin(i), pointSet.cColumnEnd(i), data);

				integer index = state->index;
				Point_ConstIterator iter = tree.insert(TreePoint(data, index));
				state->indexMap.insert(std::make_pair(index, iter));
				++state->index;

				result(i) = index;
			}
		}

		void kdErase(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			enum
			{
				State,
				IdSet,
				Inputs
			};

			enum
			{
				Outputs
			};

			ENSURE_OP(inputs, >=, IdSet);

			KdState* state = asState(inputSet[State]);
			Tree& tree = state->tree;

			if (inputs <= IdSet)
			{
				// Erase all;
				tree.erase();
				return;
			}

			Array<integer> idSet = matlabAsLinearizedArray<integer>(inputSet[IdSet]);

			integer points = idSet.size();
			for (integer i = 0;i < points;++i)
			{
				integer id = idSet(i);
				ConstIterator iter = 
					state->indexMap.find(id);
				if (iter != state->indexMap.end())
				{
					state->tree.erase(iter->second);
					state->indexMap.erase(id);
				}
			}
		}

		void kdHide(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			enum
			{
				State,
				IdSet,
				Inputs
			};

			enum
			{
				Outputs
			};

			ENSURE_OP(inputs, >=, IdSet);

			KdState* state = asState(inputSet[State]);
			Tree& tree = state->tree;

			if (inputs <= IdSet)
			{
				// Hide all;
				tree.hide();
				return;
			}

			Array<integer> idSet = matlabAsLinearizedArray<integer>(inputSet[IdSet]);

			integer points = idSet.size();
			for (integer i = 0;i < points;++i)
			{
				ConstIterator iter = 
					state->indexMap.find(idSet(i));
				if (iter != state->indexMap.end())
				{
					state->tree.hide(iter->second);
				}
			}
		}

		void kdShow(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			enum
			{
				State,
				IdSet,
				Inputs
			};

			enum
			{
				Outputs
			};

			ENSURE_OP(inputs, >=, IdSet);

			KdState* state = asState(inputSet[State]);
			Tree& tree = state->tree;

			if (inputs <= IdSet)
			{
				// Show all;
				tree.show();
				return;
			}

			Array<integer> idSet = matlabAsLinearizedArray<integer>(inputSet[IdSet]);

			integer points = idSet.size();
			for (integer i = 0;i < points;++i)
			{
				ConstIterator iter = 
					state->indexMap.find(idSet(i));
				if (iter != state->indexMap.end())
				{
					state->tree.show(iter->second);
				}
			}
		}

		void kdRefine(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			enum
			{
				State,
				BucketSize,
				Inputs
			};
			
			ENSURE_OP(inputs, ==, Inputs);

			KdState* state = asState(inputSet[State]);
			integer bucketSize = matlabAsScalar<integer>(inputSet[BucketSize]);

			state->tree.refine(
				SlidingMidpoint_SplitRule(), bucketSize);
		}

		void kdNodes(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			enum
			{
				State,
				Inputs
			};

			enum
			{
				Nodes,
				Outputs
			};

			ENSURE_OP(inputs, ==, Inputs);

			KdState* state = asState(inputSet[State]);

			integer *outNodes = matlabCreateScalar<integer>(outputSet[Nodes]);
			*outNodes = state->tree.nodes();
		}

		void kdLeaves(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			enum
			{
				State,
				Inputs
			};

			enum
			{
				Leaves,
				Outputs
			};

			ENSURE_OP(inputs, ==, Inputs);

			KdState* state = asState(inputSet[State]);

			integer* outLeaves = matlabCreateScalar<integer>(outputSet[Leaves]);
			*outLeaves = state->tree.leaves();
		}

		void kdDimension(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			enum
			{
				State,
				Inputs
			};
			
			enum
			{
				Dimension,
				Outputs
			};

			ENSURE_OP(inputs, ==, Inputs);

			KdState* state = asState(inputSet[State]);

			integer* outDimension = matlabCreateScalar<integer>(
				outputSet[Dimension]);
			*outDimension = state->tree.n();
		}

		void kdPoints(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			enum
			{
				State,
				Inputs
			};

			enum
			{
				Points,
				Outputs
			};

			ENSURE_OP(inputs, ==, Inputs);

			KdState* state = asState(inputSet[State]);

			integer* outPoints = matlabCreateScalar<integer>(
				outputSet[Points]);
			*outPoints = state->tree.points();
		}

		template <typename NormBijection>
		void kdSearchNearest_(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			enum
			{
				State,
				QuerySet,
				MaxDistanceSet,
				KNearest,
				Inputs
			};

			enum
			{
				IdSet,
				DistanceSet,
				Outputs
			};
			
			ENSURE_OP(inputs, ==, Inputs);
			ENSURE_OP(outputs, >=, 1);
			ENSURE_OP(outputs, <=, Outputs);

			// nearestIdSet = matlab_pointkdtree('pointkdtree_search_nearest', ...
			//		kdtree, querySet, maxDistanceSet, kNearest);

			KdState* state = asState(inputSet[State]);
			const IndexMap& indexMap = state->indexMap;
			Array<real> maxDistanceSet = matlabAsLinearizedArray<real>(inputSet[MaxDistanceSet]);
			integer kNearest = matlabAsScalar<integer>(inputSet[KNearest]);

			mxClassID id = mxGetClassID(inputSet[QuerySet]);
			
			bool queriesAreCoordinates = 
				id == mxSINGLE_CLASS ||
				id == mxDOUBLE_CLASS;

			bool wantDistance = (outputs >= 2);

			integer queries = 0;
			if (queriesAreCoordinates)
			{
				queries = mxGetN(inputSet[QuerySet]);
			}
			else
			{
				queries = mxGetNumberOfElements(inputSet[QuerySet]);
			}

			Array<integer> nearestArray =
				matlabCreateArray<integer>(kNearest, queries, outputSet[IdSet]);

			Array<real> distanceArray;
			if (wantDistance)
			{
				// Having the matlabCreateArray<real>() call directly
				// inside the swap() function triggers an
				// internal compiler error in Clang.
				auto copyArray = matlabCreateArray<real>(kNearest, queries, outputSet[DistanceSet]);
				distanceArray.swap(copyArray);
				boost::fill(distanceArray.range(), (real)Infinity());
			}

			using Block = tbb::blocked_range<integer>;

			if (queriesAreCoordinates)
			{
				// The queries are a set of points given explicitly
				// by their coordinates. Each column is a query point.
				Array<real> querySet = matlabAsArray<real>(inputSet[QuerySet]);
				integer n = state->tree.n();

				// Find the k-nearest-neighbors for each point.

				auto search = [&](const Block& block)
				{
					Vector<real> query(ofDimension(n));
					for (integer i = block.begin(); i < block.end(); ++i)
					{
						std::copy(
							querySet.cColumnBegin(i),
							querySet.cColumnEnd(i),
							query.begin());

						integer j = 0;
						auto report = [&](
							real distance,
							Point_ConstIterator point)
						{
							if (point != state->tree.end())
							{
								nearestArray(j, i) = point->point().id;
								if (wantDistance)
								{
									distanceArray(j, i) = distance;
								}
							}
							++j;
						};

						searchNearest(
							kdTreeNearestSet(state->tree), 
							query, 
							PASTEL_TAG(report), report,
							NormBijection(),
							PASTEL_TAG(kNearest), kNearest,
							PASTEL_TAG(maxDistance2), maxDistanceSet(i));
					}
				};

				tbb::parallel_for(Block(0, queries), search);
			}
			else
			{
				// The queries are over the points in the kd-tree,
				// given by their ids.
				Array<integer> querySet = matlabAsLinearizedArray<integer>(inputSet[QuerySet]);
		
				// Find the k-nearest-neighbors for each point.
				auto search = [&](const Block& block)
				{
					for (integer i = block.begin(); i < block.end(); ++i)
					{
						integer j = 0;
						auto report = [&](
							real distance,
							Point_ConstIterator point)
						{
							if (point != state->tree.end())
							{
								nearestArray(j, i) = point->point().id;
								if (wantDistance)
								{
									distanceArray(j, i) = distance;
								}
							}
							++j;
						};

						auto query_ = indexMap.find(querySet(i));
						if (query_ == indexMap.end())
						{
							// The query index does not exist.
							continue;
						}

						Point_ConstIterator query = query_->second;

						searchNearest(
							kdTreeNearestSet(state->tree),
							location(query->point(), state->tree.locator()),
							PASTEL_TAG(report), report,
							PASTEL_TAG(accept), predicateIndicator(query, NotEqualTo()), 
							NormBijection(),
							PASTEL_TAG(kNearest), kNearest,
							PASTEL_TAG(maxDistance2), maxDistanceSet(i));
					}
				};

				tbb::parallel_for(Block(0, queries), search);
			}
		}

		void kdSearchNearest(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			enum
			{
				State,
				QuerySet,
				MaxDistanceSet,
				KNearest,
				Norm,
				Inputs
			};

			enum
			{
				IdSet,
				DistanceSet,
				Outputs
			};
			
			ENSURE_OP(inputs, ==, Inputs);
			ENSURE_OP(outputs, >=, 1);
			ENSURE_OP(outputs, <=, Outputs);

			// nearestIdSet = matlab_pointkdtree('pointkdtree_search_nearest', ...
			//		kdtree, querySet, maxDistanceSet, kNearest, norm);

			std::string norm = matlabAsString(inputSet[Norm]);
			if (norm == "euclidean")
			{
				kdSearchNearest_<Euclidean_NormBijection<real>>(
					outputs, outputSet,
					inputs - 1, inputSet);
			}
			else if (norm == "maximum")
			{
				kdSearchNearest_<Maximum_NormBijection<real>>(
					outputs, outputSet,
					inputs - 1, inputSet);
			}
			else
			{
				bool AllowedNorm = false;
				ENSURE(AllowedNorm);
			}
		}

		template <typename NormBijection>
		void kdCountNearest_(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			enum
			{
				State,
				QuerySet,
				MaxDistanceSet,
				Inputs
			};

			enum
			{
				IdSet,
				Outputs
			};
			
			ENSURE_OP(inputs, ==, Inputs);
			ENSURE_OP(outputs, >=, 1);
			ENSURE_OP(outputs, <=, Outputs);

			// nearestIdSet = matlab_pointkdtree('pointkdtree_count_nearest', ...
			//		kdtree, querySet, maxDistanceSet, norm);

			KdState* state = asState(inputSet[State]);
			const IndexMap& indexMap = state->indexMap;
			Array<real> maxDistanceSet = matlabAsLinearizedArray<real>(inputSet[MaxDistanceSet]);
			integer queries = mxGetNumberOfElements(inputSet[QuerySet]);
			ENSURE_OP(maxDistanceSet.size(), ==, queries);

			// The queries are over the points in the kd-tree,
			// given by their ids.
			Array<integer> querySet = matlabAsLinearizedArray<integer>(inputSet[QuerySet]);

			// Create the result array.
			Array<integer> result =
				matlabCreateArray<integer>(queries, 1, outputSet[IdSet]);

			// Count the neighbors for each point.

			using Block = tbb::blocked_range<integer>;

			auto count = [&](const Block& block)
			{
				for (integer i = block.begin(); i < block.end(); ++i)
				{
					ConstIterator query_ = indexMap.find(querySet(i));
					if (query_ == indexMap.end())
					{
						continue;
					}

					Point_ConstIterator query = query_->second;

					integer count = 0;

					searchNearest(
						kdTreeNearestSet(state->tree),
						location(query->point(), state->tree.locator()),
						NormBijection(),
						PASTEL_TAG(report), [&](auto, auto) {++count;},
						PASTEL_TAG(kNearest), state->tree.points(),
						PASTEL_TAG(maxDistance2), maxDistanceSet(i));

					result(i) = count;
				}
			};

			tbb::parallel_for(Block(0, queries), count);
		}

		void kdCountNearest(
			int outputs, mxArray *outputSet[],
			int inputs, const mxArray *inputSet[])
		{
			enum
			{
				State,
				QuerySet,
				MaxDistanceSet,
				Norm,
				Inputs
			};

			enum
			{
				IdSet,
				Outputs
			};

			ENSURE_OP(inputs, ==, Inputs);
			ENSURE_OP(outputs, >=, 1);
			ENSURE_OP(outputs, <=, Outputs);

			// nearestIdSet = matlab_pointkdtree('pointkdtree_count_nearest', ...
			//		kdtree, querySet, maxDistanceSet, norm);

			std::string norm = matlabAsString(inputSet[Norm]);
			if (norm == "euclidean")
			{
				kdCountNearest_<Euclidean_NormBijection<real>>(
					outputs, outputSet,
					inputs - 1, inputSet);
			}
			else if (norm == "maximum")
			{
				kdCountNearest_<Maximum_NormBijection<real>>(
					outputs, outputSet,
					inputs - 1, inputSet);
			}
			else
			{
				bool AllowedNorm = false;
				ENSURE(AllowedNorm);
			}
		}

		void addStuff()
		{
			matlabAddFunction("pointkdtree_as_points", kdAsPoints);
			matlabAddFunction("pointkdtree_construct", kdConstruct);
			matlabAddFunction("pointkdtree_destruct", kdDestruct);
			matlabAddFunction("pointkdtree_copy", kdCopy);
			matlabAddFunction("pointkdtree_check", kdCheck);
			matlabAddFunction("pointkdtree_merge", kdMerge);
			matlabAddFunction("pointkdtree_clear", kdClear);
			matlabAddFunction("pointkdtree_points", kdPoints);
			matlabAddFunction("pointkdtree_nodes", kdNodes);
			matlabAddFunction("pointkdtree_leaves", kdLeaves);
			matlabAddFunction("pointkdtree_dimension", kdDimension);
			matlabAddFunction("pointkdtree_insert", kdInsert);
			matlabAddFunction("pointkdtree_erase", kdErase);
			matlabAddFunction("pointkdtree_hide", kdHide);
			matlabAddFunction("pointkdtree_show", kdShow);
			matlabAddFunction("pointkdtree_refine", kdRefine);
			matlabAddFunction("pointkdtree_search_nearest", kdSearchNearest);
			matlabAddFunction("pointkdtree_count_nearest", kdCountNearest);
		}

		CallFunction run(addStuff);
	
	}

}
