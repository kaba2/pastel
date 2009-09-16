#ifndef PASTEL_SUMMEDAREATABLE_HPP
#define PASTEL_SUMMEDAREATABLE_HPP

#include "pastel/gfx/summedareatable.h"
#include "pastel/gfx/bilinearimagetexture.h"

#include "pastel/sys/view_visit.h"
#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	namespace Detail_ComputeSummedAreaTable
	{

		template <int N, typename Type, typename Image_ConstView, typename Sum_View>
		class Visitor
		{
		public:
			Visitor(
				const ConstView<N, Type, Image_ConstView>& image,
				const View<N, Type, Sum_View>& sumImage)
				: image_(image)
				, sumImage_(sumImage)
			{
			}

			void operator()(
				const Vector<integer, N>& position,
				const Type& imageElement, Type& sumImageElement) const
			{
				Type sum = imageElement;

				for (integer i = 0;i < N;++i)
				{
					if (position[i] > 0)
					{
						sum += sumImage_(position - unitAxis<integer, N>(i));
					}
				}

				if (allGreater(position, 0))
				{
					sum -= (N - 1) * sumImage_(position - 1);
				}

				sumImageElement = sum;
			}

		private:
			const ConstView<N, Type, Image_ConstView>& image_;
			const View<N, Type, Sum_View>& sumImage_;
		};

	}

	template <int N, typename Type, typename Image_ConstView, typename Sum_View>
	void computeSummedAreaTable(
		const ConstView<N, Type, Image_ConstView>& image,
		const View<N, Type, Sum_View>& sumImage)
	{
		Detail_ComputeSummedAreaTable::Visitor<N, Type, Image_ConstView, Sum_View>
			visitor(image, sumImage);
		visitPosition(image, sumImage, visitor);
	}

	template <typename Image_Element, typename Image_ConstView>
	Image_Element summedAreaTable(
		const ExtendedConstView<2, Image_Element, Image_ConstView>& sumImage,
		const AlignedBox2& region)
	{
		ENSURE(false);
		return Image_Element();
		/*
		const Vector2 newMin = region.min() - 1;
		const Vector2 newMax = region.max() - 1;

		Image_Element sum = sampleBilinear(newMax, sumImage);

		if (newMin.x() >= 0)
		{
			sum -= sampleBilinear(Vector2(newMin.x(), newMax.y()), sumImage);
		}
		if (newMin.y() >= 0)
		{
			sum -= sampleBilinear(Vector2(newMax.x(), newMin.y()), sumImage);
		}

		if (newMin.x() >= 0 && newMin.y() >= 0)
		{
			sum += sampleBilinear(newMin, sumImage);
		}

		return sum;
		*/
	}

}

#endif
