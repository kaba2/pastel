#ifndef PASTEL_SEARCH_ALL_NEIGHBORS_VAIDYA_HPP
#define PASTEL_SEARCH_ALL_NEIGHBORS_VAIDYA_HPP

#include "pastel/geometry/search_all_neighbors_vaidya.h"

#include "pastel/geometry/bounding_alignedbox.h"
#include "pastel/geometry/boxvolume.h"
#include "pastel/geometry/diameter.h"
#include "pastel/geometry/distance_alignedbox_alignedbox.h"
#include "pastel/geometry/alignedbox_tools.h"

#include "pastel/sys/countedptr.h"
#include "pastel/sys/unorderedset.h"

#include <map>
#include <functional>
#include <iostream>

namespace Pastel
{

	namespace Detail_AllNearestNeighborsVaidya
	{

		template <int N, typename Real>
		class AllNearestNeighborsVaidya
		{
		public:
			explicit AllNearestNeighborsVaidya(
				integer dimension,
				const std::vector<Point<N, Real> >& pointSet,
				integer kNearest,
				Array<2, integer>& nearestSet)
				: pointSet_(pointSet)
				, kNearest_(kNearest)
				, pointPartition_()
				, boxSet_()
				, nearestSet_(nearestSet)
				, singularSet_()
				, dimension_(dimension)
			{
			}

			void work();

		private:
			class Neighborhood;
			typedef UnorderedSet<CountedPtr<Neighborhood> > NeighborhoodSet;
			typedef typename NeighborhoodSet::iterator NeighborhoodIterator;

			class Neighborhood
				: public ReferenceCounted
			{
			public:
				explicit Neighborhood(
					const AlignedBox<N, Real>& bound,
					integer partitionBegin,
					integer partitionEnd)
					: neighborSet_()
					, attractorSet_()
					, estimate_(infinity<Real>())
					, bound_(bound)
					, partitionBegin_(partitionBegin)
					, partitionEnd_(partitionEnd)
				{
					ASSERT1(partitionBegin >= 0, partitionBegin);
					ASSERT2(partitionBegin < partitionEnd, partitionBegin, partitionEnd);
				}

				integer points() const
				{
					return partitionEnd_ - partitionBegin_;
				}

				NeighborhoodSet neighborSet_;
				NeighborhoodSet attractorSet_;
				Real estimate_;
				AlignedBox<N, Real> bound_;
				integer partitionBegin_;
				integer partitionEnd_;
			};

			typedef CountedPtr<Neighborhood> NeighborhoodPtr;

			void initialize();
			
			void splitBox(
				const AlignedBox<N, Real>& bound,
				integer axis,
				integer partitionBegin,
				integer partitionEnd,
				std::vector<NeighborhoodPtr>& childSet);
			
			void shrinkBox(
				const NeighborhoodPtr& box);
			
			void createEstimate(
				const NeighborhoodPtr& box);
			
			void updateEstimate(
				const NeighborhoodPtr& box,
				const std::vector<NeighborhoodPtr>& childSet);
			
			template <typename InputIterator>
			Real estimate(
				const NeighborhoodPtr& box,
				const InputIterator& begin,
				const InputIterator& end);
			
			template <typename InputIterator>
			void removeUnderEstimate(
				const InputIterator& iterBegin,
				const InputIterator& iterEnd);
			
			void extractNearest();

			const std::vector<Point<N, Real> >& pointSet_;
			integer kNearest_;
			std::vector<integer> pointPartition_;
			std::multimap<Real, NeighborhoodPtr, std::greater<Real> > boxSet_;
			Array<2, integer>& nearestSet_;
			std::vector<NeighborhoodPtr> singularSet_;
			integer dimension_;
		};

		template <int N, typename Real>
		void AllNearestNeighborsVaidya<N, Real>::work()
		{
			initialize();
		
			while(!boxSet_.empty())
			{
				// Pick the neighborhood with max volume.

				const NeighborhoodPtr box = boxSet_.begin()->second;
				boxSet_.erase(boxSet_.begin());

				ASSERT(box->points() > 1);
				//ASSERT1(volume(box->bound_) > 0, box->points());

				// Split it into non-empty hyper-octree subnodes.
				
				std::vector<NeighborhoodPtr> childSet;
				splitBox(box->bound_, 0, 
					box->partitionBegin_, box->partitionEnd_,
					childSet);

				// Shrink each subnode minimally around
				// points.
			
				const integer children = childSet.size();
				for (integer i = 0;i < children;++i)
				{
					const NeighborhoodPtr child = childSet[i];
					shrinkBox(child);
				}

				for (integer i = 0;i < children;++i)
				{
					const NeighborhoodPtr child = childSet[i];

					// Construct the neighbors of subnodes.

					child->neighborSet_ = box->neighborSet_;
					child->neighborSet_.insert(
						childSet.begin(), childSet.end());
					child->neighborSet_.erase(child);

					// Construct the attractors of subnodes.

					child->attractorSet_ = box->attractorSet_;
					child->attractorSet_.insert(
						childSet.begin(), childSet.end());
					child->attractorSet_.erase(child);

					// Update estimates of the subnodes.

					createEstimate(child);

					// Queue the subnodes for further
					// processing.

					const Real childVolume = volume(child->bound_);

					// We use the non-zero volume instead of
					// the number of points to determine the
					// need for further processing.
					// This is because there might
					// be multiple points at the same location,
					// which leads to infinite recursion.

					//if (child->points() > 1)
					if (childVolume > 0)
					{
						boxSet_.insert(std::make_pair(
							childVolume, child));
					}
					else
					{
						singularSet_.push_back(child);
					}
				}

				// Update the neighbors of attractors.
				{
					NeighborhoodIterator iter = box->attractorSet_.begin();
					const NeighborhoodIterator iterEnd = box->attractorSet_.end();
					while(iter != iterEnd)
					{
						const NeighborhoodPtr attractor = *iter;
						
						attractor->neighborSet_.erase(box);
						attractor->neighborSet_.insert(
							childSet.begin(), childSet.end());
						updateEstimate(attractor, childSet);

						++iter;
					}
				}

				// Update the attractors of neighbors.
				{
					NeighborhoodIterator iter = box->neighborSet_.begin();
					const NeighborhoodIterator iterEnd = box->neighborSet_.end();
					while(iter != iterEnd)
					{
						const NeighborhoodPtr neighbor = *iter;
						
						neighbor->attractorSet_.erase(box);
						neighbor->attractorSet_.insert(
							childSet.begin(), childSet.end());

						++iter;
					}
				}

				// Delete boxes that are under the estimates.
				{
					removeUnderEstimate(
						box->attractorSet_.begin(),
						box->attractorSet_.end());
					removeUnderEstimate(
						childSet.begin(),
						childSet.end());
				}

				box->neighborSet_.clear();
				box->attractorSet_.clear();
			}

			extractNearest();
		}

		template <int N, typename Real>
		void AllNearestNeighborsVaidya<N, Real>::initialize()
		{
			const AlignedBox<N, Real> cubeBound =
				boundingAlignedCube(
				boundingAlignedBox<N, Real>(pointSet_.begin(), pointSet_.end()));
			
			const integer points = pointSet_.size();

			boxSet_.insert(std::make_pair(
				volume(cubeBound), 
				NeighborhoodPtr(new Neighborhood(cubeBound, 0, points))));

			pointPartition_.reserve(points);
			for (integer i = 0;i < points;++i)
			{
				pointPartition_.push_back(i);
				ASSERT(allGreaterEqual(pointSet_[i], cubeBound.min()));
				ASSERT(allLessEqual(pointSet_[i], cubeBound.max()));
			}
		}

		template <int N, typename Real>
		void AllNearestNeighborsVaidya<N, Real>::splitBox(
			const AlignedBox<N, Real>& bound,
			integer axis,
			integer partitionBegin,
			integer partitionEnd,
			std::vector<NeighborhoodPtr>& childSet)
		{
			ASSERT2(axis >= 0 && axis < dimension_, axis, dimension_);
			ASSERT2(partitionBegin < partitionEnd, partitionBegin, partitionEnd);
			ASSERT1(partitionBegin >= 0, partitionBegin);
			ASSERT2(partitionEnd <= pointPartition_.size(), partitionEnd, pointPartition_.size());

			const Real center = linear(
				bound.min()[axis], 
				bound.max()[axis], 0.5);
			
			integer leftEnd = partitionBegin;
			for (integer i = partitionBegin;i < partitionEnd;++i)
			{
				const integer pointIndex = pointPartition_[i];
				const Real position = pointSet_[pointIndex][axis];

				ASSERT2(position >= bound.min()[axis], position, bound.min()[axis]);
				ASSERT2(position <= bound.max()[axis], position, bound.max()[axis]);

				if (position < center)
				{
					std::swap(pointPartition_[i], pointPartition_[leftEnd]);
					++leftEnd;
				}
			}

			if (leftEnd > partitionBegin)
			{
				AlignedBox<N, Real> leftBound = bound;
				leftBound.max()[axis] = center;
				
				if (axis < dimension_ - 1)
				{
					splitBox(leftBound, axis + 1, partitionBegin, leftEnd, childSet);
				}
				else
				{
					const NeighborhoodPtr leftChild = 
						NeighborhoodPtr(new Neighborhood(leftBound, partitionBegin, leftEnd));
					childSet.push_back(leftChild);
				}
			}
			if (leftEnd < partitionEnd)
			{
				AlignedBox<N, Real> rightBound = bound;
				rightBound.min()[axis] = center;

				if (axis < dimension_ - 1)
				{
					splitBox(rightBound, axis + 1, leftEnd, partitionEnd, childSet);
				}
				else
				{
					const NeighborhoodPtr rightChild = 
						NeighborhoodPtr(new Neighborhood(rightBound, leftEnd, partitionEnd));
					childSet.push_back(rightChild);
				}
			}
		}

		template <int N, typename Real>
		void AllNearestNeighborsVaidya<N, Real>::shrinkBox(
			const NeighborhoodPtr& box)
		{
			ASSERT(box->points() > 0);

			const integer partitionBegin = box->partitionBegin_;
			const integer partitionEnd = box->partitionEnd_;
			
			Point<N, Real> minPoint(infinity<Real>());
			Point<N, Real> maxPoint(-infinity<Real>());

			for (integer i = partitionBegin;i < partitionEnd;++i)
			{
				const integer pointIndex = pointPartition_[i];
				minPoint = min(pointSet_[pointIndex], minPoint);
				maxPoint = max(pointSet_[pointIndex], maxPoint);
			}

			// Expand minimally to a cube which is contained
			// in the original cube.

			const AlignedBox<N, Real> originalBound = box->bound_;
			const Point<N, Real>& originalMin = originalBound.min();
			const Point<N, Real>& originalMax = originalBound.max();

			const bool beforeExtension = true;
			ASSERT(beforeExtension && allGreaterEqual(minPoint, originalMin));
			ASSERT(beforeExtension && allLessEqual(maxPoint, originalMax));

			const Vector<N, Real> extent = maxPoint - minPoint;

			const integer maxSide = maxIndex(extent);
			const Real maxSideLength = extent[maxSide];
			for (integer i = 0;i < dimension_;++i)
			{
				if (i != maxSide)
				{
					const Real center = linear(minPoint[i], maxPoint[i], 0.5);
					Real newMin = center - maxSideLength * 0.5;
					Real newMax = center + maxSideLength * 0.5;
					
					if (newMin < originalMin[i])
					{
						newMax += originalMin[i] - newMin;
						newMin = originalMin[i];
					}
					if (newMax > originalMax[i])
					{
						newMin -= newMax - originalMax[i];
						newMax = originalMax[i];
					}

					// Due to rounding errors we can
					// end up shrinking the box, leaving
					// some points outside of it.
					// We make sure this does not happen.

					minPoint[i] = std::max(
						std::min(minPoint[i], newMin),
						originalMin[i]);
					maxPoint[i] = std::min(
						std::max(maxPoint[i], newMax),
						originalMax[i]);
				}
			}

			ASSERT(allGreaterEqual(minPoint, originalMin));
			ASSERT(allLessEqual(maxPoint, originalMax));
			
			box->bound_.set(minPoint, maxPoint);
		}

		template <int N, typename Real>
		void AllNearestNeighborsVaidya<N, Real>::createEstimate(
			const NeighborhoodPtr& box)
		{
			ASSERT(box->points() > 0);

			//if (box->points() == 1)
			if (volume(box->bound_) == 0)
			{
				box->estimate_ = estimate(
					box,
					box->neighborSet_.begin(),
					box->neighborSet_.end());
			}
			else
			{
				box->estimate_ = diameter2(box->bound_);
			}
		}

		template <int N, typename Real>
		void AllNearestNeighborsVaidya<N, Real>::updateEstimate(
			const NeighborhoodPtr& box,
			const std::vector<NeighborhoodPtr>& childSet)
		{
			box->estimate_ = std::min(
				box->estimate_, 
				estimate(box, childSet.begin(), childSet.end()));
		}

		template <int N, typename Real>
		template <typename InputIterator>
		Real AllNearestNeighborsVaidya<N, Real>::estimate(
			const NeighborhoodPtr& box,
			const InputIterator& begin,
			const InputIterator& end)
		{
			Real minFarthestDistance2 = infinity<Real>();

			InputIterator iter = begin;
			while(iter != end)
			{
				NeighborhoodPtr neighbor = *iter;
				const Real distance2 =
					farthestDistance2(neighbor->bound_, box->bound_);

				ASSERT(neighbor != box);

				if (distance2 < minFarthestDistance2)
				{
					minFarthestDistance2 = distance2;
				}

				++iter;
			}

			ASSERT(minFarthestDistance2 != 0);

			return minFarthestDistance2;
		}

		template <int N, typename Real>
		template <typename InputIterator>
		void AllNearestNeighborsVaidya<N, Real>::removeUnderEstimate(
			const InputIterator& iterBegin,
			const InputIterator& iterEnd)
		{
			InputIterator iter = iterBegin;
			while(iter != iterEnd)
			{
				const NeighborhoodPtr box = *iter;
				
				NeighborhoodIterator neighborIter = 
					box->neighborSet_.begin();
				const NeighborhoodIterator neighborIterEnd = 
					box->neighborSet_.end();
				while(neighborIter != neighborIterEnd)
				{
					const NeighborhoodPtr neighbor = *neighborIter;
					NeighborhoodIterator nextNeighborIter = neighborIter;
					++nextNeighborIter;

					if (distance2(box->bound_, neighbor->bound_) >
						box->estimate_)
					{
						neighbor->attractorSet_.erase(box);
						box->neighborSet_.erase(neighborIter);
					}

					neighborIter = nextNeighborIter;
				}

				++iter;
			}
		}

		template <int N, typename Real>
		void AllNearestNeighborsVaidya<N, Real>::extractNearest()
		{
			const integer points = singularSet_.size();
			ASSERT2(points == pointSet_.size(), points, pointSet_.size());

			integer emptyOnes = 0;

			for (integer i = 0;i < points;++i)
			{
				const NeighborhoodPtr box = singularSet_[i];

				ASSERT(box->points() == 1);

				const integer pointIndex = pointPartition_[box->partitionBegin_];

				/*
				log() << "(" << box->estimate_ << ", " << box->neighborSet_.size() << ", "
					<< box->attractorSet_.size() << "), ";
				if ((i % 10) == 9)
				{
					log() << logNewLine;
				}
				*/

				if (box->neighborSet_.empty())
				{
					nearestSet_(0, pointIndex) = -1;
					++emptyOnes;
				}
				else
				{
					const integer neighborIndex = (*box->neighborSet_.begin())->partitionBegin_;
					nearestSet_(0, pointIndex) = pointPartition_[neighborIndex];
				}
				//ASSERT(!box->neighborSet_.empty());
				//nearestSet_(0, pointIndex) = (*box->neighborSet_.begin())->partitionBegin_;
			}

			log() << "Empty neighbor sets = " << emptyOnes << logNewLine;
		}

	}

	template <int N, typename Real>
	void searchAllNeighborsVaidya(
		const std::vector<Point<N, Real> >& pointSet,
		integer kNearest,
		Array<2, integer>& nearestSet)
	{
		ENSURE_OP(kNearest, >=, 1);
		ENSURE2(nearestSet.width() == kNearest, nearestSet.width(), kNearest);
		ENSURE2(nearestSet.height() == pointSet.size(), nearestSet.height(), pointSet.size());

		if (pointSet.empty())
		{
			return;
		}

		const integer dimension = pointSet.front().dimension();

		Detail_AllNearestNeighborsVaidya::AllNearestNeighborsVaidya<N, Real> computation(
			dimension, pointSet, kNearest, nearestSet);
		
		computation.work();
	}

}

#endif
