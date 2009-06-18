#ifndef PASTEL_MATRIXBASE_H
#define PASTEL_MATRIXBASE_H

#include "pastel/math/matrix.h"
#include "pastel/math/matrixexpression.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/vector_tools.h"

#include <boost/static_assert.hpp>
#include <boost/operators.hpp>

namespace Pastel
{

	template <int Height, int Width, typename Real>
	class Matrix;

	namespace Detail
	{

		template <int Height, int Width, typename Real>
		class MatrixBase
			: public MatrixExpression<Height, Width, Real, MatrixBase<Height, Width, Real> >
		{
		public:
			typedef const MatrixBase& StorageType;

			MatrixBase()
				: data_()
			{
				for (integer i = 0;i < Height;++i)
				{
					for (integer j = 0;j < Width;++j)
					{
						(*this)(i, j) = (i == j) ? 1 : 0;
					}
				}
			}

			template <typename Expression>
			explicit MatrixBase(
				const MatrixExpression<Height, Width, Real, Expression>& that)
				: data_()
			{
				for (integer i = 0;i < Height;++i)
				{
					for (integer j = 0;j < Width;++j)
					{
						(*this)(i, j) = that(i, j);
					}
				}
			}

			~MatrixBase()
			{
				BOOST_STATIC_ASSERT(Width > 0);
				BOOST_STATIC_ASSERT(Height > 0);
			}

			integer width() const
			{
				return Width;
			}

			integer height() const
			{
				return Height;
			}

			template <typename Type>
			bool involves(const Type* address) const
			{
				return (this == address);
			}

			void swap(Matrix<Height, Width, Real>& that)
			{
				for (integer i = 0;i < Height;++i)
				{
					data_[i].swap(that.data_[i]);
				}
			}

			void set(const Real& that)
			{
				for (integer y = 0;y < Height;++y)
				{
					data_[y].set(that);
				}
			}

			Real& operator()(integer y, integer x)
			{
				PENSURE2(y >= 0 && y < Height, y, Height);
				PENSURE2(x >= 0 && x < Width, x, Width);

				return data_[y][x];
			}

			const Real& operator()(integer y, integer x) const
			{
				PENSURE2(y >= 0 && y < Height, y, Height);
				PENSURE2(x >= 0 && x < Width, x, Width);

				return data_[y][x];
			}

			Vector<Width, Real>& operator[](integer y)
			{
				PENSURE2(y >= 0 && y < Height, y, Height);

				return data_[y];
			}

			const Vector<Width, Real>& operator[](integer y) const
			{
				PENSURE2(y >= 0 && y < Height, y, Height);

				return data_[y];
			}

			template <int RightWidth, typename RightExpression>
			Matrix<Height, Width, Real>& operator=(
				const MatrixExpression<Width, RightWidth, Real, RightExpression>& right)
			{
				if (right.involves(this))
				{
					// The right expression contains this matrix
					// as a subexpression. We thus need to evaluate
					// the expression first.
					
					Matrix<Width, RightWidth, Real> copyRight(right);
					*this = copyRight;
				}
				else
				{
					Matrix<Height, Width, Real>& left = 
						(Matrix<Height, Width, Real>&)*this;

					for (integer i = 0;i < Height;++i)
					{
						for (integer j = 0;j < Width;++j)
						{
							left[i][j] = right(i, j);
						}
					}
				}

				return (Matrix<Height, Width, Real>&)*this;
			}

			template <int RightWidth, typename RightExpression>
			Matrix<Height, Width, Real>& operator*=(
				const MatrixExpression<Width, RightWidth, Real, RightExpression>& right)
			{
				Matrix<Height, Width, Real>& left = 
					(Matrix<Height, Width, Real>&)*this;

				left = left * right;

				return left;
			}

			template <typename RightExpression>
			Matrix<Height, Width, Real>& operator+=(
				const MatrixExpression<Height, Width, Real, RightExpression>& right)
			{
				Matrix<Height, Width, Real>& left = 
					(Matrix<Height, Width, Real>&)*this;

				left = left + right;

				return left;
			}

			template <typename RightExpression>
			Matrix<Height, Width, Real>& operator-=(
				const MatrixExpression<Height, Width, Real, RightExpression>& right)
			{
				Matrix<Height, Width, Real>& left = 
					(Matrix<Height, Width, Real>&)*this;

				left = left - right;

				return left;
			}

			// Matrices vs scalars

			// Matrix += scalar and Matrix -= scalar are not
			// supported because of the possibly ambiguity:
			// it is not clear whether it should mean
			// "add / subtract element-wise" or
			// "add / subtract by multiples of identity matrix".
			// For *= and /= these interpretations are equivalent.

			Matrix<Height, Width, Real>& operator*=(
				const Real& right)
			{
				Matrix<Height, Width, Real>& left = 
					(Matrix<Height, Width, Real>&)*this;

				left = left * right;

				return left;
			}

			Matrix<Height, Width, Real>& operator/=(
				const Real& right)
			{
				Matrix<Height, Width, Real>& left = 
					(Matrix<Height, Width, Real>&)*this;

				left = left * inverse(right);

				return left;
			}

		private:
			// In case Height <= 0,
			// we want to give the error message
			// from BOOST_STATIC_ASSERT in
			// the destructor, rather than
			// negative subscript from here.

			Vector<Width, Real> data_[
				Height > 0 ? Height : 1];
		};

	}

}

#endif
