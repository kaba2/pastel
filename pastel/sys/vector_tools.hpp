#ifndef PASTEL_VECTOR_TOOLS_HPP
#define PASTEL_VECTOR_TOOLS_HPP

#include "pastel/sys/vector_tools.h"
#include "pastel/sys/vector.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/ensure.h"

#include "pastel/sys/math_functions.h"

#include <boost/static_assert.hpp>

namespace Pastel
{

	template <typename Real, int N, typename Expression>
	std::ostream& operator<<(std::ostream& stream,
		const VectorExpression<Real, N, Expression>& vector)
	{
		const integer size = vector.size();

		for (integer i = 0;i < size;++i)
		{
			stream << vector[i] << " ";
		}

		return stream;
	}

	template <typename Real, int N>
	std::istream& operator>>(std::istream& stream,
		Vector<Real, N>& vector)
	{
		const integer size = vector.size();

		for (integer i = 0;i < size;++i)
		{
			stream >> vector[i];
		}

		return stream;
	}

	template <typename Real, int N, typename Expression>
	Real sum(const VectorExpression<Real, N, Expression>& x)
	{
		const integer size = x.size();

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
		const integer size = x.size();

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
		typedef const VectorUnitAxis StorageType;

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

		bool involvesNonTrivially(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			return false;
		}

	private:
		const integer axis_;
		const integer size_;
	};

	template <typename Real, int N>
	VectorUnitAxis<Real, N> unitAxis(integer index)
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
		BOOST_STATIC_ASSERT(N > 0);

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
		BOOST_STATIC_ASSERT(N > 1 || N == Dynamic);

		const integer size = that.size();

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
		BOOST_STATIC_ASSERT(N > 1 || N == Dynamic);

		const integer size = that.size();

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
		typedef const VectorExtend& StorageType;

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

		bool involvesNonTrivially(
			const void* memoryBegin,
			const void* memoryEnd) const
		{
			// This is a non-trivial expression.
			return expression_.involves(memoryBegin, memoryEnd);
		}

	private:
		typename Expression::StorageType expression_;
		const integer dataIndex_;
		const Real data_;
	};

	template <typename Real, int N, typename Expression>
	inline VectorExtend<Real, N, Expression> extend(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const VectorExpression<Real, N, Expression>& right)
	{
		return VectorExtend<Real, N, Expression>(
			(const Expression&)right, 0, left);
	}

	template <typename Real, int N, typename Expression>
	inline VectorExtend<Real, N, Expression> extend(
		const VectorExpression<Real, N, Expression>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return VectorExtend<Real, N, Expression>(
			(const Expression&)left, left.size(), right);
	}

	template <typename Real, int N, typename Expression>
	inline VectorExtend<Real, N, Expression> extend(
		const VectorExpression<Real, N, Expression>& left,
		const PASTEL_NO_DEDUCTION(Real)& right,
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
		BOOST_STATIC_ASSERT(LeftN == Dynamic || RightN == Dynamic ||
			LeftN == RightN);

		return sum(left * right);
	}

	template <typename Real, int N, typename Expression>
	typename boost::enable_if_c<(N > 1 || N == Dynamic), Real>::type
		norm(const VectorExpression<Real, N, Expression>& that)
	{
		return std::sqrt(dot(that, that));
	}

	template <typename Real, int N, typename Expression>
	typename boost::enable_if_c<(N == 1), Real>::type
		norm(const VectorExpression<Real, N, Expression>& that)
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
		const PASTEL_NO_DEDUCTION(Real)& metric)
	{
		PENSURE_OP(metric, >=, 1);
		return sum(pow(mabs(that), metric));
	}

	template <typename Real, int N, typename Expression>
	Real minkowskiNorm(
		const VectorExpression<Real, N, Expression>& that,
		const PASTEL_NO_DEDUCTION(Real)& metric)
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

	template <typename Real, typename Expression>
	Vector<Real, 2> cross(
		const VectorExpression<Real, 2, Expression>& that)
	{
		Vector<Real, 2> result(-that[1], that[0]);
		return result;
	}

	template <typename Real, typename ExpressionX,
	typename ExpressionY>
	Vector<Real, 3> cross(
		const VectorExpression<Real, 3, ExpressionX>& x,
		const VectorExpression<Real, 3, ExpressionY>& y)
	{
		Vector<Real, 3> result(
			x[1] * y[2] - x[2] * y[1],
			x[2] * y[0] - x[0] * y[2],
			x[0] * y[1] - x[1] * y[0]);

		return result;
	}

}

#endif
