/*!
\file
\brief Functions for array programming with matrices.
*/

#ifndef PASTEL_MATRIX_TOOLS_MORE4_HPP
#define PASTEL_MATRIX_TOOLS_MORE4_HPP

#include "pastel/math/matrix_tools.h"

namespace Pastel
{

	template <
		int Height, int Width,
		typename Real,
		typename Expression>
	class MatrixSum
		: public VectorExpression<Width, Real,
		MatrixSum<Height, Width, Real, Expression> >
	{
	public:
		typedef const MatrixSum& StorageType;

		explicit MatrixSum(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator[](integer index) const
		{
			Real sum = 0;
			const integer n = data_.height();
			
			for (integer i = 0;i < n;++i)
			{
				sum += data_(i, index);
			}

			return sum;
		}

		integer size() const
		{
			return data_.width();
		}

		bool involves(
			void* address) const
		{
			return this == address ||
				data_.involves(address);
		}

		bool involvesNonTrivially(
			void* address) const
		{
			// This is a non-trivial vector
			// expression.
			return data_.involves(address);
		}

	private:
		typename Expression::StorageType data_;
	};

	template <int Height, int Width, typename Real, typename Expression>
	MatrixSum<Height, Width, Real, Expression> sum(
		const MatrixExpression<Height, Width, Real, Expression>& that)
	{
		return MatrixSum<Height, Width, Real, Expression>(
			(const Expression&)that);
	}

	template <
		int Height, int Width,
		typename Real,
		typename Expression>
	class MatrixMin
		: public VectorExpression<Width, Real,
		MatrixMin<Height, Width, Real, Expression> >
	{
	public:
		typedef const MatrixMin& StorageType;

		explicit MatrixMin(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator[](integer index) const
		{
			Real minValue = -infinity<Real>();
			const integer n = data_.height();
			
			for (integer i = 0;i < n;++i)
			{
				const Real value = data_(i, index);
				if (value < minValue)
				{
					minValue = value;
				}
			}

			return minValue;
		}

		integer size() const
		{
			return data_.width();
		}

		bool involves(
			void* address) const
		{
			return this == address ||
				data_.involves(address);
		}

		bool involvesNonTrivially(
			void* address) const
		{
			// This is a non-trivial vector
			// expression.
			return data_.involves(address);
		}

	private:
		typename Expression::StorageType data_;
	};

	template <int Height, int Width, typename Real, typename Expression>
	MatrixMin<Height, Width, Real, Expression> min(
		const MatrixExpression<Height, Width, Real, Expression>& that)
	{
		return MatrixMin<Height, Width, Real, Expression>(
			(const Expression&)that);
	}

	template <
		int Height, int Width,
		typename Real,
		typename Expression>
	class MatrixMax
		: public VectorExpression<Width, Real,
		MatrixMax<Height, Width, Real, Expression> >
	{
	public:
		typedef const MatrixMax& StorageType;

		explicit MatrixMax(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator[](integer index) const
		{
			Real maxValue = -infinity<Real>();
			const integer n = data_.height();
			
			for (integer i = 0;i < n;++i)
			{
				const Real value = data_(i, index);
				if (value > maxValue)
				{
					maxValue = value;
				}
			}

			return maxValue;
		}

		integer size() const
		{
			return data_.width();
		}

		bool involves(
			void* address) const
		{
			return this == address ||
				data_.involves(address);
		}

		bool involvesNonTrivially(
			void* address) const
		{
			// This is a non-trivial vector
			// expression.
			return data_.involves(address);
		}

	private:
		typename Expression::StorageType data_;
	};

	template <int Height, int Width, typename Real, typename Expression>
	MatrixMax<Height, Width, Real, Expression> max(
		const MatrixExpression<Height, Width, Real, Expression>& that)
	{
		return MatrixMax<Height, Width, Real, Expression>(
			(const Expression&)that);
	}

	template <
		int Height, int Width,
		typename Real,
		typename Expression>
	class MatrixAbs
		: public MatrixExpression<Height, Width, Real,
		MatrixAbs<Height, Width, Real, Expression> >
	{
	public:
		typedef const MatrixAbs& StorageType;

		explicit MatrixAbs(
			const Expression& data)
			: data_(data)
		{
		}

		Real operator()(integer y, integer x) const
		{
			return mabs(data_(y, x));
		}

		integer width() const
		{
			return data_.width();
		}

		integer height() const
		{
			return data_.height();
		}

		bool involves(
			void* address) const
		{
			return this == address ||
				data_.involves(address);
		}

		bool involvesNonTrivially(
			void* address) const
		{
			return data_.involvesNonTrivially(address);
		}

	private:
		typename Expression::StorageType data_;
	};

	template <int Height, int Width, typename Real, typename Expression>
	MatrixAbs<Height, Width, Real, Expression> abs(
		const MatrixExpression<Height, Width, Real, Expression>& that)
	{
		return MatrixAbs<Height, Width, Real, Expression>(
			(const Expression&)that);
	}

	template <
		typename Real,
		typename Input_ConstView>
	class ConstViewMatrix
		: public MatrixExpression<Dynamic, Dynamic, Real,
		ConstViewMatrix<Real, Input_ConstView> >
	{
	public:
		typedef const ConstViewMatrix& StorageType;

		explicit ConstViewMatrix(
			const ConstView<2, Real, Input_ConstView>& data)
			: data_(data)
		{
		}

		Real operator()(integer y, integer x) const
		{
			return mabs(data_(y, x));
		}

		integer width() const
		{
			return data_.width();
		}

		integer height() const
		{
			return data_.height();
		}

		bool involves(
			void* address) const
		{
			// TODO: FIX:
			// It is possible that the
			// view is from a matrix 
			// in which case we should 
			// have a check for the views
			// also!

			return this == address;
		}

		bool involvesNonTrivially(
			void* address) const
		{
			// TODO: FIX:
			// It is possible that the
			// view is from a matrix 
			// in which case we should 
			// have a check for the views
			// also!

			return false;
		}

	private:
		const ConstView<2, Real, Input_ConstView>& data_;
	};

	template <typename Real, typename Input_ConstView>
	ConstViewMatrix<Real, Input_ConstView> asMatrix(
		const ConstView<2, Real, Input_ConstView>& that)
	{
		return ConstViewMatrix<Real, Input_ConstView>(that);
	}

}

#endif
