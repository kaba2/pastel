// Description: Vector-pattern matching between point-sets

#ifndef PASTEL_POINT_PATTERN_MATCHING_H
#define PASTEL_POINT_PATTERN_MATCHING_H

#include "pastel/sys/tuple.h"

#include "pastel/geometry/pointkdtree.h"

namespace Pastel
{

	class PatternMatch
	{
	public:
		enum Enum
		{
			AbsoluteDistance,
			RelativeDistance
		};
	};

	//! Finds the given model point pattern from the scene point pattern.
	/*!
	Preconditions:
	minMatchRatio is in [0, 1]
	If matchingDistanceType == PatternMatch::AbsoluteDistance, then
	matchingDistance >= 0
	If matchingDistanceType == PatternMatch::RelativeDistance, then
	matchingDistance is in [0, 1]

	matchinDistanceType:
	If 'matchingDistanceType' == PatternMatch::AbsoluteDistance, then
	the 'matchingDistance' directly represents the distance that
	is tolerated for points to match.
	If 'matchingDistanceType' == PatternMatch::RelativeDistance, then
	the absolute matching distance is determined from
	the average distance of a point to its nearest neighbour in the
	scene point set multiplied by 'matchingDistance'.

	More precisely, this functions finds a similarity transformation
	such that the model points are mapped to the scene points such
	that 'minMatchRatio'-ratio of the mapped model points have
	a scene point in their t-neighbourhood.

	*/

	template <typename Real, typename ScenePolicy, typename ModelPolicy>
	bool pointPatternMatch(
		const PointKdTree<Real, 2, ScenePolicy>& sceneTree,
		const PointKdTree<Real, 2, ModelPolicy>& modelTree,
		const PASTEL_NO_DEDUCTION(Real)& minMatchRatio,
		const PASTEL_NO_DEDUCTION(Real)& matchingDistance,
		const PatternMatch::Enum& matchingDistanceType,
		Tuple<Real, 4>& similarityResult);

	//! Finds the given model point pattern from the scene point pattern.
	/*!
	This is a convenience function for one-time use.
	It simply computes the kd-trees from the point sets and
	calls the more general 'pointPatternMatch()'.
	*/

	template <typename Real, typename SceneIterator, typename ModelIterator>
	bool pointPatternMatch(
		const SceneIterator& sceneBegin,
		const SceneIterator& sceneEnd,
		const ModelIterator& modelBegin,
		const ModelIterator& modelEnd,
		const PASTEL_NO_DEDUCTION(Real)& minMatchRatio,
		const PASTEL_NO_DEDUCTION(Real)& matchingDistance,
		const PatternMatch::Enum& matchingDistanceType,
		Tuple<Real, 4>& similarityResult);

}

#include "pastel/geometry/point_pattern_matching.hpp"

#endif
