// Description: Point-pattern matching between point-sets

#ifndef PASTEL_POINT_PATTERN_MATCHING_H
#define PASTEL_POINT_PATTERN_MATCHING_H

#include "pastel/sys/tuple.h"

#include "pastel/geometry/pointkdtree.h"

#include "pastel/math/conformalaffine2d.h"

namespace Pastel
{

	//! Finds a given model point pattern from the scene point pattern.
	/*!
	Preconditions:
	0 <= minMatchRatio <= 1
	relativeMatchingDistance >= 0

	This function searches for such a similarity transformation that 
	'minMatchRatio'-ratio of the mapped model points have a unique scene 
	point in their t-neighborhood. The 't' is computed by
	t = relativeMatchingDistance * sceneSphere.radius() / (2 * sqrt(scenePoints)).
	*/

	template <typename Real, int N, typename ScenePolicy, typename ModelPolicy>
	bool pointPatternMatch(
		const PointKdTree<Real, N, ScenePolicy>& sceneTree,
		const PointKdTree<Real, N, ModelPolicy>& modelTree,
		const PASTEL_NO_DEDUCTION(Real)& minMatchRatio,
		const PASTEL_NO_DEDUCTION(Real)& relativeMatchingDistance,
		ConformalAffine2D<Real, N>& similarityResult);

	//! Finds the given model point pattern from the scene point pattern.
	/*!
	This is a convenience function for one-time use.
	It simply computes the kd-trees from the point sets and
	calls the more general 'pointPatternMatch()'.
	*/

	template <typename Real, int N, typename SceneIterator, typename ModelIterator>
	bool pointPatternMatch(
		const ForwardRange<SceneIterator>& scene,
		const ForwardRange<ModelIterator>& model,
		const PASTEL_NO_DEDUCTION(Real)& minMatchRatio,
		const PASTEL_NO_DEDUCTION(Real)& relativeMatchingDistance,
		ConformalAffine2D<Real, N>& similarityResult);

}

#include "pastel/geometry/point_pattern_matching.hpp"

#endif
