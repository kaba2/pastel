#ifndef PASTEL_RESAMPLE_HPP
#define PASTEL_RESAMPLE_HPP

#include "pastel/dsp/resample.h"

#include "pastel/sys/syscommon.h"
#include "pastel/sys/mytypes.h"
#include "pastel/sys/array.h"
#include "pastel/sys/view_tools.h"
#include "pastel/sys/math_functions.h"
#include "pastel/sys/smallset.h"

#include "pastel/sys/math_functions.h"

#include <vector>

namespace Pastel
{

	template <
		typename Computation_Element,
		typename Input_Element,
		typename Input_View,
		typename Output_Element,
		typename Output_View>
	void resampleTable(
		const ConstView<1, Input_Element, Input_View>& input,
		const PASTEL_NO_DEDUCTION((ArrayExtender<1, Input_Element>))& arrayExtender,
		const ConstTableFilterPtr& filter,
		const View<1, Output_Element, Output_View>& output,
		real blurFactor)
	{
		ENSURE_OP(blurFactor, >=, 1);

		const integer inputWidth = input.width();
		const integer outputWidth = output.width();

		if (inputWidth == 0 ||
			outputWidth == 0)
		{
			return;
		}

		/*
		if (outputWidth == inputWidth)
		{
			copy(input, output);
			return;
		}
		*/

		const real xStep = (real)inputWidth / outputWidth;
		const real filterFactor = blurFactor * ((xStep > 1) ? xStep : 1);
		const real invFilterFactor = inverse(filterFactor);

		const real filterRadius = filter->radius() * filterFactor;

		real xFilter = 0.5 * xStep;

		// xFilter + xStep * marginWidth > filterRadius
		// =>
		// xStep * marginWidth > filterRadius - xFilter
		// => 
		// marginWidth > (filterRadius - xFilter) / xStep

		const integer introEnd = 
			clamp(
			(integer)std::ceil((filterRadius - xFilter) / xStep) + 1,
			0, outputWidth);
		const integer mainEnd =
			clamp(outputWidth - introEnd, 
			introEnd, outputWidth);

		for (integer x = 0;x < introEnd;++x)
		{
			const integer rangeBegin =
				toPixelSpanPoint(xFilter - filterRadius);
			const integer rangeEnd =
				toPixelSpanPoint(xFilter + filterRadius);

			real xLocalFilter = 
				(xFilter - (rangeBegin + 0.5)) * invFilterFactor;

			// Compute the resampled value.

			Computation_Element result(0);
			real sumWeights = 0;
			for (integer i = rangeBegin; i < rangeEnd;++i)
			{
				const real weight =
					filter->evaluateInRange(xLocalFilter);
				
				result += weight *
					arrayExtender(input, i);

				xLocalFilter -= invFilterFactor;
				sumWeights += weight;
			}

			output(x) = result / sumWeights;

			xFilter += xStep;
		}

		for (integer x = introEnd;x < mainEnd;++x)
		{
			const integer rangeBegin =
				toPixelSpanPoint(xFilter - filterRadius);
			const integer rangeEnd =
				toPixelSpanPoint(xFilter + filterRadius);

			real xLocalFilter = 
				(xFilter - (rangeBegin + 0.5)) * invFilterFactor;

			// Compute the resampled value.

			Computation_Element result(0);
			real sumWeights = 0;
			for (integer i = rangeBegin; i < rangeEnd;++i)
			{
				const real weight =
					filter->evaluateInRange(xLocalFilter);
				
				Computation_Element in = input(i);
				result += weight * in;

				xLocalFilter -= invFilterFactor;
				sumWeights += weight;
			}

			output(x) = result / sumWeights;

			xFilter += xStep;
		}

		for (integer x = mainEnd;x < outputWidth;++x)
		{
			const integer rangeBegin =
				toPixelSpanPoint(xFilter - filterRadius);
			const integer rangeEnd =
				toPixelSpanPoint(xFilter + filterRadius);

			real xLocalFilter = 
				(xFilter - (rangeBegin + 0.5)) * invFilterFactor;

			// Compute the resampled value.

			Computation_Element result(0);
			real sumWeights = 0;
			for (integer i = rangeBegin; i < rangeEnd;++i)
			{
				const real weight =
					filter->evaluateInRange(xLocalFilter);
				
				result += weight *
					arrayExtender(input, i);

				xLocalFilter -= invFilterFactor;
				sumWeights += weight;
			}

			output(x) = result / sumWeights;

			xFilter += xStep;
		}
	}

	template <
		typename Computation_Element,
		typename Input_Element,
		typename Input_View,
		typename Output_Element,
		typename Output_View>
		void resample(
		const ConstView<1, Input_Element, Input_View>& input,
		const PASTEL_NO_DEDUCTION((ArrayExtender<1, Input_Element>))& arrayExtender,
		const ConstFilterPtr& filter,
		const View<1, Output_Element, Output_View>& output,
		real blurFactor)
	{
		ENSURE_OP(blurFactor, >=, 1);

		Pastel::resampleTable<Computation_Element>(
			input, arrayExtender, 
			tableFilter(filter), output,
			blurFactor);
	}

	namespace Detail_Resample
	{

		template <typename Computation_Element, typename Input_Element>
		class ResampleFunctor
		{
		public:
			ResampleFunctor(
				const ArrayExtender<1, Input_Element>& arrayExtender,
				const ConstTableFilterPtr& filter,
				real blurFactor)
			: arrayExtender_(arrayExtender)
			, filter_(filter)
			, blurFactor_(blurFactor)
			{
			}

			template <typename Left_View, typename Right_View>
			void operator()(const Left_View& left,
				const Right_View& right) const
			{
				resample<Computation_Element>(left, arrayExtender_, 
					filter_, right, blurFactor_);
			}

		private:
			const ArrayExtender<1, Input_Element>& arrayExtender_;
			const ConstTableFilterPtr& filter_;
			const real blurFactor_;
		};

		class AxisValue
		{
		public:
			AxisValue(real value,
				integer id)
				: value_(value)
				, axis_(id)
			{
			}

			bool operator<(const AxisValue& that) const
			{
				if (value_ < that.value_)
				{
					return true;
				}
				if (value_ > that.value_)
				{
					return false;
				}
				return axis_ < that.axis_;
			}

			real value_;
			integer axis_;
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
		resampleTable(
		const ConstView<N, Input_Element, Input_View>& input,
		const PASTEL_NO_DEDUCTION((ArrayExtender<N, Input_Element>))& arrayExtender,
		const ConstTableFilterPtr& filter,
		const View<N, Output_Element, Output_View>& output,
		real blurFactor)
	{
		ENSURE_OP(blurFactor, >=, 1);

		// The n-dimensional resampling is done as
		// n subsequent 1-dimensional resamplings.
		// If the filter radii are equal,
		// then it can be shown (as we will shortly) that the order
		// which maximizes performance is
		// ascending order in (radius[i] * newExtent[i] / oldExtent[i]).
		// If the filter radii are not equal,
		// then it can be shown that the same
		// heuristic gives an optimal order in many, but not all, cases.
		// I suspect that computing an optimal order in this
		// more generalized case is np-hard (and thus
		// needs to be solved by brute force by choosing
		// the minimum of all N! permutations).

		/*
		Resampling n-dimensional arrays in optimal order
		================================================

		Problem
		-------

		The resampling of an n-dimensional array can be carried away
		by n subsequent 1-dimensional resamplings.
		What is the optimal order to carry out these 1-d resamplings
		to maximize efficiency?

		Formal problem statement
		------------------------

		Consider you have an n-dimensional array of real numbers. In each 
		dimension i there are extent[i] elements ('extent' in Z^n). So for 
		example with n = 1 and extent = (20) you would have a 20-element vector, 
		with n = 2 and extent = (4, 2) you would have a 2 x 4 matrix (extent is 
		given in 'xyz...' order), etc..

		You want to resize this array in all dimensions (given by 'newExtent' in 
		Z^n). You do this by resizing each dimension i at a time. Each resizing 
		operation is associated with a cost:

		C[i] = volume(extent), if newExtent[i] < extent[i]
			   volume(extent) * (newExtent[i] / extent[i]), otherwise

		And after each resize operation 'extent' is updated:
		extent[i] := newExtent[i]

		Let P in Z^n be a permutation of (1,2, ..., n - 1, n).
		The cost function can be given as a recursive formula as follows:

		C[1] = volume(extent), if newExtent[P[i]] < extent[P[i]]
			   volume(extent) * (newExtent[P[i]] / extent[P[i]]), otherwise
		       
		C[i] = C[i - 1] * (newExtent[P[i - 1]] / extent[P[i - 1]]), if newExtent[P[i]] < extent[P[i]]
			   C[i - 1] * (newExtent[P[i - 1]] / extent[P[i - 1]]) * (newExtent[P[i]] / extent[P[i]]), otherwise

		Ctot = sum(C[i])

		The problem is to construct an algorithm that outputs a P 
		which is a permutation of (1,2, ..., n - 1, n) such that Ctot is minimal.

		Solution
		--------

		The following is a quote from James Waldby, from
		a newsgroup post in sci.math:

		- begin quote -
		In any P consider two consecutive entries P[i], P[i+1] = a,b.  
		Note that the cost of operations 1...i-1 and i+2...n do not 
		depend on whether the order of those two entries is a,b or b,a.

		Use an O(n lg n) sort on the ratio newExtent[i]/extent[i].

		To verify that this sort leads to a global min, observe that 
		when given the choice of changing extent element a before or 
		after b, you can write out expressions for the two costs in 
		three possible cases: a,b both decreasing; both increasing; 
		or one increases, one decreases.  For example, if both increase,
		say from p to q and r to s respectively,
		  C1 = H + (p / q)V + (r / s)(p / q)V + T,
		  C2 = H + (r / s)V + (p / q)(r / s)V + T,
		where H is cost incurred in earlier steps, V is volume after
		those steps, and T is cost in later steps.  Clearly, C1 < C2
		iff p / q < r / s (for positive p, q, r, s).  The same criterion 
		arises in the other two cases also.  Thus, sorting the ratios 
		into ascending order gives a global min, as follows:  Suppose 
		a claimed-optimal vector P has consecutive operations that change
		p -> q and then r -> s, such that p / q > r / s.  Reversing those 
		operations gives a smaller cost.  Therefore P was not optimal.
		- end quote -

		Let us prove the claims. 

		Assume upsampling + upsampling
		(p / q >= 1 and r / s >= 1), then:

		C1 < C2 
		<=>
		H + (p / q)V + (r / s)(p / q)V + T < H + (r / s)V + (p / q)(r / s)V + T
		<=>
		(p / q)V + (r / s)(p / q)V < (r / s)V + (p / q)(r / s)V
		<=>
		(p / q) + (r / s)(p / q) < (r / s) + (p / q)(r / s)
		<=>
		(p / q) < (r / s)

		Now assume downsampling + downsampling,
		(p / q < 1 and r / s < 1), then:

		C1 < C2
		<=>
		H + V + (p / q)V + T < H + V + (r / s)V + T
		<=>
		(p / q)V < (r / s)V
		<=>
		(p / q) < (r / s)

		Last, assume downsampling + upsampling,
		(p / q < 1 and r / s >= 1), then:

		C1 < C2
		<=>
		H + V + (p / q)(r / s)V + T < H + (r / s)V + (r / s)V + T
		<=>
		V + (p / q)(r / s)V < (r / s)V + (r / s)V
		<=>
		1 + (p / q)(r / s) < 2(r / s)
		<=>
		(s / r) + (p / q) < 2
		<=>
		(p / q) < 2 - (s / r) <= (r / s)

		For the last step:

		2 - (s / r) <= (r / s)
		<=>
		2(r / s) - 1 <= (r / s)^2
		<=>
		(r / s)^2 - 2(r / s) + 1 >= 0
		<=>
		((r / s) - 1)^2 >= 0
		<=>
		true

		Generalization
		--------------

		Let us generalize the cost function in the following manner:

		C[i] = radius[i] * volume(extent), if newExtent[P[i]] < extent[P[i]]
			   radius[i] * volume(extent) * (newExtent[P[i]] / extent[P[i]]), otherwise
		       
		C[i] = C[i - 1] * (radius[i] / radius[i - 1]) * (newExtent[P[i - 1]] / extent[P[i - 1]]), if newExtent[P[i]] < extent[P[i]]
			   C[i - 1] * (radius[i] / radius[i - 1]) * (newExtent[P[i - 1]] / extent[P[i - 1]]) * (newExtent[P[i]] / extent[P[i]]), otherwise

		C[i] = C[i - 1] * (radius[i] / radius[i - 1]) * ((radius[i - 1] * newExtent[P[i - 1]]) / (radius[i - 1] * extent[P[i - 1]])), if newExtent[P[i]] < extent[P[i]]
			   C[i - 1] * (radius[i] / radius[i - 1]) * ((radius[i - 1] * newExtent[P[i - 1]]) / (radius[i - 1] * extent[P[i - 1]])) * ((radius[i] * newExtent[P[i]]) / (radius[i] * extent[P[i]])), otherwise

		Assume upsampling + upsampling
		(p / q >= 1 and r / s >= 1), then:

		C1 < C2 
		<=>
		H + u(p / q)V + v(r / s)u(p / q)V + T < H + v(r / s)V + u(p / q)v(r / s)V + T
		<=>
		u(p / q)V + v(r / s)u(p / q)V < v(r / s)V + u(p / q)v(r / s)V
		<=>
		u(p / q) + v(r / s)u(p / q) < v(r / s) + u(p / q)v(r / s)
		<=>
		u(p / q) < v(r / s)

		Assume downsampling + downsampling,
		(p / q < 1 and r / s < 1), then:

		C1 < C2
		<=>
		H + V + u(p / q)V + T < H + V + v(r / s)V + T
		<=>
		u(p / q)V < v(r / s)V
		<=>
		u(p / q) < v(r / s)

		Last, assume downsampling + upsampling,
		(p / q < 1 and r / s >= 1), then:

		C1 < C2
		<=>
		H + uV + u(p / q)v(r / s)V + T < H + v(r / s)V + uv(r / s)V + T
		<=>
		uV + u(p / q)v(r / s)V < v(r / s)V + uv(r / s)V
		<=>
		u + uv(p / q)(r / s) < (r / s)(v + uv)
		<=>
		u(s / r) + uv(p / q) < v + uv
		<=>
		(u / v)(s / r) + u(p / q) < 1 + u
		<=>
		u(p / q) < 1 + u - (u / v)(s / r)

		Let us examine if
		1 + u - (u / v)(s / r) <= v(r / s)
		<=>
		(r / s)(1 + u) - (u / v) <= v(r / s)^2
		<=>
		v(r / s)^2 - (r / s)(1 + u) + (u / v) >= 0
		<=>
		[1]

		Quadratic inequality in (r / s), find roots:

		r0 / s0
		= [(1 + u) +- sqrt((1 + u)^2 - 4 v (u / v))] / 2v
		= [(1 + u) +- sqrt((1 + u)^2 - 4u)] / 2v
		= [(1 + u) +- sqrt((u - 1)^2)] / 2v
		= [(1 + u) +- |u - 1|] / 2v

		=>
		r0 / s0 = 1 / v
		or
		r0 / s0 = u / v

		That is:
		[1]
		<=>
		(r / s) <= 1 / v or (r / s) >= (u / v)
		<=>
		(r / s) >= (u / v)
		<=>
		[2]

		Assume u / v <= 1 <=> u <= v
		then [2] <=> true and thus
		u(p / q) < v(r / s).

		The problematic case is when u > v. Then
		we can't end up at the same inequality:

		[2]
		<=>
		v(r / s) >= u
		=> Note missing the other direction!!
		v(r / s) >= u(p / q)
		<=>
		u(p / q) <= v(r / s)

		x down	y down	u <= v	result
		0		0		0		optimal
		0		0		1		optimal
		0		1		0		optimal
		0		1		1
		1		0		0
		1		0		1		optimal
		1		1		0		optimal
		1		1		1		optimal
		*/

		BOOST_STATIC_ASSERT(N != 1);

		// Find out the mostly-optimal order.

		SmallSet<Detail_Resample::AxisValue> axisSet;
		for (integer i = 0;i < N;++i)
		{
			axisSet.insert(
				Detail_Resample::AxisValue((filter->radius() * output.extent()[i]) 
				/ input.extent()[i], i));
		}

		// The first resampling is from the input view to
		// a temporary array.

		Vector<N, integer> extent = input.extent();
		extent[axisSet[0].axis_] = output.extent()[axisSet[0].axis_];
		Array<N, Computation_Element> tempArray(extent);

		{
			const ArrayExtender<1, Input_Element> arrayExtender1D(
				arrayExtender.extender(0), arrayExtender.border());
			const Detail_Resample::ResampleFunctor<Computation_Element, Input_Element> 
				resampleFunctor(arrayExtender1D, filter, blurFactor);
			visitRows(input, arrayView(tempArray), axisSet[0].axis_, resampleFunctor);
		}

		// The second resampling to the previous-to-last resampling
		// is done between temporary arrays.

		for (integer i = 1;i < N - 1;++i)
		{
			extent[axisSet[i].axis_] = output.extent()[axisSet[i].axis_];
			Array<N, Computation_Element> tempArray2(extent);

			const ArrayExtender<1, Computation_Element> arrayExtender1D(
				arrayExtender.extender(i), arrayExtender.border());
			const Detail_Resample::ResampleFunctor<Computation_Element, Input_Element> 
				resampleFunctor(arrayExtender1D, filter, blurFactor);

			visitRows(constArrayView(tempArray), arrayView(tempArray2), 
				axisSet[i].axis_, resampleFunctor);

			tempArray.swap(tempArray2);
		}

		// The last resampling is done to the output view.

		{
			const ArrayExtender<1, Computation_Element> arrayExtender1D(
				arrayExtender.extender(N - 1), arrayExtender.border());
			const Detail_Resample::ResampleFunctor<Computation_Element, Input_Element> 
				resampleFunctor(arrayExtender1D, filter, blurFactor);

			visitRows(constArrayView(tempArray), output, axisSet[N - 1].axis_, resampleFunctor);
		}
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
		const ConstFilterPtr& filter,
		const View<N, Output_Element, Output_View>& output,
		real blurFactor)
	{
		ENSURE_OP(blurFactor, >=, 1);

		Pastel::resampleTable<Computation_Element>(
			input, arrayExtender, 
			tableFilter(filter), output,
			blurFactor);
	}

}

#endif
