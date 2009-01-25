#ifndef PASTELGFX_RESAMPLE_HPP
#define PASTELGFX_RESAMPLE_HPP

#include "pastel/gfx/resample.h"
#include "pastel/gfx/drawing.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/lineararray.h"
#include "pastel/sys/permutedview.h"
#include "pastel/sys/view_tools.h"

#include "pastel/sys/mathfunctions.h"

#include <vector>

namespace Pastel
{

	template <
		typename Input_Element,
		typename Input_View,
		typename Output_Element,
		typename Output_View>
	void resampleHorizontal(
		const ConstView<2, Input_Element, Input_View>& input,
		const ConstIndexExtenderRef& indexExtender,
		const ConstFilterRef& filter,
		const View<2, Output_Element, Output_View>& output)
	{
		const integer inputWidth = input.width();
		const integer inputHeight = input.height();
		const integer outputWidth = output.width();
		const integer outputHeight = output.height();

		ENSURE2(outputHeight == inputHeight, outputHeight, inputHeight);

		if (inputWidth == 0 || inputHeight == 0 ||
			outputWidth == 0 || outputHeight == 0)
		{
			return;
		}

		if (outputWidth == inputWidth)
		{
			copy(input, output);
			return;
		}

		const real xStep = (real)inputWidth / outputWidth;
		const real filterFactor = (xStep > 1) ? xStep : 1;
		const real invFilterFactor = inverse(filterFactor);

		const real filterRadius = filter->radius() * filterFactor;

		std::vector<real> weight;
		weight.reserve((integer)filterRadius * 2 + 1);
		std::vector<real> wrappedIndex;
		wrappedIndex.reserve((integer)filterRadius * 2 + 1);

		real xFilter = 0.5 * xStep;

		for (integer x = 0;x < outputWidth;++x)
		{
			// Compute the filter extent.
            // For justification, 
            // read proof.txt: "active pixels"

			const integer rangeBegin =
				toPixelSpanPoint(xFilter - filterRadius);
			const integer rangeEnd =
				toPixelSpanPoint(xFilter + filterRadius);
			const integer rangeWidth = rangeEnd - rangeBegin;

			// Compute the filter weights.

			weight.clear();

			real sumWeights(0);
			for (integer i = rangeBegin;i < rangeEnd;++i)
			{
				// Each pixel is surrounded with
				// a reconstruction filter function.
				// We add up all contributions
				// that cross our current sampling
				// position.

				const real value(
					(*filter)((xFilter - (i + 0.5)) *
					invFilterFactor));
				sumWeights += value;
				weight.push_back(value);
			}

			// Normalize the weights to sum to one.

			const real invSumWeights = inverse(sumWeights);
			for (integer i = 0;i < rangeWidth;++i)
			{
				weight[i] *= invSumWeights;
			}

			// Compute wrapped indices.

			wrappedIndex.clear();

			for (integer i = rangeBegin;i < rangeEnd;++i)
			{
				wrappedIndex.push_back(
					(*indexExtender)(i, inputWidth));
			}

			// Resample the current column.

			for (integer y = 0;y < inputHeight;++y)
			{
				Output_Element result(0);
				for (integer i = 0; i < rangeWidth;++i)
				{
					result += weight[i] *
						*input.constCursor(wrappedIndex[i], y);
				}

				*output.cursor(x, y) = result;
			}

			xFilter += xStep;
		}
	}

	template <
		typename Input_Element,
		typename Input_View,
		typename Output_Element,
		typename Output_View>
		void resample(
		const ConstView<2, Input_Element, Input_View>& input,
		const ArrayExtender<2, PASTEL_NO_DEDUCTION(Input_Element)>& arrayExtender,
		const ConstFilterRef& filter,
		const View<2, Output_Element, Output_View>& output)
	{
		resample(
			input,
			arrayExtender, 
			filter, filter,
			output);
	}

	template <
		typename Input_Element,
		typename Input_View,
		typename Output_Element,
		typename Output_View>
		void resample(
		const ConstView<2, Input_Element, Input_View>& input,
		const ArrayExtender<2, PASTEL_NO_DEDUCTION(Input_Element)>& arrayExtender,
		const ConstFilterRef& xFilter,
		const ConstFilterRef& yFilter,
		const View<2, Output_Element, Output_View>& output)
	{
		const integer inputWidth = input.width();
		const integer inputHeight = input.height();
		const integer outputWidth = output.width();
		const integer outputHeight = output.height();

		const integer widthDelta = inputWidth - outputWidth;
		const integer heightDelta = inputHeight - outputHeight;

		const integer xRadius = std::ceil(xFilter->radius());
		const integer yRadius = std::ceil(yFilter->radius());

		// The n-dimensional resampling is done as 
		// n subsequent 1-dimensional resamplings.
		// If the filter radii are equal, 
		// then it can be shown (as in proof.txt), that the order
		// which maximizes performance is
		// ascending order in (radius[i] * newExtent[i] / oldExtent[i]).
		// If the filter radii are not equal,
		// then it can be shown that in many cases the same
		// heuristic gives an optimal order but
		// not in all cases. 
		// I suspect that computing an optimal order in this
		// more generalized case is np-hard (and thus
		// needs to be solved by brute force by choosing
		// the minimum of all n! permutations).

		if (xRadius * ((real)outputWidth / inputWidth) < 
			yRadius * ((real)outputHeight / inputHeight))
		{
			// Resample in x.

			LinearArray<2, Output_Element> xResult(outputWidth, inputHeight);

			resampleHorizontal(
				input,
				arrayExtender.extender(0),
				xFilter, 
				arrayView(xResult));

			// Resample in y.

			resampleHorizontal(
				constPermutedView(constArrayView(xResult), Integer2(1, 0)), 
				arrayExtender.extender(1), 
				yFilter, 
				permutedView(output, Integer2(1, 0)));
		}
		else
		{
			// Resample in y.

			LinearArray<2, Output_Element> yResult(inputWidth, outputHeight);

			resampleHorizontal(
				constPermutedView(input, Integer2(1, 0)), 
				arrayExtender.extender(1), 
				yFilter, 
				permutedView(arrayView(yResult), Integer2(1, 0)));

			// Resample in x.

			resampleHorizontal(
				constArrayView(yResult), 
				arrayExtender.extender(0), 
				xFilter, 
				output);
		}
	}

}

#endif
