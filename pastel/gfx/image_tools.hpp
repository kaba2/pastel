#ifndef PASTELGFX_IMAGE_TOOLS_HPP
#define PASTELGFX_IMAGE_TOOLS_HPP

#include "pastel/gfx/image_tools.h"

#include "pastel/gfx/color_tools.h"
#include "pastel/sys/mytypes.h"
#include "pastel/sys/constants.h"

namespace Pastel
{

	inline void indexedImageToColorImage(
		const Array<uint8, 2>& indexedImage,
		const std::vector<Color>& palette,
		Array<Color, 2>& result)
	{
		ENSURE_OP(palette.size(), ==, 256);

		integer width = indexedImage.width();
		integer height = indexedImage.height();

		Array<Color, 2> colorImage(Vector2i(width, height));
		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				colorImage(x, y) = palette[indexedImage(x, y)];
			}
		}

		colorImage.swap(result);
	}

	template <int N>
	ConstView<N, Color, ConstAdaptedView<N, Rgb888Adapter, ConstArrayView<N, Array<uint32, N> > > >
		constRgb888View(const Array<uint32, N>& image)
	{
		return constAdaptedView(constArrayView(image),
			Rgb888Adapter());
	}

	template <int N>
	View<N, Color, AdaptedView<N, Rgb888Adapter, ArrayView<N, Array<uint32, N> > > >
		rgb888View(Array<uint32, N>& image)
	{
		return adaptedView(arrayView(image), Rgb888Adapter());
	}

	namespace ScaleGrayScaleImage_
	{

		template <typename Type>
		class MinMaxVisitor
		{
		public:
			MinMaxVisitor(
				Type& min,
				Type& max)
				: min_(min)
				, max_(max)
			{
			}

			void operator()(const Type& that) const
			{
				if (that < min_)
				{
					min_ = that;
				}
				else if (that > max_)
				{
					max_ = that;
				}
			}

		private:
			Type& min_;
			Type& max_;
		};

		template <typename Type>
		class ScaleVisitor
		{
		public:
			ScaleVisitor(
				const Type& min,
				const Type& max)
				: min_(min)
				, max_(max)
			{
			}

			void operator()(Type& that) const
			{
				Type delta = max_ - min_;
				if (delta > 0)
				{
					that -= min_;
					that /= delta;
				}

				that = clamp(that, 0, 1);
			}

		private:
			Type min_;
			Type max_;
		};

	}

	template <int N, typename Image_View>
	void scaleGrayscaleImage(
		const View<N, real32, Image_View>& image)
	{
		real32 min = infinity<real32>();
		real32 max = -infinity<real32>();

		ScaleGrayScaleImage_::MinMaxVisitor<real32> minMaxVisitor(min, max);

		visit(image, minMaxVisitor);

		ScaleGrayScaleImage_::ScaleVisitor<real32> scaleVisitor(min, max);

		visit(image, scaleVisitor);
	}

}

#endif
