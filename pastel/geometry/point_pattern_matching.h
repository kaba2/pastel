// Description: Point-pattern matching between point-sets

#ifndef PASTEL_POINT_PATTERN_MATCHING_H
#define PASTEL_POINT_PATTERN_MATCHING_H

#include "pastel/sys/tuple.h"

#include "pastel/geometry/pointkdtree.h"
#include "pastel/geometry/sphere.h"

#include "pastel/math/conformalaffine2d.h"

namespace Pastel
{

	//! Finds a given model point pattern from the scene point pattern.
	/*!
	Preconditions:
	0 <= minMatchRatio <= 1
	0 <= relativeMatchingDistance <= 1

	This function searches for such a similarity transformation that 
	'minMatchRatio'-ratio of the mapped model points have a unique scene 
	point in their t-neighborhood. The 't' is computed by
	t = relativeMatchingDistance * sceneSphere.radius() / (2 * sqrt(scenePoints)).
	*/

	template <typename Real, int N, 
		typename Point_ConstRange, typename PointPolicy>
	Sphere<typename PointPolicy::Real, PointPolicy::N> 
		relativeMatchingDistance(
		const Point_ConstRange& pointSet,
		const PointPolicy& pointPolicy);

	template <typename Real, int N, typename ScenePolicy, typename ModelPolicy>
	bool pointPatternMatch(
		const PointKdTree<Real, N, ScenePolicy>& sceneTree,
		const PointKdTree<Real, N, ModelPolicy>& modelTree,
		const PASTEL_NO_DEDUCTION(Real)& minMatchRatio,
		const PASTEL_NO_DEDUCTION(Real)& relativeMatchingDistance,
		const PASTEL_NO_DEDUCTION(Real)& confidence,
		ConformalAffine2D<Real, N>& similarityResult);

	//! Finds the given model point pattern from the scene point pattern.
	/*!
	This is a convenience function which builds kd-trees from the point sets 
	and calls the more general 'pointPatternMatch()'.
	*/

	template <typename Real, int N, typename SceneRange, typename ModelRange,
		typename Model_PointPolicy, typename Scene_PointPolicy>
	bool pointPatternMatch(
		const SceneRange& scene,
		const ModelRange& model,
		const PASTEL_NO_DEDUCTION(Real)& minMatchRatio,
		const PASTEL_NO_DEDUCTION(Real)& relativeMatchingDistance,
		const PASTEL_NO_DEDUCTION(Real)& confidence,
		ConformalAffine2D<Real, N>& similarityResult,
		const Model_PointPolicy& modelPointPolicy,
		const Scene_PointPolicy& scenePointPolicy);

	//! Finds the given model point pattern from the scene point pattern.
	/*!
	This is a convenience function which calls:
	pointPatternMatch(
		scene, model,
		minMatchRatio, relativeMatchingDistance,
		confidence,
		similarityResult,
		Vector_PointPolicy<Real, N>(),
		Vector_PointPolicy<Real, N>());
	*/

	template <typename Real, int N, typename SceneRange, typename ModelRange>
	bool pointPatternMatch(
		const SceneRange& scene,
		const ModelRange& model,
		const PASTEL_NO_DEDUCTION(Real)& minMatchRatio,
		const PASTEL_NO_DEDUCTION(Real)& relativeMatchingDistance,
		const PASTEL_NO_DEDUCTION(Real)& confidence,
		ConformalAffine2D<Real, N>& similarityResult);

}

#include "pastel/geometry/point_pattern_matching.hpp"

#endif
