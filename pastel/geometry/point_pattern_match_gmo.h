// Description: Point pattern matching under translations
// Documentation: point_pattern_matching.txt

#ifndef PASTEL_POINT_PATTERN_MATCH_GMO_H
#define PASTEL_POINT_PATTERN_MATCH_GMO_H

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

	template <typename Real, int N, typename Model_PointPolicy, 
		typename Scene_PointPolicy, typename Scene_Model_Reporter,
		typename NormBijection>
	bool pointPatternMatchGmo(
		const PointKdTree<Real, N, Model_PointPolicy>& modelTree,
		const PointKdTree<Real, N, Scene_PointPolicy>& sceneTree,
		const PASTEL_NO_DEDUCTION(Real)& minMatchRatio,
		const PASTEL_NO_DEDUCTION(Real)& matchingDistance,
		const PASTEL_NO_DEDUCTION(Real)& maxBias,
		MatchingMode::Enum matchingMode,
		const NormBijection& normBijection,
		Vector<Real, N>& translation,
		PASTEL_NO_DEDUCTION(Real)& bias,
		Scene_Model_Reporter report);

}

#include "pastel/geometry/point_pattern_match_gmo.hpp"

#endif
