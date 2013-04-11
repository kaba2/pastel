// Description: Point pattern matching under translations
// Documentation: point_pattern_matching_kr.txt

#ifndef PASTELGEOMETRY_POINT_PATTERN_MATCHING_KR_H
#define PASTELGEOMETRY_POINT_PATTERN_MATCHING_KR_H

#include "pastel/sys/reporter_concept.h"
#include "pastel/sys/vector.h"

#include "pastel/geometry/pointkdtree.h"

namespace Pastel
{

	class MatchingMode
	{
	public:
		enum Enum
		{
			FirstMatch,
			MaximumMatch
		};
	};

	template <typename Real, int N>
	struct Result_PointPatternMatchKr
	{
		// TODO: Remove after uniform initialization 
		// becomes available in Visual Studio 2012.
		Result_PointPatternMatchKr(
			bool success_,
			Vector<Real, N> translation_,
			Real bias_)
			: success(success_)
			, translation(translation_)
			, bias(bias_)
		{
		}

		bool success;
		Vector<Real, N> translation;
		Real bias;
	};

	//! Find a matching translation between unpaired point-sets.
	/*!
	Preconditions:
	kNearest > 0
	minMatchRatio >= 0
	maxBias >= 0
	maxBias <= 1
	matchingDistance >= 0

	modelTree:
	The point-set to search for.

	sceneTree:
	The point-set from which to search for.

	kNearest:
	The number of nearest neighbors to use for disambiguation
	in case a point has multiple candidate pairs in its 
	matching distance.

	minMatchRatio:
	The minimum ratio of model-points that need to be 
	paired scene-points to accept a match.

	matchingDistance:
	The maximum distance between a point in the model-set
	and a point in the scene-set to accept them as a pair.
	Measured in terms of the actual norm, not in terms of 
	the norm-bijection.

	maxBias:
	The maximum bias which to accept from a match.
	Matches with larger bias will be ignored.

	matchingMode:
	MatchingMode::FirstMatch: Accept the first match.
	MatchingMode::BestMatch: Search for the best match.

	normBijection:
	The distance measure.

	report:
	The point-pairs are reported in the form
	std::make_pair(modelIter, sceneIter).
	*/
	template <typename Real, int N, typename Model_PointPolicy, 
		typename Scene_PointPolicy, typename Scene_Model_Reporter,
		typename NormBijection>
	Result_PointPatternMatchKr<Real, N> pointPatternMatchKr(
		const PointKdTree<Real, N, Model_PointPolicy>& modelTree,
		const PointKdTree<Real, N, Scene_PointPolicy>& sceneTree,
		integer kNearest,
		const PASTEL_NO_DEDUCTION(Real)& minMatchRatio,
		const PASTEL_NO_DEDUCTION(Real)& matchingDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxBias,
		MatchingMode::Enum matchingMode,
		const NormBijection& normBijection,
		Scene_Model_Reporter report);

}

#include "pastel/geometry/point_pattern_matching_kr.hpp"

#endif
