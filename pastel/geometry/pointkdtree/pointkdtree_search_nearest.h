// Description: Nearest neighbor searching for PointKdTree
// Documentation: nearest_neighbors.txt

#ifndef PASTELGEOMETRY_POINTKDTREE_SEARCH_NEAREST_H
#define PASTELGEOMETRY_POINTKDTREE_SEARCH_NEAREST_H

#include "pastel/math/normbijection/normbijection_concept.h"
#include "pastel/math/normbijection/euclidean_normbijection.h"

#include "pastel/sys/output/output_concept.h"
#include "pastel/sys/indicator/indicator_concept.h"
#include "pastel/sys/indicator/all_indicator.h"
#include "pastel/sys/type_traits/is_template_instance.h"

#include "pastel/geometry/pointkdtree/pointkdtree_fwd.h"
#include "pastel/geometry/tdtree/tdtree_fwd.h"
#include "pastel/geometry/depthfirst_pointkdtree_searchalgorithm.h"
#include "pastel/geometry/pointkdtree/pointkdtree_search_nearest.hpp"

namespace Pastel
{

	template <typename KdTree>
	using KdTree_Locator =
		typename std::remove_reference_t<KdTree>::Locator;

	//! Finds the nearest neighbors of a point in a PointKdTree.
	/*!
	kdTree:
	The kd-tree to search neighbors in. 
	Either a PointKdTree or a TdTree.

	searchPoint:
	The point for which to search a neighbor for.
	This can be either a Vector<Real, N>, or
	a Point_ConstIterator of 'kdTree'.

	timeIntervalSequence:
	An interval sequence in time. A sequence 
	(t_1, t_2, t_3, t_4, ...) corresponds to the
	time-intervals [t_1, t_2), [t_3, t_4), ...
	If the number of time-instants is odd, then
	the sequence is implicitly appended 
	infinity<Real>().

	nearestOutput:
	A reporter to which the found neighbors 
	(KdTree::Point_ConstIterator) are reported to.
	The reporting is done in the form
	nearestOutput(distance, point).

	acceptPoint:
	An indicator which decides whether to accept a point 
	(KdTree::Point_ConstIterator) as a neighbor or not.

	normBijection:
	The norm used to measure distance.

	searchAlgorithm:
	The search algorithm to use for searching the 'kdTree'.
	See 'pointkdtree_searchalgorithm.txt'.

	Optional arguments
	------------------

	maxDistance (Real >= 0):
	The distance after which points are not considered neighbors
	anymore. Can be set to infinity<Real>(). This distance
	is in terms of the used norm bijection.
	Default: infinity<Real>()

	maxRelativeError (Real >= 0):
	Maximum allowed relative error in the distance of the  
	result point to the true nearest neighbor. Allowing error
	increases performance. Use 0 for exact matches. 
	Default: 0

	bucketSize (integer > 0):
	The number of points under which to start a brute-force
	search in a node.
	Default: 16

	kNearest (integer > 0):
	The number of nearest neighbors to search.

	Returns (by implicit conversion)
	--------------------------------

	Real:
	The distance (in terms of the norm bijection) to 
	the k:th nearest neighbor. If the k:th nearest 
	neighbor does not exist, infinity<Real>().

	Point_ConstIterator:
	The k:th nearest neighbor. If the k:th nearest 
	neighbor does not exist, kdTree.end().

	std::pair<Real, Point_ConstIterator>:
	A combination of the previous two.
	*/
	template <
		typename KdTree,
		typename SearchPoint,
		typename ConceptCheck = 
			Requires<
				// Visual Studio 2015 RC has bugs which cause these
				// commented stuff to fail.
				//Models<NormBijection, NormBijection_Concept>,
				Or<
					IsPointKdTree<KdTree>,
					IsTdTree<KdTree>
				>
				//Models<NearestOutput, Output_Concept(?)>,
				//Models<Indicator, Indicator_Concept(typename KdTree::Point_ConstIterator)>
			>,
		typename NearestOutput = Null_Output,
		typename Indicator = All_Indicator,
		typename Locator = typename KdTree::Locator,
		typename Real = Locator_Real<Locator>,
		typename NormBijection = Euclidean_NormBijection<Real>, 
		typename SearchAlgorithm = DepthFirst_SearchAlgorithm_PointKdTree,
		typename IntervalSequence = Vector<Real, 2>
		>
	auto searchNearest(
		const KdTree& kdTree,
		const SearchPoint& searchPoint,
		const NearestOutput& nearestOutput = NearestOutput(),
		const Indicator& acceptPoint = Indicator(),
		const NormBijection& normBijection = NormBijection(),
		const SearchAlgorithm& searchAlgorithm = SearchAlgorithm(),
		const IntervalSequence& timeIntervalSequence = IntervalSequence({-infinity<Real>(), infinity<Real>()}))
		-> SearchNearest_<KdTree, SearchPoint, NearestOutput, 
		Indicator, NormBijection, SearchAlgorithm, IntervalSequence>
	{
		return 
			{
				kdTree, 
				searchPoint,
				nearestOutput, 
				acceptPoint, 
				normBijection, 
				searchAlgorithm,
				timeIntervalSequence
			};
	}

}

#include "pastel/geometry/pointkdtree/pointkdtree_search_nearest.hpp"

#endif
