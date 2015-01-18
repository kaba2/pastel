#ifndef PASTELGFX_CONVOLUTION_HPP
#define PASTELGFX_CONVOLUTION_HPP

#include "pastel/gfx/image_processing/convolution.h"
#include "pastel/gfx/texture/nearestimage_texture.h"
#include "pastel/gfx/color/colormixer/additive_colormixer.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/view/borderedconstview.h"
#include "pastel/sys/view/transformedview.h"

namespace Pastel
{

	namespace Convolute_
	{

		template <typename Input, typename Output>
		class Transform
		{
		public:
			using Input_ = Input;
			using Output_ = Output;

			explicit Transform(const Output& factor)
				: factor_(factor)
			{
			}

			Output operator()(const Input& input) const
			{
				return input * factor_;
			}

		private:
			Output factor_;
		};

		template <
			int N,
			typename Input_RingElement,
			typename Input_ConstView,
			typename Filter_RingElement,
			typename Filter_ConstView,
			typename Output_RingElement,
			typename Output_View,
			typename ConvoluteProcessFunctor>
		class ConvoluteFunctor
		{
		public:
			ConvoluteFunctor(
				const ConstView<N, Input_RingElement, Input_ConstView>& inputView,
				const ConstView<N, Filter_RingElement, Filter_ConstView>& filterView,
				const View<N, Output_RingElement, Output_View>& outputView,
				const ConvoluteProcessFunctor& processFunctor)
				: inputView_(inputView)
				, filterView_(filterView)
				, outputView_(outputView)
				, processFunctor_(processFunctor)
			{
			}

			void operator()(
				const Vector<integer, N>& position,
				const Input_RingElement& factor) const
			{
				if (processFunctor_(factor))
				{
					drawView(
						constTransformedView(filterView_,
						Transform<Filter_RingElement, Output_RingElement>(factor)),
						evaluate(position - filterView_.extent() / 2),
						outputView_,
						additiveColorMixer<Output_RingElement>());
				}
			}

		private:
			ConstView<N, Input_RingElement, Input_ConstView> inputView_;
			ConstView<N, Filter_RingElement, Filter_ConstView> filterView_;
			View<N, Output_RingElement, Output_View> outputView_;
			ConvoluteProcessFunctor processFunctor_;
		};

	}

	template <
		int N,
		typename Input_RingElement,
		typename Input_ConstView,
		typename Filter_RingElement,
		typename Filter_ConstView,
		typename Output_RingElement,
		typename Output_View,
		typename ConvoluteProcessFunctor>
		void convolute(
		const ConstView<N, Input_RingElement, Input_ConstView>& inputView,
		const ConstView<N, Filter_RingElement, Filter_ConstView>& filterView,
		const View<N, Output_RingElement, Output_View>& outputView,
		const ConvoluteProcessFunctor& processFunctor)
	{
		// Convolution performance:
		//
		// A 5000 x 5000 float image with 100000 random pixels set to 1 and
		// other pixels 0 with a 21 x 21 filterView.
		//
		// Absolute addressing means to address views as (x, y, ..).
		// Relative addressing means to move from a pixel to its neighbour.
		//
		// 245s shooting, absolute addressing
		// 1.6s shooting, absolute addressing, zero optimization
		// 106s gathering, relative addressing
		// 55s shooting, relative addressing
		// 0.4s shooting, relative addressing, zero optimization
		//
		// - Gathering takes about 2x time compared to shooting.
		// - Shooting can use zero optimization. This brings dramatic speedups for sparse
		//   convolutions and adds negligible time for dense convolutions.
		// - Absolute addressing takes about 4x time compared to relative addressing.
		// - Clamping the filterView to the viewport takes negligible time (i.e. it does
		//   not pay to convolute the central area specially without clamping).

		ENSURE(inputView.extent() == outputView.extent());

		Convolute_::ConvoluteFunctor<N, Input_RingElement, Input_ConstView, Filter_RingElement,
			Filter_ConstView, Output_RingElement, Output_View, ConvoluteProcessFunctor>
			convoluteFunctor(inputView, filterView, outputView, processFunctor);

		visitPosition(inputView, convoluteFunctor);
	}

	namespace Convolute_
	{

		template <typename Type>
		class SkipZero
		{
		public:
			bool operator()(const Type& that) const
			{
				return !(that == Type(0));
			}
		};

	}

	template <
		int N,
		typename Input_RingElement,
		typename Input_ConstView,
		typename Filter_RingElement,
		typename Filter_ConstView,
		typename Output_RingElement,
		typename Output_View>
		void convolute(
		const ConstView<N, Input_RingElement, Input_ConstView>& inputView,
		const ConstView<N, Filter_RingElement, Filter_ConstView>& filterView,
		const View<N, Output_RingElement, Output_View>& outputView)
	{
		Convolute_::SkipZero<Input_RingElement> skipZero;

		Pastel::convolute(inputView, filterView, outputView, skipZero);
	}

	namespace GeneralizedConvolute_
	{

		template <
			typename Input_Element,
			typename Output_Element,
			typename Texture_Element>
		class ColorMixer
		{
		public:
			explicit ColorMixer(
				const Input_Element& input)
				: input_(input)
			{
			}

			Output_Element operator()(
				const Output_Element& image,
				const Texture_Element& texture) const
			{
				return image + texture * input_;
			}

		private:
			Input_Element input_;
		};

	}

	template <
		typename Input_Element,
		typename Input_ConstView,
		typename Filter_Element,
		typename Filter_ConstView,
		typename Radius_Element,
		typename Radius_ConstView,
		typename Output_Element,
		typename Output_View,
		typename ConvoluteProcessFunctor>
		void generalizedConvolute(
		const ConstView<2, Input_Element, Input_ConstView>& inputView,
		const ConstView<2, Filter_Element, Filter_ConstView>& filterView,
		const ConstView<2, Radius_Element, Radius_ConstView>& radiusView,
		const View<2, Output_Element, Output_View>& outputView,
		ConvoluteProcessFunctor& processFunctor)
	{
		integer width = inputView.width();
		integer height = inputView.height();

		ENSURE2(width == outputView.width(), width, outputView.width());
		ENSURE2(height == outputView.height(), height, outputView.height());

		using InputCursor = typename Input_ConstView::ConstCursor;
		using FilterCursor = typename Filter_ConstView::ConstCursor;
		using RadiusCursor = typename Radius_ConstView::ConstCursor;
		using OutputCursor = typename Output_View::Cursor;

		AlignedBox2 outputBound(0, 0, outputView.width(), outputView.height());
		AlignedBox2 textureBox(0, 0, filterView.width(), filterView.height());

		RadiusCursor yRadiusCursor = radiusView.constCursor(0, 0);
		InputCursor yInputCursor = inputView.constCursor(0, 0);

		using FilterView = ConstView<2, Filter_Element, Filter_ConstView>;
		using BorderedFilterView = BorderedConstView<2, FilterView>;
		using ExtendedFilterView = ExtendedConstView<2, Filter_Element, BorderedFilterView>;

		NearestImage_Texture<ExtendedFilterView> textureSampler = nearestImageTexture(
			borderedConstView(filterView, 0));

		for (integer y = 0;y < height;++y)
		{
			RadiusCursor xyRadiusCursor = yRadiusCursor;
			InputCursor xyInputCursor = yInputCursor;

			for (integer x = 0;x < width;++x)
			{

				const Input_Element height = *xyInputCursor;
				if (processFunctor(height))
				{
					const Radius_Element radius = *xyRadiusCursor;

					GeneralizedConvolute_::
						ColorMixer<Input_Element, Output_Element, Filter_Element>
						colorMixer(height);

					drawTexturedBox(
						AlignedBox2(x - radius, y - radius, x + radius, y + radius),
						textureSampler,
						outputView,
						colorMixer);
				}

				xyRadiusCursor.xIncrement();
				xyInputCursor.xIncrement();
			}

			yRadiusCursor.yIncrement();
			yInputCursor.yIncrement();
		}
	}

	template <
		typename Input_Element,
		typename Input_ConstView,
		typename Filter_Element,
		typename Filter_ConstView,
		typename Radius_Element,
		typename Radius_ConstView,
		typename Output_Element,
		typename Output_View>
		void generalizedConvolute(
		const ConstView<2, Input_Element, Input_ConstView>& inputView,
		const ConstView<2, Filter_Element, Filter_ConstView>& filterView,
		const ConstView<2, Radius_Element, Radius_ConstView>& radiusView,
		const View<2, Output_Element, Output_View>& outputView)
	{
		Convolute_::SkipZero<Input_Element> skipZero;

		Pastel::generalizedConvolute(inputView, filterView,
			radiusView, outputView, skipZero);
	}

}

#endif
