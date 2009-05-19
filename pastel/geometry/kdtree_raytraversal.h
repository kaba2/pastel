#ifndef PASTEL_KDTREE_RAYTRAVERSAL_H
#define PASTEL_KDTREE_RAYTRAVERSAL_H

#include "pastel/geometry/kdtree.h"
#include "pastel/geometry/line.h"

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
			const Line<N, Real>& ray, 
			Real& t) const;
	};
	*/

	template <
		int N,
		typename Real,
		typename ObjectPolicy,
		typename Intersector>
	typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator
		recursiveRayTraversal(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Line<N, Real>& ray,
		Intersector intersector);

}

#include "pastel/geometry/kdtree_raytraversal.hpp"

#endif
