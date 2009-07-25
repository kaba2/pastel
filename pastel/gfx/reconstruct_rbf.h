// Description: Interpolation based on Radial Basis Functions

#ifndef PASTEL_RECONSTRUCT_RBF_H
#define PASTEL_RECONSTRUCT_RBF_H

#include "pastel/sys/point.h"

#include "pastel/dsp/filter.h"

#include "pastel/geometry/alignedbox.h"

#include <vector>

namespace Pastel
{

	class MultiQuadric_Rbf
		: public Filter
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		explicit MultiQuadric_Rbf(real beta = 1)
			: Filter(infinity<real>(), "multiquadric")
			, beta_(beta)
		{
		}

		virtual ~MultiQuadric_Rbf()
		{
		}

		virtual real evaluateInRange(real x) const
		{
			return std::sqrt(x + square(beta_));
		}

	private:
		// Prohibited
		MultiQuadric_Rbf(const MultiQuadric_Rbf& that);
		// Prohibited
		MultiQuadric_Rbf& operator=(const MultiQuadric_Rbf& that);

		real beta_;
	};


	template <int N, typename Real, typename Data, typename Output_View>
	void reconstructRbf(
		const std::vector<Point<N, Data> >& positionList,
		const std::vector<Data>& dataList,
		const AlignedBox<N, Real>& region,
		const FilterPtr& radialBasisFunction,
		const View<N, Data, Output_View>& view);

}

#include "pastel/math/matrix_tools.h"
#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	namespace Detail_ReconstructRbf
	{

		template <int N, typename Real, typename Data>
		class ReconstructFunctor
		{
		public:
			explicit ReconstructFunctor(
				const std::vector<Point<N, Real> >& positionList,
				const Vector<Dynamic, Real>& w,
				const Vector<Dynamic, Data>& b,
				const Vector<N, Real>& scaling,
				const FilterPtr& radialBasisFunction)
				: positionList_(positionList)
				, w_(w)
				, b_(b)
				, scaling_(scaling)
				, radialBasisFunction_(radialBasisFunction)
			{
			}

			void operator()(
				const Point<N, integer>& position,
				Data& data) const
			{
				const integer n = w_.size();

				Real result = 0;
				for (integer i = 0;i < n;++i)
				{
					result += w_[i] * radialBasisFunction_->evaluate(
						dot(((Point<N, Real>(position) + 0.5) - positionList_[i]) * scaling_));
				}

				data = result;
			}

		private:
			const std::vector<Point<N, Real> >& positionList_;
			const Vector<Dynamic, Real>& w_;
			const Vector<Dynamic, Data>& b_;
			const Vector<N, Real> scaling_;
			const FilterPtr& radialBasisFunction_;
		};

	}

	template <int N, typename Real, typename Data, typename Output_View>
	void reconstructRbf(
		const std::vector<Point<N, Real> >& positionList,
		const std::vector<Data>& dataList,
		const AlignedBox<N, Real>& region,
		const FilterPtr& radialBasisFunction,
		const View<N, Data, Output_View>& view)
	{
		const integer n = positionList.size();

		Matrix<Dynamic, Dynamic, Real> a(n, n);

		for (integer i = 0;i < n;++i)
		{
			a(i, i) = radialBasisFunction->evaluate(0);
		}

		Vector<N, Real> scaling = region.extent() / Vector<N, Real>(view.extent());

		for (integer i = 0;i < n;++i)
		{
			for (integer j = i + 1;j < n;++j)
			{
				const Real value = 
					radialBasisFunction->evaluate(
					dot(positionList[j] - positionList[i]));
				
				a(i, j) = value;
				a(j, i) = value;
			}
		}
		
		Vector<Dynamic, Real> b(ofDimension(n));
		for (integer i = 0;i < n;++i)
		{
			b[i] = dataList[i];
		}
		
		const Vector<Dynamic, Real> w = solveLinear(a, b);

		const Detail_ReconstructRbf::ReconstructFunctor<N, Real, Data>
			reconstructFunctor(positionList, w, b, scaling, radialBasisFunction);
		
		visitPosition(view, reconstructFunctor);
	}

}

#endif
