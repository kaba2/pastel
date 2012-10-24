// Description: PointKdTree Matlab interface

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/search_all_neighbors_pointkdtree.h"
#include "pastel/geometry/search_all_neighbors_bruteforce.h"

#include "pastel/matlab/pastelmatlab.h"

#include "pastel/sys/pointpolicies.h"
#include "pastel/sys/pool_allocator.h"

#include <unordered_map>

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

		class TreePoint_PointPolicy
		{
		public:
			typedef real Real;
			typedef TreePoint Point;
			typedef const Real* ConstIterator;

			explicit TreePoint_PointPolicy(
				integer dimension)
				: dimension_(dimension)
			{
			}

			typedef boost::iterator_range<ConstIterator>
				ConstRange;

			typedef Array_VectorExpression<real, Dynamic>
				Expression;

			const Real& axis(const Point& point, integer i) const
			{
				return *(point.data + i);
			}

			Expression operator()(const Point& point) const
			{
				return Expression(begin(point), n());
			}

			ConstRange range(const Point& point) const
			{
				return ConstRange(begin(point), end(point));
			}

			ConstIterator begin(const Point& point) const
			{
				return point.data;
			}

			ConstIterator end(const Point& point) const
			{
				return point.data + dimension_;
			}

			const Real* point(const Point& point) const
			{
				return point.data;
			}

			Real point(const Point& point, integer axis) const
			{
				return point.data[axis];
			}

			integer n() const
			{
				return dimension_;
			}

			integer dimension(const Point& point) const
			{
				return dimension_;
			}

		private:
			integer dimension_;
		};

		typedef PointKdTree<real, Dynamic, TreePoint_PointPolicy> Tree;
		typedef Tree::Point_ConstIterator Point_ConstIterator;
		typedef Tree::Point_ConstRange Point_ConstRange;
		typedef std::unordered_map<integer, Point_ConstIterator> IndexMap;
		typedef IndexMap::const_iterator ConstIterator;

		struct KdState
		{
			explicit KdState(integer dimension)
				: tree(TreePoint_PointPolicy(dimension))
				, indexMap()
				, pointAllocator(dimension * sizeof(real))
				, index(0)
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
				const integer dimension = thatTree.n();

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

						const integer index = treePoint.id;

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

						const integer index = treePoint.id;

						Point_ConstIterator iter = tree.insert(
							TreePoint(data, index), true);
						indexMap.insert(std::make_pair(index, iter));

						range.pop_front();
					}
				}
			}

			~KdState()
			{
				pointAllocator.clear();
			}

			typedef std::vector<TreePoint> PointSet;

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
			return *((KdState**)mxGetPr(matlabArray));
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
			Array<integer> idSet = asLinearizedArray<integer>(inputSet[IdSet]);

			const integer d = state->tree.n();
			const integer n = idSet.size();

			typedef Tree::PointPolicy_ PointPolicy;
			const PointPolicy& pointPolicy = state->tree.pointPolicy();

			Array<real> pointSet = createArray<real>(
				n, d, outputSet[PointSet]);
			for (integer i = 0;i < n;++i)
			{
				const integer id = idSet(i);
				ConstIterator iter = state->indexMap.find(id);

				if (iter != state->indexMap.end())
				{
					Point_ConstIterator pointIter =
						iter->second;

					std::copy(
						pointPolicy.begin(pointIter->point()),
						pointPolicy.end(pointIter->point()),
						pointSet.columnBegin(i));
				}					
				else
				{
					std::fill(
						pointSet.columnBegin(i),
						pointSet.columnEnd(i),
						nan<real>());
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
			
			const integer dimension = asScalar<integer>(inputSet[Dimension]);

			// Get enough memory to hold a KdState pointer.
			const mwSize extent[] = {1, sizeof(KdState*)};
			outputSet[0] = mxCreateCharArray(2, extent);
			KdState** rawResult = (KdState**)mxGetPr(outputSet[0]);

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
			const mwSize extent[] = {1, sizeof(KdState*)};
			outputSet[0] = mxCreateCharArray(2, extent);
			KdState** rawResult = (KdState**)mxGetPr(outputSet[0]);

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

			ENSURE(check(tree));
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
			const integer dimension = tree.n();
			const real* pointSet = mxGetPr(inputSet[PointSet]);
			const integer elements = mxGetNumberOfElements(inputSet[PointSet]);
			const integer points = elements / tree.n();

			Array<integer> result =
				createArray<integer>(points, 1, outputSet[IdSet]);
			for (integer i = 0;i < points;++i)
			{
				real* data = (real*)state->pointAllocator.allocate();
				std::copy(pointSet, pointSet + dimension, data);
				pointSet += dimension;

				const integer index = state->index;
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

			Array<integer> idSet = asLinearizedArray<integer>(inputSet[IdSet]);

			const integer points = idSet.size();
			for (integer i = 0;i < points;++i)
			{
				const integer id = idSet(i);
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

			Array<integer> idSet = asLinearizedArray<integer>(inputSet[IdSet]);

			const integer points = idSet.size();
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

			Array<integer> idSet = asLinearizedArray<integer>(inputSet[IdSet]);

			const integer points = idSet.size();
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
			const integer bucketSize = asScalar<integer>(inputSet[BucketSize]);

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

			integer *outNodes = createScalar<integer>(outputSet[Nodes]);
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

			integer* outLeaves = createScalar<integer>(outputSet[Leaves]);
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

			integer* outDimension = createScalar<integer>(
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

			integer* outPoints = createScalar<integer>(
				outputSet[Points]);
			*outPoints = state->tree.points();
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
				Inputs
			};

			enum
			{
				IdSet,
				Outputs
			};
			
			ENSURE_OP(inputs, ==, Inputs);
			ENSURE_OP(outputs, ==, Outputs);

			// nearestIdSet = matlab_pointkdtree('pointkdtree_search_nearest', ...
			//		kdtree, querySet, maxDistanceSet, kNearest);

			KdState* state = asState(inputSet[State]);
			const IndexMap& indexMap = state->indexMap;
			Array<real> maxDistanceSet = asLinearizedArray<real>(inputSet[MaxDistanceSet]);
			const integer k = asScalar<integer>(inputSet[KNearest]);

			Array<Point_ConstIterator> nearestArray;
			integer queries = 0;

			mxClassID id = mxGetClassID(inputSet[QuerySet]);
			if (id == mxSINGLE_CLASS ||
				id == mxDOUBLE_CLASS)
			{
				// The queries are a set of points given explicitly
				// by their coordinates. Each column is a query point.
				Array<real> querySet = asArray<real>(inputSet[QuerySet]);
				queries = querySet.width();
				integer n = state->tree.n();

				nearestArray.setExtent(
					Vector2i(k, queries),
					state->tree.end());

				// Find the k-nearest-neighbors for each point.
				Vector<real> query(ofDimension(n));
				for (integer i = 0;i < queries;++i)
				{
					std::copy(
						querySet.cColumnBegin(i),
						querySet.cColumnEnd(i),
						query.begin());

					searchNearest(
						state->tree,
						query, k,
						rangeReporter(nearestArray.rowRange(i)),
						nullReporter(),
						maxDistanceSet(i));
				}
			}
			else
			{
				// The queries are over the points in the kd-tree,
				// given by their ids.
				Array<integer> querySet = asLinearizedArray<integer>(inputSet[QuerySet]);
				queries = querySet.size();

				nearestArray.setExtent(
					Vector2i(k, queries),
					state->tree.end());

				// Find the iterators corresponding to the
				// point-ids.
				std::vector<Point_ConstIterator> queryIterSet;
				queryIterSet.reserve(queries);
				for (integer i = 0;i < queries;++i)
				{			
					const ConstIterator iter = 
						indexMap.find(querySet(i));
					if (iter != indexMap.end())
					{
						queryIterSet.push_back(iter->second);
					}
				}
			
				// Find the k-nearest-neighbors for each point.
				searchAllNeighbors(
					state->tree, 
					range(queryIterSet.begin(), queryIterSet.end()),
					0, k,
					&nearestArray,
					(Array<real>*)0,
					maxDistanceSet.range());
			}

			Array<integer> result =
				createArray<integer>(k, queries, outputSet[IdSet]);
			for (integer x = 0;x < nearestArray.width();++x)
			{
				for (integer y = 0;y < nearestArray.height();++y)
				{
					Point_ConstIterator iter = nearestArray(x, y);
					if (iter != state->tree.end())
					{
						result(x, y) = iter->point().id;
					}
					else
					{
						result(x, y) = -1;
					}
				}			
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
		}

		CallFunction run(addStuff);
	
	}

}
