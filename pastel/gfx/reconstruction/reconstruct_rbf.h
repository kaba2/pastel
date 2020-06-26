// Description: Interpolation based on Radial Basis Functions
// Documentation: reconstruction.txt

#ifndef PASTELGFX_RECONSTRUCT_RBF_H
#define PASTELGFX_RECONSTRUCT_RBF_H

#include "pastel/sys/vector.h"

#include "pastel/gfx/filter.h"

#include "pastel/geometry/shape/alignedbox.h"

#include <vector>

namespace Pastel
{

	class MultiQuadric_Rbf
		: public Filter
	{
	public:
		// Using default copy constructor.
		// Using default assignment.

		explicit MultiQuadric_Rbf(dreal beta = 1)
			: Filter((dreal)Infinity(), "multiquadric")
			, beta_(beta)
		{
		}

		virtual ~MultiQuadric_Rbf()
		{
		}

		virtual dreal evaluateInRange(dreal x) const
		{
			return std::sqrt(x + square(beta_));
		}

	private:
		MultiQuadric_Rbf(const MultiQuadric_Rbf& that) = delete;
		MultiQuadric_Rbf& operator=(const MultiQuadric_Rbf& that) = delete;

		dreal beta_;
	};

	template <typename Real, integer N, typename Data, typename Output_View>
	void reconstructRbf(
		const std::vector<Vector<Data, N> >& positionList,
		const std::vector<Data>& dataList,
		const AlignedBox<Real, N>& region,
		const FilterPtr& radialBasisFunction,
		const View<N, Data, Output_View>& view);

}

#include "pastel/sys/vector/vector_tools.h"

namespace Pastel
{

	namespace ReconstructRbf_
	{

		template <typename Real, integer N, typename Data>
		class ReconstructFunctor
		{
		public:
			explicit ReconstructFunctor(
				const std::vector<Vector<Real, N> >& positionList,
				const Vector<Real, Dynamic>& w,
				const Vector<Data, Dynamic>& b,
				const Vector<Real, N>& scaling,
				const FilterPtr& radialBasisFunction)
				: positionList_(positionList)
				, w_(w)
				, b_(b)
				, scaling_(scaling)
				, radialBasisFunction_(radialBasisFunction)
			{
			}

			void operator()(
				const Vector<integer, N>& position,
				Data& data) const
			{
				integer n = w_.size();

				Real result = 0;
				for (integer i = 0;i < n;++i)
				{

					result += w_[i] * radialBasisFunction_->evaluate(
						dot(((Vector<Real, N>(position) + 0.5) - positionList_[i]) * scaling_));
				}

				data = result;
			}

		private:
			const std::vector<Vector<Real, N> >& positionList_;
			const Vector<Real, Dynamic>& w_;
			const Vector<Data, Dynamic>& b_;
			Vector<Real, N> scaling_;

			const FilterPtr& radialBasisFunction_;
		};

	}

	template <typename Real, integer N, typename Data, typename Output_View>
	void reconstructRbf(
		const std::vector<Vector<Real, N> >& positionList,
		const std::vector<Data>& dataList,
		const AlignedBox<Real, N>& region,
		const FilterPtr& radialBasisFunction,
		const View<N, Data, Output_View>& view)
	{
		integer n = positionList.size();

		Matrix<Real> a(n, n);

		for (integer i = 0;i < n;++i)
		{
			a(i, i) = radialBasisFunction->evaluate(0);
		}

		Vector<Real, N> scaling = region.extent() / Vector<Real, N>(view.extent());

		for (integer i = 0;i < n;++i)
		{
			for (integer j = i + 1;j < n;++j)
			{
				Real value = 
					radialBasisFunction->evaluate(
					dot(positionList[j] - positionList[i]));
				
				a(i, j) = value;
				a(j, i) = value;
			}
		}
		
		Vector<Real, Dynamic> b(ofDimension(n));
		for (integer i = 0;i < n;++i)
		{
			b[i] = dataList[i];
		}
		
		Vector<Real, Dynamic> w = solveLinear(a, b);

		ReconstructRbf_::ReconstructFunctor<Real, N, Data>
			reconstructFunctor(positionList, w, b, scaling, radialBasisFunction);
		
		visitPosition(view, reconstructFunctor);
	}

}

#endif
