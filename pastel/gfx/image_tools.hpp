#ifndef PASTELGFX_IMAGE_TOOLS_HPP
#define PASTELGFX_IMAGE_TOOLS_HPP

#include "pastel/gfx/image_tools.h"

namespace Pastel
{

	template <int N>
	ConstView<N, Color, ConstAdaptedView<N, Rgb888Adapter, ConstArrayView<N, Array<N, uint32> > > >
		constRgb888View(const Array<N, uint32>& image)
	{
		return constAdaptedView(constArrayView(image),
			Rgb888Adapter());
	}

	template <int N>
	View<N, Color, AdaptedView<N, Rgb888Adapter, ArrayView<N, Array<N, uint32> > > >
		rgb888View(Array<N, uint32>& image)
	{
		return adaptedView(arrayView(image), Rgb888Adapter());
	}

	namespace Detail_ScaleGrayScaleImage
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
				const Type delta = max_ - min_;
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

		Detail_ScaleGrayScaleImage::MinMaxVisitor<real32> minMaxVisitor(min, max);

		visit(image, minMaxVisitor);

		Detail_ScaleGrayScaleImage::ScaleVisitor<real32> scaleVisitor(min, max);

		visit(image, scaleVisitor);
	}

}

#endif
