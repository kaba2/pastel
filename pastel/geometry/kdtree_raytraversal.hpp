#ifndef PASTEL_KDTREE_RAYTRAVERSAL_HPP
#define PASTEL_KDTREE_RAYTRAVERSAL_HPP

#include "pastel/geometry/kdtree_raytraversal.h"

#include <vector>

namespace Pastel
{

	namespace Detail
	{

		template <
			int N, typename Real,
			typename ObjectPolicy>
		class RecursiveRayTraversal
		{
		private:
			typedef KdTree<N, Real, ObjectPolicy> Tree;
			typedef typename Tree::Cursor Cursor;
			typedef typename Tree::ConstObjectIterator ConstObjectIterator;

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
				const KdTree<N, Real, ObjectPolicy>& tree,
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

				Real tClosest = infinity<Real>();
				ConstObjectIterator iterClosest = tree.end();

				while(!stack.empty())
				{
					StackElement stackElement = stack.back();
					stack.pop_back();

					Cursor currentNode = stackElement.node_;
					Real tMin = stackElement.tMin_;
					Real tMax = stackElement.tMax_;
					Point<Real, N> minPosition = ray.position() + tMin * ray.direction();
					Point<Real, N> maxPosition = ray.position() + tMax * ray.direction();

					while(!currentNode.leaf())
					{
						const Real splitPosition = currentNode.splitPosition();
						const integer splitAxis = currentNode.splitAxis();

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

						ASSERT(t != infinity<Real>());
						ASSERT(t != -infinity<Real>());

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

	template <
		int N,
		typename Real,
		typename ObjectPolicy,
		typename Intersector>
		typename KdTree<N, Real, ObjectPolicy>::ConstObjectIterator
		recursiveRayTraversal(
		const KdTree<N, Real, ObjectPolicy>& tree,
		const Line<Real, N>& ray,
		Intersector intersector)
	{
		static Detail::RecursiveRayTraversal<N, Real, ObjectPolicy> rayTraversal;
		return rayTraversal.work(tree, ray, intersector);
	}

}

#endif
