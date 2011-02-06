#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/search_all_neighbors_pointkdtree.h"
#include "pastel/geometry/search_all_neighbors_bruteforce.h"

#include "pastel/matlab/matlab.h"

#include "pastel/sys/pointpolicy_all.h"
#include "pastel/sys/unorderedmap.h"
#include "pastel/sys/poolallocator.h"

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

			typedef RandomAccessIterator_Range<ConstIterator>
				ConstRange;

			typedef Array_VectorExpression<real, Dynamic>
				Expression;

			const Real& axis(const Point& point, integer i) const
			{
				return *(point.data + i);
			}

			Expression operator()(const Point& point) const
			{
				return Expression(begin(point), dimension());
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

			integer dimension() const
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
		typedef UnorderedMap<integer, Point_ConstIterator> IndexMap;
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

			~KdState()
			{
				pointAllocator.clear();
			}

			typedef std::vector<TreePoint> PointSet;

			Tree tree;
			IndexMap indexMap;
			PoolAllocator pointAllocator;
			PointSet pointSet;
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
			IntegerArrayPtr idSet = asLinearizedArray<integer>(inputSet[IdSet]);

			const integer d = state->tree.dimension();
			const integer n = idSet->size();

			typedef Tree::PointPolicy_ PointPolicy;
			const PointPolicy& pointPolicy = state->tree.pointPolicy();

			RealArrayPtr pointSet = createArray<real>(
				n, d, outputSet[PointSet]);
			for (integer i = 0;i < n;++i)
			{
				const integer id = (*idSet)(i);
				ConstIterator iter = state->indexMap.find(id);

				if (iter != state->indexMap.end())
				{
					Point_ConstIterator pointIter =
						iter->second;
					std::copy(
						pointPolicy.begin(pointIter->point()),
						pointPolicy.end(pointIter->point()),
						pointSet->columnBegin(i));
				}					
				else
				{
					std::fill(
						pointSet->columnBegin(i),
						pointSet->columnEnd(i),
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
			Tree* tree = new Tree(TreePoint_PointPolicy(dimension));
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
			const integer dimension = tree.dimension();
			const real* pointSet = mxGetPr(inputSet[PointSet]);
			const integer elements = mxGetNumberOfElements(inputSet[PointSet]);
			const integer points = elements / tree.dimension();

			//printf("Dimension %d\n", dimension);
			//printf("Elements %d\n", elements);
			//printf("Points %d\n", points);

			IntegerArrayPtr result =
				createArray<integer>(points, 1, outputSet[IdSet]);
			for (integer i = 0;i < points;++i)
			{
				real* data = (real*)state->pointAllocator.allocate();
				std::copy(pointSet, pointSet + dimension, data);
				pointSet += dimension;

				/*
				for (integer j = 0;j < dimension;++j)
				{
					printf("%f, ", data[j]);
				}
				printf("\n");
				*/

				const integer index = state->index;
				state->pointSet.push_back(TreePoint(data, index));
				Point_ConstIterator iter = tree.insert(TreePoint(data, index));
				state->indexMap.insert(std::make_pair(index, iter));
				++state->index;

				(*result)(i) = index;
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
				SlidingMidpoint_SplitRule_PointKdTree(), bucketSize);

			state->tree.erase();
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
			*outDimension = state->tree.dimension();
		}

		void kdObjects(
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

			// nearestIdSet = matlab_pointkdtree('pointkdtree_search_nearest', kdtree, idSet, Inf, k);
			KdState* state = asState(inputSet[State]);
			const IndexMap& indexMap = state->indexMap;
			IntegerArrayPtr querySet = asArray<integer>(inputSet[QuerySet]);
			const integer queries = querySet->size();
			const integer k = asScalar<integer>(inputSet[KNearest]);

			std::vector<Point_ConstIterator> queryIterSet;
			queryIterSet.reserve(queries);
			for (integer i = 0;i < queries;++i)
			{			
				const ConstIterator iter = 
					indexMap.find((*querySet)(i));
				if (iter != indexMap.end())
				{
					queryIterSet.push_back(iter->second);
				}
			}
			
			Array<Point_ConstIterator> nearestArray(k, queries);
			searchAllNeighbors(
				state->tree, 
				range(queryIterSet.begin(), queryIterSet.end()),
				0, k,
				&nearestArray,
				0,
				range(mxGetPr(inputSet[MaxDistanceSet]), queries));

			IntegerArrayPtr result =
				createArray<integer>(queries, k, outputSet[IdSet]);
			for (integer x = 0;x < nearestArray.width();++x)
			{
				for (integer y = 0;y < nearestArray.height();++y)
				{
					(*result)(x, y) = nearestArray(x, y)->point().id;
				}			
			}
		}

		void addStuff()
		{
			matlabAddFunction("pointkdtree_as_points", kdAsPoints);
			matlabAddFunction("pointkdtree_construct", kdConstruct);
			matlabAddFunction("pointkdtree_destruct", kdDestruct);
			matlabAddFunction("pointkdtree_objects", kdObjects);
			matlabAddFunction("pointkdtree_nodes", kdNodes);
			matlabAddFunction("pointkdtree_leaves", kdLeaves);
			matlabAddFunction("pointkdtree_dimension", kdDimension);
			matlabAddFunction("pointkdtree_insert", kdInsert);
			matlabAddFunction("pointkdtree_refine", kdRefine);
			matlabAddFunction("pointkdtree_search_nearest", kdSearchNearest);
		}

		CallFunction run(addStuff);
	
	}

}
