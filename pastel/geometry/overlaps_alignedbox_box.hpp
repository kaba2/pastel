#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_BOX_HPP
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_BOX_HPP

#include "pastel/geometry/overlaps_alignedbox_box.h"
#include "pastel/geometry/overlaps_alignedbox_alignedbox.h"
#include "pastel/geometry/bounding_alignedbox.h"
#include "pastel/geometry/linear_projection.h"

#include "pastel/sys/mytypes.h"

#include "pastel/sys/math_functions.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	// Specialization for 2D

	template <typename Real>
	bool overlaps(
		const AlignedBox<Real, 2>& alignedBox,
		const Box<Real, 2>& box)
	{
		// Using the separating axis theorem.

		// In dimension 2, there are only 4 axes to test,
		// 2 from each box.

		// The following might seem messy. This is because
		// we want to take advantage of the zeros and ones
		// in the aligned box normals.

		const Vector<Real, 2> alignedBoxWidth(
			(alignedBox.max() - alignedBox.min()) * 0.5);
		const Vector<Real, 2> delta(
			box.position() - linear(alignedBox.min(), alignedBox.max(), 0.5));
		const Matrix<Real>& boxRotation = box.rotation();
		const Vector<Real, 2>& boxWidth = box.width();
		Matrix<Real> absRotation(abs(box.rotation()));

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
		const AlignedBox<Real, 2>& aAlignedBox,
		const Box<Real, 2>& bBox,
		const Vector<Real, 2>& bVelocity,
		Tuple<Real, 2>& intersectionRange)
	{
		// Using the separating axis theorem.

		const AlignedBox<Real, 2> bAlignedBox = boundingAlignedBox(bBox);

		// Test for the standard basis vectors.

		Tuple<Real, 2> tRange;
		if (!overlaps(aAlignedBox, bAlignedBox, bVelocity, tRange))
		{
			return false;
		}

		Real tMaxStart = tRange[0];
		Real tMinEnd = tRange[1];

		// Test for the 'bBox' vectors.

		for (integer i = 0;i < 2;++i)
		{
			const Vector<Real, N>& unitAxis = bBox.rotation()[i];
			const AlignedBox<Real, 1> aInterval = projectAxis(aAlignedBox, unitAxis);
			const AlignedBox<Real, 1> bInterval = projectAxis(bBox, unitAxis);

			const Vector<Real, 1> bProjectedVelocity(dot(bVelocity, unitAxis));

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
		const AlignedBox<Real, 2>& alignedBox,
		const Box<Real, 2>& box,
		Vector<Real, 2>& projection,
		Vector<Real, 2>& commonPoint)
	{
		// Using the separating axis theorem.

		Real minDepth = infinity<Real>();
		Vector<Real, 2> minProjection;

		// In dimension 2, there are only 4 axes to test,
		// 2 from each box.

		// The following might seem messy. This is because
		// we want to take advantage of the zeros and ones
		// in the aligned box normals.

		const Vector<Real, 2> alignedBoxWidth(
			(alignedBox.max() - alignedBox.min()) * 0.5);
		const Vector<Real, 2> delta(
			box.position() - linear(alignedBox.min(), alignedBox.max(), 0.5));
		const Matrix<Real>& boxRotation = box.rotation();
		const Vector<Real, 2>& boxWidth = box.width();

		Matrix<Real> absRotation(abs(box.rotation()));

		Vector<Real, 2> someCommonPoint;

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
					minProjection = unitAxis<2>(i);
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
		const AlignedBox<Real, 3>& alignedBox,
		const Box<Real, 3>& box)
	{
		// Use separating axis theorem to
		// test for aligned box-box overlap.

		// In dimension 3, there are 15 axes to test:
		// 3 from box alignedBox, 3 from box box, and 9 from
		// cross product axes.

		// The following might seem messy. This is because
		// we want to take advantage of the zeros and ones
		// in the aligned box normals.

		const Vector<Real, 3> alignedBoxWidth((alignedBox.max() - alignedBox.min()) * 0.5);

		const Vector<Real, 3> delta(
			box.position() - linear(alignedBox.min(), alignedBox.max(), 0.5));
		const Matrix<Real>& boxRotation = box.rotation();
		const Vector<Real, 3>& boxWidth = box.width();
		Matrix<Real> absRotation(abs(box.rotation()));

		// Test for projection axes from aligned box

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
		const AlignedBox<Real, 3>& alignedBox,
		const Box<Real, 3>& box,
		Vector<Real, 3>& projection)
	{
		// Use separating axis theorem to
		// test for aligned box-box overlap.

		Real minDepth = infinity<Real>();
		Vector<Real, 3> minProjection;

		// In dimension 3, there are 15 axes to test:
		// 3 from box aligned box, 3 from box box, and 9 from
		// cross product axes.

		// The following might seem messy. This is because
		// we want to take advantage of the zeros and ones
		// in the alignedBox normals.

		const Vector<Real, 3> alignedBoxWidth((alignedBox.max() - alignedBox.min()) * 0.5);

		const Vector<Real, 3> delta(
			box.position() - linear(alignedBox.min(), alignedBox.max(), 0.5));
		const Matrix<Real>& boxRotation = box.rotation();
		const Vector<Real, 3>& boxWidth = box.width();
		Matrix<Real> absRotation(abs(box.rotation()));

		// Test for projection axes from aligned box

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
					minProjection = unitAxis<3>(i);
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
