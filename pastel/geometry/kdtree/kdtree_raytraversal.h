// Description: Ray-traversal algorithm for KdTree
// Documentation: kdtree.txt

#ifndef PASTELGEOMETRY_KDTREE_RAYTRAVERSAL_H
#define PASTELGEOMETRY_KDTREE_RAYTRAVERSAL_H

#include "pastel/geometry/kdtree/kdtree.h"
#include "pastel/geometry/shape/line.h"

#include <vector>

namespace Pastel
{

	namespace Detail
	{

		template <
			typename Real, int N,
			typename ObjectPolicy>
		class RecursiveRayTraversal
		{
		private:
			using Tree = KdTree<Real, N, ObjectPolicy>;
			using Cursor = typename Tree::Cursor;
			using ConstObjectIterator = typename Tree::ConstObjectIterator;

			class StackElement
			{
			public:
				StackElement()
					: node_()
					, tMin_(0)
					, tMax_(0)
				{
				}

				StackElement(
					const Cursor& node,
					const Real& tMin,
					const Real& tMax)
					: node_(node)
					, tMin_(tMin)
					, tMax_(tMax)
				{
				}

				Cursor node_;
				Real tMin_;
				Real tMax_;
			};

		public:

			template <typename Intersector>
			ConstObjectIterator work(
				const KdTree<Real, N, ObjectPolicy>& tree,
				const Line<Real, N>& ray,
				Intersector intersector)
			{
				Vector<Real, 2> tHit;
				if (!intersect(ray, tree.bound(), tHit) ||
					tHit[1] < 0)
				{
					return tree.end();
				}

				if (tHit[0] < 0)
				{
					tHit[0] = 0;
				}

				// We are doing a front-to-back depth-first traversal.
				// The traversal is
				// done iteratively rather than
				// recursively by using a stack.

				std::vector<StackElement> stack;
				stack.reserve(50);

				Cursor farChild;
				stack.push_back(StackElement(tree.root(), tHit[0], tHit[1]));

				Real tClosest = Infinity();
				ConstObjectIterator iterClosest = tree.end();

				while(!stack.empty())
				{
					StackElement stackElement = stack.back();
					stack.pop_back();

					Cursor currentNode = stackElement.node_;
					Real tMin = stackElement.tMin_;
					Real tMax = stackElement.tMax_;
					Vector<Real, N> minPosition = ray.at(tMin);
					Vector<Real, N> maxPosition = ray.at(tMax);

					while(!currentNode.leaf())
					{
						Real splitPosition = currentNode.splitPosition();
						integer splitAxis = currentNode.splitAxis();

						if (minPosition[splitAxis] <= splitPosition)
						{
							if (maxPosition[splitAxis] <= splitPosition)
							{
								// Only the negative child needs to be traversed.

								currentNode = currentNode.negative();
								continue;
							}
							// The thesis has a bug here, reading
							//if (stack[exitIndex].position_[splitAxis] == splitPosition)
							// Clearly it should be:
							if (minPosition[splitAxis] == splitPosition)
							{
								// Only the positive child needs to be traversed.

								currentNode = currentNode.positive();
								continue;
							}

							// Both children need to be traversed,
							// first the negative, then the positive.

							farChild = currentNode.positive();
							currentNode = currentNode.negative();
						}
						else
						{
							if (maxPosition[splitAxis] > splitPosition)
							{
								// Only the positive child needs to be traversed.

								currentNode = currentNode.positive();
								continue;
							}

							// Both children need to be traversed,
							// first the positive, then the negative.

							farChild = currentNode.negative();
							currentNode = currentNode.positive();
						}

						// Traversing both children: the farther one
						// is put on the stack.
						// Note if both children are not traversed
						// then this code is skipped by the continues.

						ASSERT(ray.direction()[splitAxis] != 0);

						//const Real t = (splitPosition - ray.position()[splitAxis]) /
						//	ray.direction()[splitAxis];

						const Real t = (splitPosition - ray.position()[splitAxis]) *
							ray.inverseDirection()[splitAxis];

						ASSERT(t != (Real)Infinity());
						ASSERT(t != -(Real)Infinity());

						stack.push_back(StackElement(farChild, t, tMax));

						tMax = t;
						maxPosition = ray.position() + tMax * ray.direction();
						maxPosition[splitAxis] = splitPosition;
					}

					// We are now in a leaf node.

					//ASSERT(tMin < tMax);

					ConstObjectIterator iter = currentNode.begin();
					ConstObjectIterator iterEnd = currentNode.end();
					while(iter != iterEnd)
					{
						Real t = 0;
						if (intersector(*iter, ray, t))
						{
							// Only accept intersection if it
							// it is contained in this leaf node
							// and it is closer than the previous
							// intersection.

							if (t < tClosest &&
								t >= tMin && t <= tMax)
							{
								tClosest = t;
								iterClosest = iter;
							}
						}

						++iter;
					}

					if (iterClosest != tree.end())
					{
						return iterClosest;
					}
				}

				return tree.end();
			}
		};

	}

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
		Intersector intersector)
	{
		static Detail::RecursiveRayTraversal<Real, N, ObjectPolicy> rayTraversal;
		return rayTraversal.work(tree, ray, intersector);
	}

}

#endif
