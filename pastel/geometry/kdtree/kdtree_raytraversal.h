// Description: Ray-traversal algorithm for KdTree
// Documentation: kdtree.txt

#ifndef PASTELGEOMETRY_KDTREE_RAYTRAVERSAL_H
#define PASTELGEOMETRY_KDTREE_RAYTRAVERSAL_H

#include "pastel/geometry/kdtree/kdtree.h"
#include "pastel/geometry/shape/line.h"

namespace Pastel
{

	//! Intersects objects in a kdtree with a ray.
	/*!
	class Intersector
	{
	public:
		// If 'ray' intersects 'object',
		// fill 't' with the intersection parameter
		// and return true. Otherwise return false.
		bool operator()(
			const Object& object,
			const Line<Real, N>& ray, 
			Real& t) const;
	};
	*/

	template <
		typename Real,
		int N,
		typename ObjectPolicy,
		typename Intersector>
	typename KdTree<Real, N, ObjectPolicy>::ConstObjectIterator
		recursiveRayTraversal(
		const KdTree<Real, N, ObjectPolicy>& tree,
		const Line<Real, N>& ray,
		Intersector intersector);

}

#include "pastel/geometry/kdtree/kdtree_raytraversal.hpp"

#endif
