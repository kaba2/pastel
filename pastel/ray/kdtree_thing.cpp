#include "pastel/ray/kdtree_thing.h"

#include "pastel/geometry/kdtree_raytraversal.h"
#include "pastel/geometry/kdtree_refine.h"
#include "pastel/geometry/kdtree_tools.h"

#include "pastel/geometry/bounding_alignedbox_alignedbox.h"

namespace Pastel
{

	KdTree_Thing::KdTree_Thing()
		: kdTree_()
	{
	}

	KdTree_Thing::~KdTree_Thing()
	{
	}

	AlignedBox3 KdTree_Thing::bound() const
	{
		return boundingAlignedBox(
			kdTree_.bound(),
			transformation().forward());
	}

	void KdTree_Thing::refine()
	{
		refineSurfaceAreaHeuristic(
			computeKdTreeMaxDepth(kdTree_.objects()),
			3,
			kdTree_);
	}

	// Private

	bool KdTree_Thing::intersect_(
		const Ray3& ray,
		ShapeIntersection& intersection) const
	{
		intersection.t = infinity<real>();

		const Intersector intersector(
			intersection);

		recursiveRayTraversal(
			kdTree_,
			ray.line(),
			intersector);

		return intersection.t < infinity<real>();
	}

}
