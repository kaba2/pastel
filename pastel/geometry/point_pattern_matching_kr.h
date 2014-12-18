// Description: Point pattern matching under translations
// Documentation: point_pattern_matching_kr.txt

#ifndef PASTELGEOMETRY_POINT_PATTERN_MATCHING_KR_H
#define PASTELGEOMETRY_POINT_PATTERN_MATCHING_KR_H

#include "pastel/sys/output_concept.h"
#include "pastel/sys/vector.h"

#include "pastel/geometry/pointkdtree/pointkdtree.h"

namespace Pastel
{

	enum class MatchingMode : integer
	{
		FirstMatch,
		MaximumMatch
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
	template <
		typename Model_Settings, template <typename> class Model_Customization,
		typename Scene_Settings, template <typename> class Scene_Customization,
		typename NormBijection,
		typename Scene_Model_Output,
		typename Locator = typename Scene_Settings::Locator,
		typename Real = typename Locator::Real,
		integer N = Locator::N>
	Result_PointPatternMatchKr<Real, N> pointPatternMatchKr(
		const PointKdTree<Model_Settings, Model_Customization>& modelTree,
		const PointKdTree<Scene_Settings, Scene_Customization>& sceneTree,
		integer kNearest,
		const NoDeduction<Real>& minMatchRatio,
		const NoDeduction<Real>& matchingDistance,
		const NoDeduction<Real>& maxBias,
		MatchingMode matchingMode,
		const NormBijection& normBijection,
		Scene_Model_Output report);

}

#include "pastel/geometry/point_pattern_matching_kr.hpp"

#endif
