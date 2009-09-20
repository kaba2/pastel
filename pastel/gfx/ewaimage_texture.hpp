#ifndef PASTEL_EWAIMAGE_TEXTURE_HPP
#define PASTEL_EWAIMAGE_TEXTURE_HPP

#include "pastel/gfx/ewaimage_texture.h"
#include "pastel/gfx/bilinearimage_texture.h"

#include "pastel/geometry/ellipsoid.h"

namespace Pastel
{

	template <typename Type>
	Type EwaImage_Texture<Type>::operator()(
		const Vector2& p_,
		const Vector2& dpDx_,
		const Vector2& dpDy_) const
	{
		// Read ewaimatexture.txt for implementation documentation.

		if (!mipMap_ || mipMap_->empty())
		{
			return Type(0.5);
		}

		const Array<Type, 2>& mostDetailedImage = mipMap_->mostDetailed();

		const Vector2 imageExtent(mostDetailedImage.extent());

		// The derivative vectors along with 'uv' represent an affine
		// transformation from the image plane to the texture plane.

		const Vector2 p(p_ * imageExtent);
		const Vector2 dpDx = dpDx_ * imageExtent * filterRadius_;
		const Vector2 dpDy = dpDy_ * imageExtent * filterRadius_;

		// Find the ellipse that is obtained by applying
		// that affine transformation to a unit sphere.

		const Matrix2 basis(
			dpDx, dpDy);

		Matrix2 quadraticForm =
			ellipsoidQuadraticForm(basis);

		// We do not use the coefficients as computed.
		// These small modifications make sure that
		// the filter always intersects at least one pixel.
		// This way the filter can also be used for
		// reconstruction.

		quadraticForm *= square(determinant(basis));
		quadraticForm(0, 0) += square(filterRadius_);
		quadraticForm(1, 1) += square(filterRadius_);
		quadraticForm /= determinant(quadraticForm);

		// Now find out the ellipses major
		// and minor axis. These are given
		// by the eigenvectors and eigenvalues
		// of the 'quadraticForm'.
		//
		// Let
		// x^T S x = 1
		// where S is symmetric.
		//
		// Let x be an eigenvector of S corresponding to the eigenvalue k
		// with the restriction that it lies on the ellipsoid:
		//
		// x^T S x = k x^T x = 1
		// =>
		// k = 1 / |x|^2
		// =>
		// |x| = 1 / sqrt(k)
		//
		// Thus the lengths of the principal axes can be
		// computed from the eigenvalues.

		const Vector2 eigenValue = symmetricEigenValues(quadraticForm);

		real majorAxisLength2 = inverse(eigenValue[0]);
		real minorAxisLength2 = inverse(eigenValue[1]);

		// The eigenvalues are returned in ascending order.

		const real eccentricity2 = majorAxisLength2/ minorAxisLength2;

		const real MaxEccentricity2 = square(30);
		if (eccentricity2 > MaxEccentricity2)
		{
			// The ellipse is way too skinny. This can
			// cause an unbounded amount of computation
			// for the filtering and thus we need to modify the filter
			// somehow. This will cause errors: we just hope that
			// the errors are not that noticable. We have two options:
			// 1) Shortening the major axis. This results in aliasing.
			// 2) Lenghtening the minor axis. This results in blurring.
			// We can't allow 1) so we select 2).

			// Scale the minor axis so that the eccentricity
			// becomes MaxEccentricity:
			// majorAxisLength / (k * minorAxisLength) = MaxEccentricity
			// =>
			// k = majorAxisLength / (minorAxisLength * MaxEccentricity)
			// = eccentricity / MaxEccentricity

			const Vector2 aMajorAxisCandidate(
				quadraticForm(1, 0), eigenValue[0] - quadraticForm(0, 0));
			const Vector2 bMajorAxisCandidate(
				eigenValue[0] - quadraticForm(1, 1), quadraticForm(1, 0));

			Vector2 majorAxis;

			if (normManhattan(aMajorAxisCandidate) <
				normManhattan(bMajorAxisCandidate))
			{
				majorAxis = bMajorAxisCandidate;
			}
			else
			{
				majorAxis = aMajorAxisCandidate;
			}

			majorAxis /= norm(majorAxis);

			Vector2 minorAxis = cross(majorAxis);

			minorAxisLength2 *= (eccentricity2 / MaxEccentricity2);

			minorAxis *= std::sqrt(minorAxisLength2);
			majorAxis *= std::sqrt(majorAxisLength2);

			// The quadratic form has to be recomputed since
			// the minor axis has changed.

			quadraticForm = ellipsoidQuadraticForm(
				Matrix2(minorAxis, majorAxis));
		}

		// Select an appropriate mipmap level.
		// We want the minor axis to cover at least 'pixelsPerMinorAxis' pixels per 'minorAxis' length
		// in the more detailed image (note that it is half of the minor diameter).

		const real invLn2 = inverse(constantLn2<real>());
		const real pixelsPerMinorAxis = 2;
		const real level = 0.5 * std::log(minorAxisLength2 /
			square(pixelsPerMinorAxis * filterRadius_)) * invLn2;

		//return Type(level / (mipMap_->levels() - 1));

		const AlignedBox2 bound =
			ellipsoidBoundingAlignedBox(quadraticForm) + p;

		// We want 'f' to give the value of
		// 'filterTableSize' at the edge of the ellipse.
		// This normalization does that.

		quadraticForm *= filterTableSize_;

		// Compute filter transition coefficient.

		const real transitionBegin = 0;
		const real transitionEnd = 0.15;
		const real transitionWidth = transitionEnd - transitionBegin;
		const real normalizedLevel = level / (real)(mipMap_->levels() - 1);
		const real tTransition = clamp((normalizedLevel - transitionBegin) / transitionWidth,
			(real)0, (real)1);

		//return Type(tTransition);

		// If the level is non-positive, then
		// we have magnification and can simply reconstruct
		// the most detailed image.

		if (level <= 0)
		{
			return
				sampleEwa(p, quadraticForm, bound, 1, tTransition,
				mostDetailedImage);
		}

		// If the ellipse covers the whole image, we can
		// simply return the average of the whole image.

		if (level >= mipMap_->levels() - 1)
		{
			return mipMap_->coarsest()(0, 0);
		}

		// Otherwise we interpolate the results of the two
		// image levels.

		const integer detailLevel = std::floor(level);
		const Array<Type, 2>& detailImage = (*mipMap_)(detailLevel);
		const Type detailSample =
			sampleEwa(p, quadraticForm, bound, (real)1 / (1 << detailLevel), tTransition,
			detailImage);

		const integer coarseLevel = detailLevel + 1;
		const Array<Type, 2>& coarseImage = (*mipMap_)(coarseLevel);
		const Type coarseSample =
			sampleEwa(p, quadraticForm, bound, (real)1 / (1 << coarseLevel), tTransition,
			coarseImage);

		return linear(detailSample, coarseSample, level - detailLevel);
	}

	template <typename Type>
	Type EwaImage_Texture<Type>::sampleEwa(
		const Vector2& p,
		const Matrix2& quadraticForm,
		const AlignedBox2& bound,
		real scaling,
		real tTransition,
		const Array<Type, 2>& image) const
	{
		// Read ewaimatexture.txt for implementation documentation.

		const Rectangle2 window(
			floor(bound.min() * scaling),
			ceil(bound.max() * scaling));

		// Compute start values.

		const Vector2 pStart(Vector2(window.min()) + 0.5 - p * scaling);

		const real formScaling = inverse(square(scaling));

		real fLeft = dot(pStart * quadraticForm, pStart) * formScaling;
		Vector2 dfLeft = (2 * (pStart * quadraticForm) + diagonal(quadraticForm)) * formScaling;
		const Matrix2 ddf = (2 * formScaling) * quadraticForm;

		Type imageSum(0);
		real weightSum = 0;

		for (integer i = window.min().y();i < window.max().y();++i)
		{
			real f = fLeft;
			real dfDu = dfLeft.x();

			for (integer j = window.min().x();j < window.max().x();++j)
			{
				const real fClamped = f < 0 ? 0 : f;

				if (fClamped < filterTableSize_)
				{
					const real weight = linear(maxFilterTable_[fClamped],
						minFilterTable_[fClamped], tTransition);

					imageSum += weight * extender_(image, Vector2i(j, i));
					weightSum += weight;
				}

				f += dfDu;
				dfDu += ddf(0, 0);
			}

			fLeft += dfLeft.y();
			dfLeft += ddf[1];
		}

		if (weightSum <= 0)
		{
			// Reaching this place would mean
			// that no pixels intersected the ellipse.
			// This should never happen because
			// the filter we use always
			// intersects at least one pixel.

			return Type(0);
		}

		return imageSum / weightSum;
	}

}

#endif
