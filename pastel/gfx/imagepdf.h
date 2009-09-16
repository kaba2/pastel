// Description: Random draws from a pdf described by a scalar array

#ifndef PASTEL_IMAGEPDF_H
#define PASTEL_IMAGEPDF_H

#include "pastel/sys/view.h"
#include "pastel/sys/vector.h"

#include <vector>

namespace Pastel
{

	//! Returns random pixel positions distributed in accordance to an image.

	template <int N, typename Real>
	class ImagePdf
	{
	public:
		template <typename Image_ConstView>
		explicit ImagePdf(
			const ConstView<N, Real, Image_ConstView>& image);

		Vector<integer, N> operator()() const;

	private:
		class CdfElement
		{
		public:
			CdfElement(
				const Real& cdf,
				const Vector<integer, N>& position)
				: cdf_(cdf)
				, position_(position)
			{
			}

			bool operator<(const CdfElement& that) const
			{
				return cdf_ < that.cdf_;
			}

			const Real& cdf() const
			{
				return cdf_;
			}

			const Vector<integer, N>& position() const
			{
				return position_;
			}

		private:
			Real cdf_;
			Vector<integer, N> position_;
		};

		class CdfFunctor
		{
		public:
			CdfFunctor(
				Real& imageSum,
				std::vector<CdfElement>& cdf)
				: imageSum_(imageSum)
				, cdf_(cdf)
			{
			}

			void operator()(const Vector<integer, N>& position,
				Real imageValue) const
			{
				if (imageValue > 0)
				{
					imageSum_ += imageValue;

					cdf_.push_back(CdfElement(imageSum_, position));
				}
			}

		private:
			Real& imageSum_;
			std::vector<CdfElement>& cdf_;
		};

		template <typename Image_ConstView>
		void computeCdf(
			const ConstView<N, Real, Image_ConstView>& image);

		std::vector<CdfElement> cdf_;
	};

}

#include "pastel/gfx/imagepdf.hpp"

#endif
