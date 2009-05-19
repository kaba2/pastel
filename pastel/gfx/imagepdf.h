#ifndef PASTEL_IMAGEPDF_H
#define PASTEL_IMAGEPDF_H

#include "pastel/sys/view.h"
#include "pastel/sys/point.h"

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

		Point<N, integer> operator()() const;

	private:
		class CdfElement
		{
		public:
			CdfElement(
				const Real& cdf,
				const Point<N, integer>& position)
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

			const Point<N, integer>& position() const
			{
				return position_;
			}

		private:
			Real cdf_;
			Point<N, integer> position_;
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

			void operator()(const Point<N, integer>& position,
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
