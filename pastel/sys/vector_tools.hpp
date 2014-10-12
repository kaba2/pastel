#ifndef PASTELSYS_VECTOR_TOOLS_HPP
#define PASTELSYS_VECTOR_TOOLS_HPP

#include "pastel/sys/vector_tools.h"
#include "pastel/sys/vector.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <typename Real, int N, typename Expression>
	std::ostream& operator<<(std::ostream& stream,
		const VectorExpression<Real, N, Expression>& vector)
	{
		integer n = vector.size();

		for (integer i = 0;i < n - 1;++i)
		{
			stream << vector[i] << " ";
		}
		stream << vector[n - 1];

		return stream;
	}

	template <typename Real, int N>

	std::istream& operator>>(std::istream& stream,
		Vector<Real, N>& vector)
	{
		integer size = vector.size();

		for (integer i = 0;i < size;++i)
		{
			stream >> vector[i];
		}

		return stream;
	}

	template <typename Real, int N, typename Expression>

	Real sum(const VectorExpression<Real, N, Expression>& x)
	{
		integer size = x.size();

		Real result(x[0]);

		for (integer i = 1;i < size;++i)
		{
			result += x[i];
		}

		return result;
	}

	template <typename Real, int N, typename Expression>

	Real product(const VectorExpression<Real, N, Expression>& x)
	{
		integer size = x.size();

		Real result(x[0]);

		for (integer i = 1;i < size;++i)
		{

			result *= x[i];
		}

		return result;
	}

	template <
		typename Real,
		int N>
	class VectorUnitAxis
		: public VectorExpression<Real, N, VectorUnitAxis<Real, N> >
	{
	public:
		using StorageType = const VectorUnitAxis;

		VectorUnitAxis(
			integer axis,
			integer size)
			: axis_(axis)
			, size_(size)
		{
		}

		Real operator[](integer index) const
		{
			if (index == axis_)
			{
				return 1;
			}

			return 0;
		}

		integer size() const
		{
			return size_;
		}

		bool involves(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return false;
		}

		bool evaluateBeforeAssignment(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return false;
		}

	private:
		integer axis_;
		integer size_;
	};

	template <typename Real, int N>
	VectorUnitAxis<Real, N> unitAxis(integer index)
	{
		PASTEL_STATIC_ASSERT(N != Dynamic);
		PASTEL_STATIC_ASSERT(N > 0);


		PENSURE2(index >= 0 && index < N, index, N);

		return Pastel::unitAxis<Real, N>(N, index);
	}

	template <typename Real, int N>
	VectorUnitAxis<Real, N> unitAxis(
		integer dimension, integer index)
	{
		PENSURE_OP(dimension, >, 0);
		PENSURE2(N == Dynamic || dimension == N,
			dimension, N);
		PENSURE2(index >= 0 && index < dimension, index, dimension);

		return VectorUnitAxis<Real, N>(index, dimension);
	}

	template <typename Real, int N, typename Expression>
	inline Vector<Real, ModifyN<N, N - 1>::Result> shrink(
		const VectorExpression<Real, N, Expression>& that)
	{
		PASTEL_STATIC_ASSERT(N > 1 || N == Dynamic);

		integer size = that.size();

		Vector<Real, ModifyN<N, N - 1>::Result> result(size - 1);
		for (int i = 0;i < size - 1;++i)
		{
			result[i] = that[i];
		}

		return result;
	}

	template <typename Real, int N, typename Expression>
	inline Vector<Real, ModifyN<N, N - 1>::Result> shrink(
		const VectorExpression<Real, N, Expression>& that,
		integer index)
	{
		PASTEL_STATIC_ASSERT(N > 1 || N == Dynamic);

		integer size = that.size();

		Vector<Real, ModifyN<N, N - 1>::Result> result(size - 1);
		for (integer i = 0;i < index;++i)
		{
			result[i] = that[i];
		}
		for  (integer i = index;i < size - 1;++i)
		{
			result[i] = that[i + 1];
		}

		return result;
	}

	template <
		typename Real,
		int N,
		typename Expression>
	class VectorExtend
		: public VectorExpression<Real, ModifyN<N, N + 1>::Result, VectorExtend<Real, N, Expression> >
	{
	public:

		using StorageType = const VectorExtend&;

		VectorExtend(
			const Expression& expression,
			integer dataIndex,
			const Real& data)
			: expression_(expression)
			, dataIndex_(dataIndex)
			, data_(data)
		{
			PENSURE2(dataIndex >= 0 && dataIndex <= expression.size(),
				dataIndex, expression.size());
		}

		Real operator[](integer index) const
		{
			if (index < dataIndex_)
			{
				return expression_[index];
			}

			if (dataIndex_ < index)
			{
				return expression_[index - 1];
			}

			return data_;
		}

		integer size() const
		{
			return expression_.size() + 1;
		}

		bool involves(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return expression_.involves(memoryBegin, memoryEnd);
		}

		bool evaluateBeforeAssignment(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			// This is a non-trivial expression.
			return expression_.involves(memoryBegin, memoryEnd);
		}

	private:
		typename Expression::StorageType expression_;
		integer dataIndex_;
		Real data_;
	};

	template <typename Real, int N, typename Expression>
	inline VectorExtend<Real, N, Expression> extend(
		const NoDeduction<Real>& left,
		const VectorExpression<Real, N, Expression>& right)
	{
		return VectorExtend<Real, N, Expression>(
			(const Expression&)right, 0, left);
	}

	template <typename Real, int N, typename Expression>
	inline VectorExtend<Real, N, Expression> extend(
		const VectorExpression<Real, N, Expression>& left,
		const NoDeduction<Real>& right)
	{
		return VectorExtend<Real, N, Expression>(
			(const Expression&)left, left.size(), right);
	}

	template <typename Real, int N, typename Expression>
	inline VectorExtend<Real, N, Expression> extend(
		const VectorExpression<Real, N, Expression>& left,
		const NoDeduction<Real>& right,
		integer index)
	{
		return VectorExtend<Real, N, Expression>(
			(const Expression&)left, index, right);
	}

	template <typename Real, int N,
		typename Expression>
		inline Real dot(
		const VectorExpression<Real, N, Expression>& that)
	{
		return sum(that * that);
	}

	template <typename Real, int LeftN, int RightN,
		typename LeftExpression, typename RightExpression>
		inline Real dot(
		const VectorExpression<Real, LeftN, LeftExpression>& left,
		const VectorExpression<Real, RightN, RightExpression>& right)
	{
		PASTEL_STATIC_ASSERT(LeftN == Dynamic || RightN == Dynamic ||
			LeftN == RightN);

		return sum(left * right);
	}

	template <typename Real, int N, typename Expression, 
		typename NormBijection>
	Real norm2(const VectorExpression<Real, N, Expression>& that,
		const NormBijection& normBijection)
	{
		Real result = 0;

		integer n = that.n();
		for (integer i = 0;i < n;++i)
		{
			result = normBijection.addAxis(
				result, 
				normBijection.signedAxis(that[i]));
		}

		return result;
	}

	template <
		typename Real, 
		int N, 
		typename Expression,
		EnableIfC<(N > 1 || N == Dynamic)>*>
	Real norm(const VectorExpression<Real, N, Expression>& that)
	{
		return std::sqrt(dot(that, that));
	}

	template <
		typename Real, 
		int N, 
		typename Expression,
		EnableIfC<(N == 1)>*>
	Real norm(const VectorExpression<Real, N, Expression>& that)
	{
		return mabs(that[0]);
	}

	template <typename Real, int N, typename Expression>
	Real manhattanNorm(const VectorExpression<Real, N, Expression>& that)
	{
		return sum(mabs(that));
	}

	template <typename Real, int N, typename Expression>
	Real powerSum(
		const VectorExpression<Real, N, Expression>& that,
		const NoDeduction<Real>& metric)
	{
		PENSURE_OP(metric, >=, 1);
		return sum(pow(mabs(that), metric));
	}

	template <typename Real, int N, typename Expression>
	Real minkowskiNorm(
		const VectorExpression<Real, N, Expression>& that,
		const NoDeduction<Real>& metric)
	{
		PENSURE_OP(metric, >=, 1);

		return pow(powerSum(that, metric), inverse(metric));
	}

	template <typename Real, int N, typename Expression>
	Real maxNorm(const VectorExpression<Real, N, Expression>& that)
	{
		return max(mabs(that));
	}

	template <typename Real, int N>
	Vector<Real, N> normalize(const Vector<Real, N>& that)
	{
		return that / norm(that);
	}

	template <typename Real, int N, typename Expression>
	Vector<Real, N> normalize(
		const VectorExpression<Real, N, Expression>& that)
	{
		return Pastel::normalize(evaluate(that));
	}

	template <typename Real, int N, typename Expression>
	Vector<Real, 2> cross(
		const VectorExpression<Real, N, Expression>& that)
	{
		PASTEL_STATIC_ASSERT(N == 2 || N == Dynamic);
		PENSURE_OP(that.size(), ==, 2);
		
		Vector<Real, 2> result(-that[1], that[0]);

		return result;
	}

	template <typename Real, int N, typename ExpressionX,
	typename ExpressionY>
	Vector<Real, 3> cross(
		const VectorExpression<Real, N, ExpressionX>& x,
		const VectorExpression<Real, N, ExpressionY>& y)
	{
		PASTEL_STATIC_ASSERT(N == 3 || N == Dynamic);
		PENSURE_OP(x.size(), ==, 3);
		PENSURE_OP(y.size(), ==, 3);

		Vector<Real, 3> result(
			x[1] * y[2] - x[2] * y[1],
			x[2] * y[0] - x[0] * y[2],
			x[0] * y[1] - x[1] * y[0]);

		return result;
	}

}

#endif
