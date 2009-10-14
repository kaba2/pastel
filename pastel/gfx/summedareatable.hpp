#ifndef PASTEL_SUMMEDAREATABLE_HPP
#define PASTEL_SUMMEDAREATABLE_HPP

#include "pastel/gfx/summedareatable.h"
#include "pastel/gfx/linearimage_texture.h"

#include "pastel/sys/view_visit.h"
#include "pastel/sys/vector_tools.h"
#include "pastel/sys/rectangle_tools.h"

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
				const integer n = position.size();

				Type sum = imageElement;

				for (integer i = 0;i < n;++i)
				{
					if (position[i] > 0)
					{
						sum += sumImage_(position - unitAxis<integer, N>(n, i));
					}
				}

				if (allGreater(position, 0))
				{
					sum -= (n - 1) * sumImage_(position - 1);
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

	template <typename Type, typename Sum_ConstView>
	Type summedAreaTable(
		const ConstView<2, Type, Sum_ConstView>& sumImage,
		const Rectangle2& region)
	{
		if (anyEqual(sumImage.extent(), 0) ||
			anyLess(region.max(), 0))
		{
			return 0;
		}

		const Rectangle2 equalRegion(
			min(region.min(), sumImage.extent() - 1) - 1,
			min(region.max(), sumImage.extent()) - 1);

		Type sum = sumImage(equalRegion.max());

		if (equalRegion.min().x() >= 0)
		{
			sum -= sumImage(equalRegion.min().x(), equalRegion.max().y());
		}
		if (equalRegion.min().y() >= 0)
		{
			sum -= sumImage(equalRegion.max().x(), equalRegion.min().y());
		}
		if (equalRegion.min().x() >= 0 && equalRegion.min().y() >= 0)
		{
			sum += sumImage(equalRegion.min());
		}

		return sum;
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

		Image_Element sum = sampleLinear(newMax, sumImage);

		if (newMin.x() >= 0)
		{
			sum -= sampleLinear(Vector2(newMin.x(), newMax.y()), sumImage);
		}
		if (newMin.y() >= 0)
		{
			sum -= sampleLinear(Vector2(newMax.x(), newMin.y()), sumImage);
		}

		if (newMin.x() >= 0 && newMin.y() >= 0)
		{
			sum += sampleLinear(newMin, sumImage);
		}

		return sum;
		*/
	}

}

#endif
