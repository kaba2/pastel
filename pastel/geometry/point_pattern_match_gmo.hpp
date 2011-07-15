#ifndef PASTEL_POINT_PATTERN_MATCH_GMO_HPP
#define PASTEL_POINT_PATTERN_MATCH_GMO_HPP

#include "pastel/geometry/point_pattern_match_gmo.h"
#include "pastel/geometry/search_nearest_one_pointkdtree.h"

#include "pastel/sys/countingiterator.h"

#include <map>

namespace Pastel
{

	namespace Detail_PointPatternMatch
	{

		template <typename Real, int N, typename Model_PointPolicy, 
			typename Scene_PointPolicy, typename SceneModel_Iterator>
		class PointPatternGmo
		{
		public:
			typedef PointKdTree<Real, N, Model_PointPolicy> ModelTree;
			typedef typename ModelTree::Point_ConstIterator 
				ConstModelIterator;
			typedef typename ModelTree::Point ModelPoint;

			typedef PointKdTree<Real, N, Scene_PointPolicy> SceneTree;
			typedef typename SceneTree::Point_ConstIterator 
				ConstSceneIterator;
			typedef typename SceneTree::Point ScenePoint;

			typedef std::map<ConstSceneIterator, ConstModelIterator> 
				PairSet;
			typedef typename PairSet::iterator Pair_Iterator;
			typedef typename PairSet::const_iterator Pair_ConstIterator;

			template <typename Type>
			class Unique_AcceptPoint
			{
			public:
				typedef Type Point;

				explicit Unique_AcceptPoint(
					const PairSet& pairSet)
					: pairSet_(pairSet)
				{
				}

				bool operator()(
					const Point& that) const
				{
					return pairSet_.find(that) == pairSet_.end();
				}

			private:
				const PairSet& pairSet_;
			};

			bool match(
				const PointKdTree<Real, N, Model_PointPolicy>& modelTree,
				const PointKdTree<Real, N, Scene_PointPolicy>& sceneTree,
				const Real& minMatchRatio,
				const Real& matchingDistance,
				Vector<Real, N>& translation,
				SceneModel_Iterator output)
			{
				// It always holds that modelTree.size() <= sceneTree.size().						

				std::vector<ConstModelIterator> modelSet(
					countingIterator(modelTree.begin()),
					countingIterator(modelTree.end()));
				std::random_shuffle(modelSet.begin(), modelSet.end());

				Vector<Real, N> searchPoint(ofDimension(modelTree.dimension()));
				Vector<Real, N> tryTranslation(ofDimension(modelTree.dimension()));
				// We allow relative error for the nearest neighbor,
				// since this is an approximate algorithm anyway.
				// This gives us some performance boost.
				const Real maxRelativeError = 1;

				const integer minMatches = 
					std::min((integer)std::ceil(minMatchRatio * modelSet.size()),
					(integer)modelSet.size());

				const integer n = modelSet.size();
				for (integer i = 0;i < n;++i)
				{
					// Pick a model pivot point.
					const ConstModelIterator modelPivotIter = modelSet[i];

					// Go over all scene pivot points.
					ConstSceneIterator scenePivotIter = sceneTree.begin();
					const ConstSceneIterator scenePivotEnd = sceneTree.end();
					while(scenePivotIter != scenePivotEnd)
					{
						tryTranslation = 
							sceneTree.pointPolicy()(
							scenePivotIter->point()) -
							modelTree.pointPolicy()(
							modelPivotIter->point());

						// Find out how many points match
						// under this translation.

						PairSet pairSet;
						for (integer j = 0;j < modelSet.size();++j)
						{
							const ConstModelIterator modelIter = modelSet[j];
							
							searchPoint = modelTree.pointPolicy()(
								modelIter->point()) + 
								tryTranslation;

							const KeyValue<Real, ConstSceneIterator> neighbor = 
								searchNearestOne(
								sceneTree, 
								searchPoint,
								matchingDistance,
								maxRelativeError,
								Unique_AcceptPoint<ConstSceneIterator>(pairSet));

							//log() << "Distance " << neighbor.key() << logNewLine;
							
							if (neighbor.value() != sceneTree.end())
							{
								pairSet.insert(std::make_pair(
									neighbor.value(), modelIter));
							}
						}

						//log() << pairSet.size() << " ";

						if (pairSet.size() >= minMatches)
						{
							// We have found a matching translation.
							std::copy(
								pairSet.begin(), pairSet.end(),
								output);
							translation = tryTranslation;
							return true;
						}

						++scenePivotIter;
					}
				}

				return false;
			}
		};
	
	}

	template <typename Real, int N, typename Model_PointPolicy, 
		typename Scene_PointPolicy, typename SceneModel_Iterator>
	bool pointPatternMatchGmo(
		const PointKdTree<Real, N, Model_PointPolicy>& modelTree,
		const PointKdTree<Real, N, Scene_PointPolicy>& sceneTree,
		const PASTEL_NO_DEDUCTION(Real)& minMatchRatio,
		const PASTEL_NO_DEDUCTION(Real)& matchingDistance,
		Vector<Real, N>& translation,
		SceneModel_Iterator output)
	{
		ENSURE_OP(minMatchRatio, >=, 0);
		ENSURE_OP(minMatchRatio, <=, 1);
		ENSURE_OP(matchingDistance, >=, 0);

		Detail_PointPatternMatch::PointPatternGmo<
			Real, N, Model_PointPolicy, 
			Scene_PointPolicy, SceneModel_Iterator> 
			pointPattern;

		return pointPattern.match(
			modelTree, sceneTree, 
			minMatchRatio,
			matchingDistance,
			translation, output);
	}

}

#endif
