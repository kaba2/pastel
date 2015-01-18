#ifndef PASTELGFX_EWAIMAGE_TEXTURE_HPP
#define PASTELGFX_EWAIMAGE_TEXTURE_HPP

#include "pastel/gfx/texture/ewaimage_texture.h"
#include "pastel/gfx/texture/linearimage_texture.h"

#include "pastel/math/matrix_determinant.h"
#include "pastel/math/matrix_eigen.h"

#include "pastel/geometry/shape/ellipsoid.h"

namespace Pastel
{

	template <typename Type, int N>
	void EwaImage_Texture<Type, N>::setFilter(
		const FilterPtr& maxFilter,
		const FilterPtr& minFilter)
	{
		ENSURE(maxFilter && minFilter);

		real filterRadius =
			std::max(minFilter->radius(), maxFilter->radius());


		const integer tableSize = filterRadius * 1024;
		real scaling = square(filterRadius) / tableSize;

		std::vector<real> minFilterTable(tableSize);
		std::vector<real> maxFilterTable(tableSize);

		for (integer i = 0;i < tableSize;++i)
		{

			const real t = std::sqrt(i * scaling);

			minFilterTable[i] = minFilter->evaluate(t);
			maxFilterTable[i] = maxFilter->evaluate(t);
		}

		minFilterTable_.swap(minFilterTable);
		maxFilterTable_.swap(maxFilterTable);
		filterTableSize_ = tableSize;
		filterRadius_ = filterRadius;
	}

	template <typename Type, int N>
	Type EwaImage_Texture<Type, N>::operator()(
		const Vector<real, N>& p_,
		const Matrix<real>& m_) const
	{
		// Read ewaimatexture.txt for implementation documentation.

		if (!mipMap_ || mipMap_->empty())
		{
			return Type(0.5);
		}

		integer n = m_.height();

		const Array<Type, N>& mostDetailedImage = mipMap_->mostDetailed();
		Vector<real, N> imageExtent(mostDetailedImage.extent());

		// The derivative vectors along with 'uv' represent an affine
		// transformation from the image plane to the texture plane.


		const Vector<real, N> p(p_ * imageExtent);
		Matrix<real> basis = m_;
		for (integer i = 0;i < n;++i)
		{
			basis.column(i) *= imageExtent * filterRadius_;
		}

		// Find the ellipse that is obtained by applying
		// that affine transformation to a unit sphere.

		Matrix<real> quadraticForm =
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

		// NOTE: We don't have a general function to compute the 
		// eigenvalues of a matrix. Until we have that we must
		// restrict to 2D.

		Vector<real, N> eigenValue = symmetricEigenValues(quadraticForm);

		// The eigenvalues are returned in ascending order.

		real majorAxisLength2 = inverse(eigenValue[0]);
		real minorAxisLength2 = inverse(eigenValue[eigenValue.size() - 1]);

		real eccentricity2 = majorAxisLength2/ minorAxisLength2;

		real MaxEccentricity2 = square(30);
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

			Vector<real, N> aMajorAxisCandidate(
				quadraticForm(1, 0), eigenValue[0] - quadraticForm(0, 0));
			Vector<real, N> bMajorAxisCandidate(
				eigenValue[0] - quadraticForm(1, 1), quadraticForm(1, 0));

			Vector<real, N> majorAxis;

			if (manhattanNorm(aMajorAxisCandidate) <
				manhattanNorm(bMajorAxisCandidate))
			{
				majorAxis = bMajorAxisCandidate;
			}
			else
			{
				majorAxis = aMajorAxisCandidate;
			}

			majorAxis /= norm(majorAxis);

			Vector<real, N> minorAxis = cross(majorAxis);


			minorAxisLength2 *= (eccentricity2 / MaxEccentricity2);

			minorAxis *= std::sqrt(minorAxisLength2);
			majorAxis *= std::sqrt(majorAxisLength2);

			// The quadratic form has to be recomputed since
			// the minor axis has changed.

			quadraticForm = ellipsoidQuadraticForm(
				matrix2x2<real>(minorAxis, majorAxis));
		}

		// Select an appropriate mipmap level.
		// We want the minor axis to cover at least 'pixelsPerMinorAxis' pixels per 'minorAxis' length
		// in the more detailed image (note that it is half of the minor diameter).

		real invLn2 = inverse(constantLn2<real>());
		real pixelsPerMinorAxis = 2;

		const real level = 0.5 * std::log(minorAxisLength2 /
			square(pixelsPerMinorAxis * filterRadius_)) * invLn2;

		//return Type(level / (mipMap_->levels() - 1));

		AlignedBoxD bound =
			ellipsoidBoundingAlignedBox(quadraticForm) + p;

		// We want 'f' to give the value of
		// 'filterTableSize' at the edge of the ellipse.
		// This normalization does that.


		quadraticForm *= filterTableSize_;

		// Compute filter transition coefficient.

		real transitionBegin = 0;
		real transitionEnd = 0.15;
		real transitionWidth = transitionEnd - transitionBegin;
		real normalizedLevel = level / (real)(mipMap_->levels() - 1);
		real tTransition = clamp((normalizedLevel - transitionBegin) / transitionWidth,
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

		integer detailLevel = std::floor(level);

		const Array<Type, 2>& detailImage = (*mipMap_)(detailLevel);
		Type detailSample =
			sampleEwa(p, quadraticForm, bound, (real)1 / (1 << detailLevel), tTransition,
			detailImage);

		integer coarseLevel = detailLevel + 1;

		const Array<Type, 2>& coarseImage = (*mipMap_)(coarseLevel);
		Type coarseSample =
			sampleEwa(p, quadraticForm, bound, (real)1 / (1 << coarseLevel), tTransition,
			coarseImage);

		return linear(detailSample, coarseSample, level - detailLevel);
	}

	template <typename Type, int N>
	Type EwaImage_Texture<Type, N>::sampleEwa(
		const Vector<real, N>& p,
		const Matrix<real>& quadraticForm,
		const AlignedBoxD& bound,
		real scaling,
		real tTransition,
		const Array<Type, N>& image) const
	{
		// Read ewaimatexture.txt for implementation documentation.

		AlignedBox<integer, N> window(
			floor(bound.min() * scaling),
			ceil(bound.max() * scaling));

		// Compute start values.

		const Vector<real, N> pStart(Vector<real, N>(window.min()) + 0.5 - p * scaling);

		real formScaling = inverse(square(scaling));


		real fLeft = dot(pStart * quadraticForm, pStart) * formScaling;
		Vector<real, N> dfLeft = (2 * (pStart * quadraticForm) + diagonal(quadraticForm)) * formScaling;
		const Matrix<real> ddf = (2 * formScaling) * quadraticForm;

		Type imageSum(0);
		real weightSum = 0;

		for (integer i = window.min().y();i < window.max().y();++i)
		{
			real f = fLeft;
			real dfDu = dfLeft.x();

			for (integer j = window.min().x();j < window.max().x();++j)
			{
				real fClamped = f < 0 ? 0 : f;

				if (fClamped < filterTableSize_)
				{
					real weight = linear(maxFilterTable_[fClamped],
						minFilterTable_[fClamped], tTransition);


					imageSum += weight * extender_(image, Vector<integer, N>(j, i));
					weightSum += weight;
				}

				f += dfDu;
				dfDu += ddf(0, 0);
			}

			fLeft += dfLeft.y();
			dfLeft += ddf.cColumn(1);
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
