#ifndef PASTEL_KDTREE_THING_H
#define PASTEL_KDTREE_THING_H

#include "pastel/ray/composite_thing.h"

#include "pastel/geometry/kdtree.h"

#include "pastel/sys/range.h"

namespace Pastel
{

	class PASTELRAY KdTree_Thing
		: public Composite_Thing
	{
	public:
		KdTree_Thing();
		virtual ~KdTree_Thing();

		template <typename ThingPtr_Range>
		void insert(const ThingPtr_Range& thingSet);

		void refine();

		virtual AlignedBox3 bound() const;
	
	private:
		class ObjectPolicy
		{
		public:
			typedef ThingPtr Object;

			AlignedBox3 bound(const Object& object) const
			{
				return object->bound();
			}

			Real2 bound(const Object& object, integer axis) const
			{
				return Real2(
					object->bound().min()[axis], 
					object->bound().max()[axis]);
			}
		};
		
		class Intersector
		{
		public:
			explicit Intersector(
				ShapeIntersection& intersection)
				: tClosest_(infinity<real>())
				, intersection_(intersection)
			{
			}

			bool operator()(
				const ThingPtr& thing,
				const Line3& ray,
				real& t) const
			{
				ShapeIntersection intersection;
				if (!thing->intersect(
					Ray3(ray.position(), ray.direction()),
					intersection))
				{
					return false;
				}

				if (intersection.t < tClosest_)
				{
					intersection_ = intersection;
				}

				t = intersection.t;

				return true;
			}

			mutable real tClosest_;
			ShapeIntersection& intersection_;
		};

		virtual bool intersect_(
			const Ray3& ray,
			ShapeIntersection& intersection) const;
		
		KdTree<real, 3, ObjectPolicy> kdTree_;
	};

	typedef CountedPtr<KdTree_Thing> KdTree_ThingPtr;

}

#include "pastel/ray/kdtree_thing.hpp"

#endif
