// Description: Point-pattern matching between point-sets

#ifndef PASTELGEOMETRY_POINT_PATTERN_MATCHING_VW_H
#define PASTELGEOMETRY_POINT_PATTERN_MATCHING_VW_H

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
		typename Point_ConstRange, typename Locator>
	Sphere<typename Locator::Real, Locator::N> 
		relativeMatchingDistance(
		const Point_ConstRange& pointSet,
		const Locator& locator);

	template <
		typename Scene_Settings, template <typename> class Scene_Customization,
		typename Model_Settings, template <typename> class Model_Customization,
		typename Real = typename Scene_Settings::Real>
	bool pointPatternMatch(
		const PointKdTree<Scene_Settings, Scene_Customization>& sceneTree,
		const PointKdTree<Model_Settings, Model_Customization>& modelTree,
		const PASTEL_NO_DEDUCTION(Real)& minMatchRatio,
		const PASTEL_NO_DEDUCTION(Real)& relativeMatchingDistance,
		const PASTEL_NO_DEDUCTION(Real)& confidence,
		ConformalAffine2D<Real>& similarityResult);

	//! Finds the given model point pattern from the scene point pattern.
	/*!
	This is a convenience function which builds kd-trees from the point sets 
	and calls the more general 'pointPatternMatch()'.
	*/

	template <
		typename Real, typename SceneRange, typename ModelRange,
		typename Scene_Locator,
		typename Model_Locator>
	bool pointPatternMatch(
		const SceneRange& scene,
		const ModelRange& model,
		const PASTEL_NO_DEDUCTION(Real)& minMatchRatio,
		const PASTEL_NO_DEDUCTION(Real)& relativeMatchingDistance,
		const PASTEL_NO_DEDUCTION(Real)& confidence,
		ConformalAffine2D<Real>& similarityResult,
		const Scene_Locator& sceneLocator,
		const Model_Locator& modelLocator);

	//! Finds the given model point pattern from the scene point pattern.
	/*!
	This is a convenience function which calls:
	pointPatternMatch(
		scene, model,
		minMatchRatio, relativeMatchingDistance,
		confidence,
		similarityResult,
		Vector_Locator<Real, N>(),
		Vector_Locator<Real, N>());
	*/

	template <typename Real, typename SceneRange, typename ModelRange>
	bool pointPatternMatch(
		const SceneRange& scene,
		const ModelRange& model,
		const PASTEL_NO_DEDUCTION(Real)& minMatchRatio,
		const PASTEL_NO_DEDUCTION(Real)& relativeMatchingDistance,
		const PASTEL_NO_DEDUCTION(Real)& confidence,
		ConformalAffine2D<Real>& similarityResult);

}

#include "pastel/geometry/point_pattern_matching_vw.hpp"

#endif
