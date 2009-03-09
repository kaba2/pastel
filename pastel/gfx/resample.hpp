#ifndef PASTELGFX_RESAMPLE_HPP
#define PASTELGFX_RESAMPLE_HPP

#include "pastel/gfx/resample.h"
#include "pastel/gfx/drawing.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/array.h"
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
	void resample(
		const ConstView<1, Input_Element, Input_View>& input,
		const PASTEL_NO_DEDUCTION((ArrayExtender<1, Input_Element>))& arrayExtender,
		const ConstFilterRef& filter,
		const View<1, Output_Element, Output_View>& output)
	{
		const integer inputWidth = input.width();
		const integer outputWidth = output.width();

		if (inputWidth == 0 ||
			outputWidth == 0)
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

		real xFilter = 0.5 * xStep;

		for (integer x = 0;x < outputWidth;++x)
		{
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

			// Compute the resampled value.

			Output_Element result(0);
			for (integer i = 0; i < rangeWidth;++i)
			{
				result += weight[i] *
					arrayExtender(input, i + rangeBegin);
			}

			output(x) = result;

			xFilter += xStep;
		}
	}

	namespace Detail_Resample
	{

		template <typename Input_Element>
		class ResampleFunctor
		{
		public:
			ResampleFunctor(
				const ArrayExtender<1, Input_Element>& arrayExtender,
				const ConstFilterRef& filter)
			: arrayExtender_(arrayExtender)
			, filter_(filter)
			{
			}

			template <typename Left_View, typename Right_View>
			void operator()(const Left_View& left,
				const Right_View& right) const
			{
				resample(left, arrayExtender_, filter_, right);
			}

		private:
			const ArrayExtender<1, Input_Element>& arrayExtender_;
			const ConstFilterRef& filter_;
		};

		class CostId
		{
		public:
			CostId(real cost,
				integer id)
				: cost_(cost)
				, id_(id)
			{
			}

			bool operator<(const CostId& that) const
			{
				if (cost_ < that.cost_)
				{
					return true;
				}
				if (cost_ > that.cost_)
				{
					return false;
				}
				return id_ < that.id_;
			}

			real cost_;
			integer id_;
		};

	}

	template <
		typename Computation_Element,
		int N,
		typename Input_Element,
		typename Input_View,
		typename Output_Element,
		typename Output_View>
		typename boost::enable_if_c<(N > 1), void>::type
		resample(
		const ConstView<N, Input_Element, Input_View>& input,
		const PASTEL_NO_DEDUCTION((ArrayExtender<N, Input_Element>))& arrayExtender,
		const ConstFilterRef& filter,
		const View<N, Output_Element, Output_View>& output)
	{
		// The n-dimensional resampling is done as
		// n subsequent 1-dimensional resamplings.
		// If the filter radii are equal,
		// then it can be shown (as in proof.txt), that the order
		// which maximizes performance is
		// ascending order in (radius[i] * newExtent[i] / oldExtent[i]).
		// If the filter radii are not equal,
		// then it can be shown that the same
		// heuristic gives an optimal order in many, but not all, cases.
		// I suspect that computing an optimal order in this
		// more generalized case is np-hard (and thus
		// needs to be solved by brute force by choosing
		// the minimum of all N! permutations).

		BOOST_STATIC_ASSERT(N != 1);

		// Find out the mostly-optimal order.

		SmallSet<Detail_Resample::CostId> orderSet;
		for (integer i = 0;i < N;++i)
		{
			orderSet.insert(
				Detail_Resample::CostId((filter->radius() * output.extent()[i]) 
				/ input.extent()[i], i));
		}

		// The first resampling is from the input view to
		// a temporary array.

		Vector<N, integer> extent = input.extent();
		extent[orderSet[0].id_] = output.extent()[orderSet[0].id_];
		Array<N, Computation_Element> tempArray(extent);

		{
			const ArrayExtender<1, Input_Element> arrayExtender1D(
				arrayExtender.extender(0), arrayExtender.border());
			const Detail_Resample::ResampleFunctor<Input_Element> resampleFunctor(
				arrayExtender1D, filter);
			visitRows(input, arrayView(tempArray), orderSet[0].id_, resampleFunctor);
		}

		// The second resampling to the previous-to-last resampling
		// is done between temporary arrays.

		for (integer i = 1;i < N - 1;++i)
		{
			extent[orderSet[i].id_] = output.extent()[orderSet[i].id_];
			Array<N, Computation_Element> tempArray2(extent);

			const ArrayExtender<1, Computation_Element> arrayExtender1D(
				arrayExtender.extender(i), arrayExtender.border());
			const Detail_Resample::ResampleFunctor<Computation_Element> resampleFunctor(
				arrayExtender1D, filter);

			visitRows(constArrayView(tempArray), arrayView(tempArray2), 
				orderSet[i].id_, resampleFunctor);

			tempArray.swap(tempArray2);
		}

		// The last resampling is done to the output view.

		{
			const ArrayExtender<1, Computation_Element> arrayExtender1D(
				arrayExtender.extender(N - 1), arrayExtender.border());
			const Detail_Resample::ResampleFunctor<Computation_Element> resampleFunctor(
				arrayExtender1D, filter);

			visitRows(constArrayView(tempArray), output, orderSet[N - 1].id_, resampleFunctor);
		}
	}

}

#endif
