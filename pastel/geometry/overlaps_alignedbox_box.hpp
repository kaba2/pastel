#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_BOX_HPP
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_BOX_HPP

#include "pastel/geometry/overlaps_alignedbox_box.h"
#include "pastel/geometry/overlaps_alignedbox_alignedbox.h"
#include "pastel/geometry/bounding_alignedbox.h"
#include "pastel/geometry/projectaxis.h"

#include "pastel/sys/mytypes.h"

#include "pastel/math/linear.h"

#include "pastel/sys/mathfunctions.h"

namespace Pastel
{

	// Specialization for 2D

	template <typename Real>
	bool overlaps(
		const AlignedBox<2, Real>& alignedBox,
		const Box<2, Real>& box)
	{
		// Using the separating axis theorem.

		// In dimension 2, there are only 4 axes to test,
		// 2 from each box.

		// The following might seem messy. This is because
		// we want to take advantage of the zeros and ones
		// in the alignedBox normals.

		const Vector<2, Real> alignedBoxWidth(
			(alignedBox.max() - alignedBox.min()) * 0.5);
		const Vector<2, Real> delta(
			box.position() - linear(alignedBox.min(), alignedBox.max(), 0.5));
		const Matrix<2, 2, Real>& boxRotation = box.rotation();
		const Vector<2, Real>& boxWidth = box.width();
		Matrix<2, 2, Real> absRotation(box.rotation());
		modify(absRotation, (real(*)(real))mabs);

		// Project to the aligned box's normals
		// (the natural basis vectors).

		for (integer i = 0;i < 2;++i)
		{
			// Because the aligned box axes are the
			// vectors of the natural basis,
			// the projection to i:th axis just picks up
			// the i:th coordinate.

			const Real aProjectedRadius =
				alignedBoxWidth[i];

			// The projection of the box on the axis
			// is symmetric w.r.t projection of
			// the center point. Here we find the
			// (radius of the) projection of the
			// extremal point.

			const Real bProjectedRadius =
				boxWidth[0] * absRotation[0][i] +
				boxWidth[1] * absRotation[1][i];

			// This is the distance between
			// the centers of the aligned box and the box on
			// this axis.
			// Remember the aligned box is at the origin.

			const Real signedProjectedDistance = delta[i];

			const Real depth =
				(aProjectedRadius + bProjectedRadius) -
				mabs(signedProjectedDistance);

			if (depth < 0)
			{
				// Found a separating axis.
				return false;
			}
		}

		// Project to the box's normals
		// (the vectors of its rotation matrix).

		for (integer i = 0;i < 2;++i)
		{
			// Find out the radius of the
			// projection of the aligned box's extremal
			// point on this box axis.

			const Real aProjectedRadius =
				alignedBoxWidth[0] * absRotation[i][0] +
				alignedBoxWidth[1] * absRotation[i][1];

			// On box's own coordinate system,
			// the radius of the projection is
			// simply the width of the box.

			const Real bProjectedRadius = boxWidth[i];

			// This is the distance between
			// the centers of the aligned box and the box on
			// this axis.
			// Remember the aligned box is at the origin.

			const Real signedProjectedDistance =
				dot(box.rotation()[i], delta);

			const Real depth =
				(aProjectedRadius + bProjectedRadius) -
				mabs(signedProjectedDistance);

			if (depth < 0)
			{
				// Found a separating axis.
				return false;
			}
		}

		// No separating axis found, so the boxs
		// are intersecting.

		return true;
	}

	template <typename Real>
	bool overlaps(
		const AlignedBox<2, Real>& aAlignedBox,
		const Box<2, Real>& bBox,
		const Vector<2, Real>& bVelocity,
		Tuple<2, Real>& intersectionRange)
	{
		// Using the separating axis theorem.

		enum
		{
			N = 2
		};

		const AlignedBox<N, Real> bAlignedBox = boundingAlignedBox(bBox);

		// Test for the standard basis vectors.

		Tuple<2, Real> tRange;
		if (!overlaps(aAlignedBox, bAlignedBox, bVelocity, tRange))
		{
			return false;
		}

		Real tMaxStart = tRange[0];
		Real tMinEnd = tRange[1];

		// Test for the 'bBox' vectors.

		for (integer i = 0;i < N;++i)
		{
			const Vector<N, Real>& unitAxis = bBox.rotation()[i];
			const AlignedBox<1, Real> aInterval = projectAxis(aAlignedBox, unitAxis);
			const AlignedBox<1, Real> bInterval = projectAxis(bBox, unitAxis);

			const Vector<1, Real> bProjectedVelocity(dot(bVelocity, unitAxis));

			if (!overlaps(aInterval, bInterval, bProjectedVelocity, tRange))
			{
				return false;
			}

			if (tRange[0] > tMaxStart)
			{
				tMaxStart = tRange[0];
			}
			if (tRange[1] < tMinEnd)
			{
				tMinEnd = tRange[1];
			}
			if (tMaxStart > tMinEnd)
			{
				return false;
			}
		}

		intersectionRange.set(tMaxStart, tMinEnd);

		return true;
	}

	template <typename Real>
	bool overlaps(
		const AlignedBox<2, Real>& alignedBox,
		const Box<2, Real>& box,
		Vector<2, Real>& projection,
		Point<2, Real>& commonPoint)
	{
		// Using the separating axis theorem.

		Real minDepth = infinity<Real>();
		Vector<2, Real> minProjection;

		// In dimension 2, there are only 4 axes to test,
		// 2 from each box.

		// The following might seem messy. This is because
		// we want to take advantage of the zeros and ones
		// in the aligned box normals.

		const Vector<2, Real> alignedBoxWidth(
			(alignedBox.max() - alignedBox.min()) * 0.5);
		const Vector<2, Real> delta(
			box.position() - linear(alignedBox.min(), alignedBox.max(), 0.5));
		const Matrix<2, 2, Real>& boxRotation = box.rotation();
		const Vector<2, Real>& boxWidth = box.width();

		Matrix<2, 2, Real> absRotation(box.rotation());
		modify(absRotation, (real(*)(real))mabs);

		Point<2, Real> someCommonPoint;

		// Project to the aligned box's normals
		// (the standard basis vectors).

		for (integer i = 0;i < 2;++i)
		{
			// Because the aligned box axes are the
			// vectors of the standard basis,
			// the projection to i:th axis just picks up
			// the i:th coordinate.

			const Real aProjectedRadius =
				alignedBoxWidth[i];

			// The projection of the box on the axis
			// is symmetric w.r.t projection of
			// the center point. Here we find the
			// (radius of the) projection of the
			// box.

			const Real bProjectedRadius =
				boxWidth[0] * absRotation[0][i] +
				boxWidth[1] * absRotation[1][i];

			const Real projectedRadiusSum =
				aProjectedRadius + bProjectedRadius;

			// This is the distance between
			// the centers of the aligned box and the box on
			// this axis.

			const Real signedProjectedDistance = delta[i];
			const Real projectedDistance =
				mabs(signedProjectedDistance);

			if (projectedRadiusSum < projectedDistance)
			{
				// Found a separating axis.
				return false;
			}

			// Compute penetration depth

			if (signedProjectedDistance < 0)
			{
				const Real depth = signedProjectedDistance + projectedRadiusSum;

				if (depth < minDepth)
				{
					minDepth = depth;
					minProjection = -unitAxis<2, Real>(i);
				}
			}
			else
			{
				const Real depth = -signedProjectedDistance + projectedRadiusSum;

				if (depth < minDepth)
				{
					minDepth = depth;
					minProjection = unitAxis<2, Real>(i);
				}
			}

			// Compute a common point.

			const Real aProjectedCenter =
				linear(alignedBox.min()[i], alignedBox.max()[i], 0.5);
			const Real bProjectedCenter =
				box.position()[i];

			const Real aMin = aProjectedCenter - aProjectedRadius;
			const Real aMax = aProjectedCenter + aProjectedRadius;
			const Real bMin = bProjectedCenter - bProjectedRadius;
			const Real bMax = bProjectedCenter + bProjectedRadius;

			const Real projectedIntersectionMin = std::max(aMin, bMin);
			const Real projectedIntersectionMax = std::min(aMax, bMax);

			someCommonPoint[i] =
				linear(projectedIntersectionMin, projectedIntersectionMax, 0.5);
		}

		// Project to the box's normals
		// (the vectors of its rotation matrix).

		for (integer i = 0;i < 2;++i)
		{
			// Find out the radius of the
			// projection of the aligned box's extremal
			// point on this box axis.

			const Real aProjectedRadius =
				alignedBoxWidth[0] * absRotation[i][0] +
				alignedBoxWidth[1] * absRotation[i][1];

			// On box's own coordinate system,
			// the radius of the projection is
			// simply the width of the box.

			const Real bProjectedRadius = boxWidth[i];

			const Real projectedRadiusSum =
				aProjectedRadius + bProjectedRadius;

			// This is the distance between
			// the centers of the aligned box and the box on
			// this axis.
			// Remember the aligned box is at the origin.

			const Real signedProjectedDistance =
				dot(box.rotation()[i], delta);
			const Real projectedDistance =
				mabs(signedProjectedDistance);

			if (projectedRadiusSum < projectedDistance)
			{
				// Found a separating axis.
				return false;
			}

			// Compute penetration depth

			if (signedProjectedDistance < 0)
			{
				const Real depth = signedProjectedDistance + projectedRadiusSum;

				if (depth < minDepth)
				{
					minDepth = depth;
					minProjection = -box.rotation()[i];
				}
			}
			else
			{
				const Real depth = -signedProjectedDistance + projectedRadiusSum;

				if (depth < minDepth)
				{
					minDepth = depth;
					minProjection = box.rotation()[i];
				}
			}
		}

		// No separating axis found, so the boxs
		// are intersecting.

		projection = minProjection * minDepth;
		commonPoint = someCommonPoint;

		return true;
	}

	// Specialization for 3D

	template <typename Real>
	bool overlaps(
		const AlignedBox<3, Real>& alignedBox,
		const Box<3, Real>& box)
	{
		// Use separating axis theorem to
		// test for aligned box-box overlap.

		// In dimension 3, there are 15 axes to test:
		// 3 from box alignedBox, 3 from box box, and 9 from
		// cross product axes.

		// The following might seem messy. This is because
		// we want to take advantage of the zeros and ones
		// in the alignedBox normals.

		const Vector<3, Real> alignedBoxWidth((alignedBox.max() - alignedBox.min()) * 0.5);

		const Vector<3, Real> delta(
			box.position() - linear(alignedBox.min(), alignedBox.max(), 0.5));
		const Matrix<3, 3, Real>& boxRotation = box.rotation();
		const Vector<3, Real>& boxWidth = box.width();
		Matrix<3, 3, Real> absRotation(box.rotation());
		modify(absRotation, (real(*)(real))mabs);

		// Test for projection axes from alignedBox

		for (integer i = 0;i < 3;++i)
		{
			const Real aProjectedRadius =
				alignedBoxWidth[i];
			const Real bProjectedRadius =
				boxWidth[0] * absRotation[0][i] +
				boxWidth[1] * absRotation[1][i] +
				boxWidth[2] * absRotation[2][i];
			const Real signedProjectedDistance =
				delta[i];

			const Real depth = (aProjectedRadius + bProjectedRadius) -
				mabs(signedProjectedDistance);

			if (depth < 0)
			{
				return false;
			}
		}

		// Test for projection axes from box

		for (integer i = 0;i < 3;++i)
		{
			const Real aProjectedRadius =
				alignedBoxWidth[0] * absRotation[i][0] +
				alignedBoxWidth[1] * absRotation[i][1] +
				alignedBoxWidth[2] * absRotation[i][2];
			const Real bProjectedRadius = boxWidth[i];
			const Real signedProjectedDistance =
				dot(delta, boxRotation[i]);

			const Real depth = (aProjectedRadius + bProjectedRadius) -
				mabs(signedProjectedDistance);

			if (depth < 0)
			{
				return false;
			}
		}

		// Test for cross product axes

		{
			const Real signedProjectedDistance =
				boxRotation[0][1] * delta[2] -
				boxRotation[0][2] * delta[1];
			const Real aProjectedRadius =
				alignedBoxWidth[2] * absRotation[0][1] +
				alignedBoxWidth[1] * absRotation[0][2];
			const Real bProjectedRadius =
				boxWidth[2] * absRotation[1][0] +
				boxWidth[1] * absRotation[2][0];
			const Real depth = (aProjectedRadius + bProjectedRadius) -
				mabs(signedProjectedDistance);

			if (depth < 0)
			{
				return false;
			}
		}

		{
			const Real signedProjectedDistance =
				boxRotation[1][1] * delta[2] -
				boxRotation[1][2] * delta[1];
			const Real aProjectedRadius =
				alignedBoxWidth[2] * absRotation[1][1] +
				alignedBoxWidth[1] * absRotation[1][2];
			const Real bProjectedRadius =
				boxWidth[2] * absRotation[1][1] +
				boxWidth[1] * absRotation[2][1];
			const Real depth = (aProjectedRadius + bProjectedRadius) -
				mabs(signedProjectedDistance);

			if (depth < 0)
			{
				return false;
			}
		}

		{
			const Real signedProjectedDistance =
				boxRotation[2][1] * delta[2] -
				boxRotation[2][2] * delta[1];
			const Real aProjectedRadius =
				alignedBoxWidth[2] * absRotation[2][1] +
				alignedBoxWidth[1] * absRotation[2][2];
			const Real bProjectedRadius =
				boxWidth[2] * absRotation[1][2] +
				boxWidth[1] * absRotation[2][2];
			const Real depth = (aProjectedRadius + bProjectedRadius) -
				mabs(signedProjectedDistance);

			if (depth < 0)
			{
				return false;
			}
		}

		{
			const Real signedProjectedDistance =
				boxRotation[0][2] * delta[0] -
				boxRotation[0][0] * delta[2];
			const Real aProjectedRadius =
				alignedBoxWidth[2] * absRotation[0][0] +
				alignedBoxWidth[0] * absRotation[0][2];
			const Real bProjectedRadius =
				boxWidth[2] * absRotation[0][0] +
				boxWidth[0] * absRotation[2][0];
			const Real depth = (aProjectedRadius + bProjectedRadius) -
				mabs(signedProjectedDistance);

			if (depth < 0)
			{
				return false;
			}

		}

		{
			const Real signedProjectedDistance =
				boxRotation[1][2] * delta[0] -
				boxRotation[1][0] * delta[2];
			const Real aProjectedRadius =
				alignedBoxWidth[2] * absRotation[1][0] +
				alignedBoxWidth[0] * absRotation[1][2];
			const Real bProjectedRadius =
				boxWidth[2] * absRotation[0][1] +
				boxWidth[0] * absRotation[2][1];
			const Real depth = (aProjectedRadius + bProjectedRadius) -
				mabs(signedProjectedDistance);

			if (depth < 0)
			{
				return false;
			}
		}

		{
			const Real signedProjectedDistance =
				boxRotation[2][2] * delta[0] -
				boxRotation[2][0] * delta[2];
			const Real aProjectedRadius =
				alignedBoxWidth[2] * absRotation[2][0] +
				alignedBoxWidth[0] * absRotation[2][2];
			const Real bProjectedRadius =
				boxWidth[2] * absRotation[0][2] +
				boxWidth[0] * absRotation[2][2];
			const Real depth = (aProjectedRadius + bProjectedRadius) -
				mabs(signedProjectedDistance);

			if (depth < 0)
			{
				return false;
			}
		}

		{
			const Real signedProjectedDistance =
				boxRotation[0][0] * delta[1] -
				boxRotation[0][1] * delta[0];
			const Real aProjectedRadius =
				alignedBoxWidth[1] * absRotation[0][0] +
				alignedBoxWidth[0] * absRotation[0][1];
			const Real bProjectedRadius =
				boxWidth[1] * absRotation[0][0] +
				boxWidth[0] * absRotation[1][0];
			const Real depth = (aProjectedRadius + bProjectedRadius) -
				mabs(signedProjectedDistance);

			if (depth < 0)
			{
				return false;
			}
		}

		{
			const Real signedProjectedDistance =
				boxRotation[1][0] * delta[1] -
				boxRotation[1][1] * delta[0];
			const Real aProjectedRadius =
				alignedBoxWidth[1] * absRotation[1][0] +
				alignedBoxWidth[0] * absRotation[1][1];
			const Real bProjectedRadius =
				boxWidth[1] * absRotation[0][1] +
				boxWidth[0] * absRotation[1][1];
			const Real depth = (aProjectedRadius + bProjectedRadius) -
				mabs(signedProjectedDistance);

			if (depth < 0)
			{
				return false;
			}
		}

		{
			const Real signedProjectedDistance =
				boxRotation[2][0] * delta[1] -
				boxRotation[2][1] * delta[0];
			const Real aProjectedRadius =
				alignedBoxWidth[1] * absRotation[2][0] +
				alignedBoxWidth[0] * absRotation[2][1];
			const Real bProjectedRadius =
				boxWidth[1] * absRotation[0][2] +
				boxWidth[0] * absRotation[1][2];
			const Real depth = (aProjectedRadius + bProjectedRadius) -
				mabs(signedProjectedDistance);

			if (depth < 0)
			{
				return false;
			}
		}

		// No separating axis found, so the boxes
		// are intersecting.

		return true;
	}

	template <typename Real>
	bool overlaps(
		const AlignedBox<3, Real>& alignedBox,
		const Box<3, Real>& box,
		Vector<3, Real>& projection)
	{
		// Use separating axis theorem to
		// test for alignedBox-box overlap.

		Real minDepth = infinity<Real>();
		Vector<3, Real> minProjection;

		// In dimension 3, there are 15 axes to test:
		// 3 from box alignedBox, 3 from box box, and 9 from
		// cross product axes.

		// The following might seem messy. This is because
		// we want to take advantage of the zeros and ones
		// in the alignedBox normals.

		const Vector<3, Real> alignedBoxWidth((alignedBox.max() - alignedBox.min()) * 0.5);

		const Vector<3, Real> delta(
			box.position() - linear(alignedBox.min(), alignedBox.max(), 0.5));
		const Matrix<3, 3, Real>& boxRotation = box.rotation();
		const Vector<3, Real>& boxWidth = box.width();
		Matrix<3, 3, Real> absRotation(box.rotation());
		modify(absRotation, (real(*)(real))mabs);

		// Test for projection axes from alignedBox

		for (integer i = 0;i < 3;++i)
		{
			const Real aProjectedRadius =
				alignedBoxWidth[i];
			const Real bProjectedRadius =
				boxWidth[0] * absRotation[0][i] +
				boxWidth[1] * absRotation[1][i] +
				boxWidth[2] * absRotation[2][i];
			const Real signedProjectedDistance =
				delta[i];

			const Real depth = (aProjectedRadius + bProjectedRadius) -
				mabs(signedProjectedDistance);

			if (depth < 0)
			{
				return false;
			}

			if (depth < minDepth)
			{
				minDepth = depth;
				if (signedProjectedDistance > 0)
				{
					minProjection = unitAxis<3, Real>(i);
				}
				else
				{
					minProjection = -unitAxis<3, Real>(i);
				}
			}
		}

		// Test for projection axes from box

		for (integer i = 0;i < 3;++i)
		{
			const Real aProjectedRadius =
				alignedBoxWidth[0] * absRotation[i][0] +
				alignedBoxWidth[1] * absRotation[i][1] +
				alignedBoxWidth[2] * absRotation[i][2];
			const Real bProjectedRadius = boxWidth[i];
			const Real signedProjectedDistance =
				dot(delta, boxRotation[i]);

			const Real depth = (aProjectedRadius + bProjectedRadius) -
				mabs(signedProjectedDistance);

			if (depth < 0)
			{
				return false;
			}

			if (depth < minDepth)
			{
				minDepth = depth;
				if (signedProjectedDistance > 0)
				{
					minProjection = boxRotation[i];
				}
				else
				{
					minProjection = -boxRotation[i];
				}
			}
		}

		// Test for cross product axes

		{
			const Real signedProjectedDistance =
				boxRotation[0][1] * delta[2] -
				boxRotation[0][2] * delta[1];
			const Real aProjectedRadius =
				alignedBoxWidth[2] * absRotation[0][1] +
				alignedBoxWidth[1] * absRotation[0][2];
			const Real bProjectedRadius =
				boxWidth[2] * absRotation[1][0] +
				boxWidth[1] * absRotation[2][0];
			const Real depth = (aProjectedRadius + bProjectedRadius) -
				mabs(signedProjectedDistance);

			if (depth < 0)
			{
				return false;
			}

			if (depth < minDepth)
			{
				minDepth = depth;
				if (signedProjectedDistance > 0)
				{
					minProjection = boxRotation[i];
				}
				else
				{
					minProjection = -boxRotation[i];
				}
			}
		}

		{
			const Real signedProjectedDistance =
				boxRotation[1][1] * delta[2] -
				boxRotation[1][2] * delta[1];
			const Real aProjectedRadius =
				alignedBoxWidth[2] * absRotation[1][1] +
				alignedBoxWidth[1] * absRotation[1][2];
			const Real bProjectedRadius =
				boxWidth[2] * absRotation[1][1] +
				boxWidth[1] * absRotation[2][1];
			const Real depth = (aProjectedRadius + bProjectedRadius) -
				mabs(signedProjectedDistance);

			if (depth < 0)
			{
				return false;
			}

			if (depth < minDepth)
			{
				minDepth = depth;
				if (signedProjectedDistance > 0)
				{
					minProjection = boxRotation[i];
				}
				else
				{
					minProjection = -boxRotation[i];
				}
			}
		}

		{
			const Real signedProjectedDistance =
				boxRotation[2][1] * delta[2] -
				boxRotation[2][2] * delta[1];
			const Real aProjectedRadius =
				alignedBoxWidth[2] * absRotation[2][1] +
				alignedBoxWidth[1] * absRotation[2][2];
			const Real bProjectedRadius =
				boxWidth[2] * absRotation[1][2] +
				boxWidth[1] * absRotation[2][2];
			const Real depth = (aProjectedRadius + bProjectedRadius) -
				mabs(signedProjectedDistance);

			if (depth < 0)
			{
				return false;
			}

			if (depth < minDepth)
			{
				minDepth = depth;
				if (signedProjectedDistance > 0)
				{
					minProjection = boxRotation[i];
				}
				else
				{
					minProjection = -boxRotation[i];
				}
			}
		}

		{
			const Real signedProjectedDistance =
				boxRotation[0][2] * delta[0] -
				boxRotation[0][0] * delta[2];
			const Real aProjectedRadius =
				alignedBoxWidth[2] * absRotation[0][0] +
				alignedBoxWidth[0] * absRotation[0][2];
			const Real bProjectedRadius =
				boxWidth[2] * absRotation[0][0] +
				boxWidth[0] * absRotation[2][0];
			const Real depth = (aProjectedRadius + bProjectedRadius) -
				mabs(signedProjectedDistance);

			if (depth < 0)
			{
				return false;
			}

			if (depth < minDepth)
			{
				minDepth = depth;
				if (signedProjectedDistance > 0)
				{
					minProjection = boxRotation[i];
				}
				else
				{
					minProjection = -boxRotation[i];
				}
			}
		}

		{
			const Real signedProjectedDistance =
				boxRotation[1][2] * delta[0] -
				boxRotation[1][0] * delta[2];
			const Real aProjectedRadius =
				alignedBoxWidth[2] * absRotation[1][0] +
				alignedBoxWidth[0] * absRotation[1][2];
			const Real bProjectedRadius =
				boxWidth[2] * absRotation[0][1] +
				boxWidth[0] * absRotation[2][1];
			const Real depth = (aProjectedRadius + bProjectedRadius) -
				mabs(signedProjectedDistance);

			if (depth < 0)
			{
				return false;
			}

			if (depth < minDepth)
			{
				minDepth = depth;
				if (signedProjectedDistance > 0)
				{
					minProjection = boxRotation[i];
				}
				else
				{
					minProjection = -boxRotation[i];
				}
			}
		}

		{
			const Real signedProjectedDistance =
				boxRotation[2][2] * delta[0] -
				boxRotation[2][0] * delta[2];
			const Real aProjectedRadius =
				alignedBoxWidth[2] * absRotation[2][0] +
				alignedBoxWidth[0] * absRotation[2][2];
			const Real bProjectedRadius =
				boxWidth[2] * absRotation[0][2] +
				boxWidth[0] * absRotation[2][2];
			const Real depth = (aProjectedRadius + bProjectedRadius) -
				mabs(signedProjectedDistance);

			if (depth < 0)
			{
				return false;
			}

			if (depth < minDepth)
			{
				minDepth = depth;
				if (signedProjectedDistance > 0)
				{
					minProjection = boxRotation[i];
				}
				else
				{
					minProjection = -boxRotation[i];
				}
			}
		}

		{
			const Real signedProjectedDistance =
				boxRotation[0][0] * delta[1] -
				boxRotation[0][1] * delta[0];
			const Real aProjectedRadius =
				alignedBoxWidth[1] * absRotation[0][0] +
				alignedBoxWidth[0] * absRotation[0][1];
			const Real bProjectedRadius =
				boxWidth[1] * absRotation[0][0] +
				boxWidth[0] * absRotation[1][0];
			const Real depth = (aProjectedRadius + bProjectedRadius) -
				mabs(signedProjectedDistance);

			if (depth < 0)
			{
				return false;
			}

			if (depth < minDepth)
			{
				minDepth = depth;
				if (signedProjectedDistance > 0)
				{
					minProjection = boxRotation[i];
				}
				else
				{
					minProjection = -boxRotation[i];
				}
			}
		}

		{
			const Real signedProjectedDistance =
				boxRotation[1][0] * delta[1] -
				boxRotation[1][1] * delta[0];
			const Real aProjectedRadius =
				alignedBoxWidth[1] * absRotation[1][0] +
				alignedBoxWidth[0] * absRotation[1][1];
			const Real bProjectedRadius =
				boxWidth[1] * absRotation[0][1] +
				boxWidth[0] * absRotation[1][1];
			const Real depth = (aProjectedRadius + bProjectedRadius) -
				mabs(signedProjectedDistance);

			if (depth < 0)
			{
				return false;
			}

			if (depth < minDepth)
			{
				minDepth = depth;
				if (signedProjectedDistance > 0)
				{
					minProjection = boxRotation[i];
				}
				else
				{
					minProjection = -boxRotation[i];
				}
			}
		}

		{
			const Real signedProjectedDistance =
				boxRotation[2][0] * delta[1] -
				boxRotation[2][1] * delta[0];
			const Real aProjectedRadius =
				alignedBoxWidth[1] * absRotation[2][0] +
				alignedBoxWidth[0] * absRotation[2][1];
			const Real bProjectedRadius =
				boxWidth[1] * absRotation[0][2] +
				boxWidth[0] * absRotation[1][2];
			const Real depth = (aProjectedRadius + bProjectedRadius) -
				mabs(signedProjectedDistance);

			if (depth < 0)
			{
				return false;
			}

			if (depth < minDepth)
			{
				minDepth = depth;
				if (signedProjectedDistance > 0)
				{
					minProjection = boxRotation[i];
				}
				else
				{
					minProjection = -boxRotation[i];
				}
			}
		}

		// No separating axis found, so the boxs
		// are intersecting.

		projection = minProjection * minDepth;

		return true;
	}

}

#endif
