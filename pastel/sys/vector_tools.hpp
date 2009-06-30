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

	template <int N, typename Real, typename Expression>
	std::ostream& operator<<(std::ostream& stream,
		const VectorExpression<N, Real, Expression>& vector)
	{
		const integer size = vector.size();

		for (integer i = 0;i < size;++i)
		{
			stream << vector[i] << " ";
		}

		return stream;
	}

	template <int N, typename Real>
	std::istream& operator>>(std::istream& stream,
		Vector<N, Real>& vector)
	{
		const integer size = vector.size();

		for (integer i = 0;i < size;++i)
		{
			stream >> vector[i];
		}

		return stream;
	}

	template <int N, typename Real, typename Expression>
	Real sum(const VectorExpression<N, Real, Expression>& x)
	{
		const integer size = x.size();

		Real result(x[0]);

		for (integer i = 1;i < size;++i)
		{
			result += x[i];
		}

		return result;
	}

	template <int N, typename Real, typename Expression>
	Real product(const VectorExpression<N, Real, Expression>& x)
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
		int N,
		typename Real>
	class VectorUnitAxis
		: public VectorExpression<N, Real,
		VectorUnitAxis<N, Real> >
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

	template <int N, typename Real>
	VectorUnitAxis<N, Real> unitAxis(integer index)
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
		BOOST_STATIC_ASSERT(N > 0);

		PENSURE2(index >= 0 && index < N, index, N);

		return Pastel::unitAxis<N, Real>(N, index);
	}

	template <int N, typename Real>
	VectorUnitAxis<N, Real> unitAxis(
		integer dimension, integer index)
	{
		PENSURE1(dimension > 0, dimension);
		PENSURE2(N == Dynamic || dimension == N,
			dimension, N);
		PENSURE2(index >= 0 && index < dimension, index, dimension);

		return VectorUnitAxis<N, Real>(index, dimension);
	}

	template <int N, typename Real, typename Expression>
	inline TemporaryVector<PASTEL_ADD_N(N, -1), Real> shrink(
		const VectorExpression<N, Real, Expression>& that)
	{
		BOOST_STATIC_ASSERT(N > 1 || N == Dynamic);

		const integer size = that.size();

		Vector<PASTEL_ADD_N(N, -1), Real> result(size - 1);
		for (int i = 0;i < size - 1;++i)
		{
			result[i] = that[i];
		}

		return result.asTemporary();
	}

	template <int N, typename Real, typename Expression>
	inline TemporaryVector<PASTEL_ADD_N(N, -1), Real> shrink(
		const VectorExpression<N, Real, Expression>& that,
		integer index)
	{
		BOOST_STATIC_ASSERT(N > 1 || N == Dynamic);

		const integer size = that.size();

		Vector<PASTEL_ADD_N(N, -1), Real> result(size - 1);
		for (integer i = 0;i < index;++i)
		{
			result[i] = that[i];
		}
		for  (integer i = index;i < size - 1;++i)
		{
			result[i] = that[i + 1];
		}

		return result.asTemporary();
	}

	template <
		int N,
		typename Real,
		typename Expression>
	class VectorExtend
		: public VectorExpression<PASTEL_ADD_N(N, 1), Real,
		VectorExtend<N, Real, Expression> >
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

	template <int N, typename Real, typename Expression>
	inline VectorExtend<N, Real, Expression> extend(
		const PASTEL_NO_DEDUCTION(Real)& left,
		const VectorExpression<N, Real, Expression>& right)
	{
		return VectorExtend<N, Real, Expression>(
			(const Expression&)right, 0, left);
	}

	template <int N, typename Real, typename Expression>
	inline VectorExtend<N, Real, Expression> extend(
		const VectorExpression<N, Real, Expression>& left,
		const PASTEL_NO_DEDUCTION(Real)& right)
	{
		return VectorExtend<N, Real, Expression>(
			(const Expression&)left, left.size(), right);
	}

	template <int N, typename Real, typename Expression>
	inline VectorExtend<N, Real, Expression> extend(
		const VectorExpression<N, Real, Expression>& left,
		const PASTEL_NO_DEDUCTION(Real)& right,
		integer index)
	{
		return VectorExtend<N, Real, Expression>(
			(const Expression&)left, index, right);
	}

	template <int N, typename Real,
		typename Expression>
		inline Real dot(
		const VectorExpression<N, Real, Expression>& that)
	{
		return sum(that * that);
	}

	template <int N, typename Real,
		typename LeftExpression, typename RightExpression>
		inline Real dot(
		const VectorExpression<N, Real, LeftExpression>& left,
		const VectorExpression<N, Real, RightExpression>& right)
	{
		return sum(left * right);
	}

	template <int N, typename Real, typename Expression>
	typename boost::enable_if_c<(N > 1 || N == Dynamic), Real>::type
		norm(const VectorExpression<N, Real, Expression>& that)
	{
		return std::sqrt(dot(that, that));
	}

	template <int N, typename Real, typename Expression>
	typename boost::enable_if_c<(N == 1), Real>::type
		norm(const VectorExpression<N, Real, Expression>& that)
	{
		return mabs(that[0]);
	}

	template <int N, typename Real, typename Expression>
	Real normManhattan(const VectorExpression<N, Real, Expression>& that)
	{
		return sum(mabs(that));
	}

	template <int N, typename Real, typename Expression>
	Real powerSum(
		const VectorExpression<N, Real, Expression>& that,
		const PASTEL_NO_DEDUCTION(Real)& metric)
	{
		PENSURE(metric >= 1);
		return sum(pow(mabs(that), metric));
	}

	template <int N, typename Real, typename Expression>
	Real pNorm(
		const VectorExpression<N, Real, Expression>& that,
		const PASTEL_NO_DEDUCTION(Real)& metric)
	{
		PENSURE1(metric >= 1, metric);

		return pow(powerSum(that, metric), inverse(metric));
	}

	template <int N, typename Real, typename Expression>
	Real normInfinity(const VectorExpression<N, Real, Expression>& that)
	{
		return max(mabs(that));
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> normalize(const TemporaryVector<N, Real>& that)
	{
		Vector<N, Real> result = that;
		result /= norm(result);
		return result.asTemporary();
	}

	template <int N, typename Real>
	TemporaryVector<N, Real> normalize(const Vector<N, Real>& that)
	{
		return that / norm(that);
	}

	template <typename Real, typename Expression>
	TemporaryVector<2, Real> cross(
		const VectorExpression<2, Real, Expression>& that)
	{
		Vector<2, Real> result(-that[1], that[0]);
		return result.asTemporary();
	}

	template <typename Real, typename ExpressionX,
	typename ExpressionY>
	TemporaryVector<3, Real> cross(
		const VectorExpression<3, Real, ExpressionX>& x,
		const VectorExpression<3, Real, ExpressionY>& y)
	{
		Vector<3, Real> result(
			x[1] * y[2] - x[2] * y[1],
			x[2] * y[0] - x[0] * y[2],
			x[0] * y[1] - x[1] * y[0]);

		return result.asTemporary();
	}

}

#endif
