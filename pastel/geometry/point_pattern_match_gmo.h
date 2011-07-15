// Description: Point pattern matching under translations
// Documentation: point_pattern_matching.txt

#ifndef PASTEL_POINT_PATTERN_MATCH_GMO_H
#define PASTEL_POINT_PATTERN_MATCH_GMO_H

#include "pastel/sys/vector.h"

#include "pastel/geometry/pointkdtree.h"

namespace Pastel
{

	template <typename Real, int N, typename Model_PointPolicy, 
		typename Scene_PointPolicy, typename SceneModel_Iterator>
	bool pointPatternMatchGmo(
		const PointKdTree<Real, N, Model_PointPolicy>& modelTree,
		const PointKdTree<Real, N, Scene_PointPolicy>& sceneTree,
		const PASTEL_NO_DEDUCTION(Real)& minMatchRatio,
		const PASTEL_NO_DEDUCTION(Real)& matchingDistance,
		Vector<Real, N>& translation,
		SceneModel_Iterator output);

}

#include "pastel/geometry/point_pattern_match_gmo.hpp"

#endif
