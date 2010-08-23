#ifndef PASTEL_POINT_PATTERN_MATCH_GMO_HPP
#define PASTEL_POINT_PATTERN_MATCH_GMO_HPP

#include "pastel/geometry/point_pattern_match_gmo.h"
#include "pastel/sys/pointpolicy_tools.h"
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
			typedef typename ModelTree::ConstObjectIterator 
				ConstModelIterator;
			typedef typename ModelTree::Object ModelPoint;

			typedef PointKdTree<Real, N, Scene_PointPolicy> SceneTree;
			typedef typename SceneTree::ConstObjectIterator 
				ConstSceneIterator;
			typedef typename SceneTree::Object ScenePoint;

			typedef std::map<ConstSceneIterator, ConstModelIterator> 
				PairSet;
			typedef typename PairSet::iterator Pair_Iterator;
			typedef typename PairSet::const_iterator Pair_ConstIterator;

			template <typename Type>
			class Unique_AcceptPoint
			{
			public:
				typedef Type Object;

				explicit Unique_AcceptPoint(
					const PairSet& pairSet)
					: pairSet_(pairSet)
				{
				}

				bool operator()(
					const Object& that) const
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
				const Real& confidence,
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

				const Real suggestedTries = 
					std::ceil(std::log(1 - confidence) / std::log(1 - minMatchRatio));

				integer tries = modelSet.size();
				if (suggestedTries >= 1 && suggestedTries < tries)
				{
					tries = suggestedTries;
				}

				for (integer i = 0;i < tries;++i)
				{
					// Pick a model pivot point.
					ConstModelIterator modelPivotIter = modelSet[i];

					// Go over all scene pivot points.
					ConstSceneIterator scenePivotIter = sceneTree.begin();
					const ConstSceneIterator scenePivotEnd = sceneTree.end();
					while(scenePivotIter != scenePivotEnd)
					{
						tryTranslation = 
							pointAsVector(
							scenePivotIter->object(),
							sceneTree.pointPolicy()) -
							pointAsVector(
							modelPivotIter->object(),
							modelTree.pointPolicy());

						// Find out how many points match
						// under this translation.

						PairSet pairSet;
						for (integer j = 0;j < modelSet.size();++j)
						{
							const ConstModelIterator modelIter = modelSet[j];
							
							searchPoint = pointAsVector(
								modelIter->object(),
								modelTree.pointPolicy()) + 
								tryTranslation;

							const KeyValue<Real, ConstSceneIterator> neighbor = 
								searchNearestOne(
								sceneTree, 
								searchPoint,
								matchingDistance,
								maxRelativeError,
								Unique_AcceptPoint<ConstSceneIterator>(pairSet));
							
							if (neighbor.value() != sceneTree.end())
							{
								pairSet.insert(std::make_pair(
									neighbor.value(), modelIter));
							}
						}

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
		const PASTEL_NO_DEDUCTION(Real)& confidence,
		Vector<Real, N>& translation,
		SceneModel_Iterator output)
	{
		ENSURE_OP(minMatchRatio, >=, 0);
		ENSURE_OP(minMatchRatio, <=, 1);
		ENSURE_OP(matchingDistance, >=, 0);
		ENSURE_OP(confidence, >=, 0);
		ENSURE_OP(confidence, <=, 1);

		Detail_PointPatternMatch::PointPatternGmo<
			Real, N, Model_PointPolicy, 
			Scene_PointPolicy, SceneModel_Iterator> 
			pointPattern;

		return pointPattern.match(
			modelTree, sceneTree, minMatchRatio,
			matchingDistance, confidence,
			translation, output);
	}

}

#endif
