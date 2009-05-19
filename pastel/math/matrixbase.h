#ifndef PASTEL_MATRIXBASE_H
#define PASTEL_MATRIXBASE_H

#include "pastel/math/matrix.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/vector_tools.h"

#include <boost/static_assert.hpp>

namespace Pastel
{

	namespace Detail
	{

		template <int Height, int Width, typename Real,
			template <int, int, typename> class DerivedT>
		class MatrixBase
		{
		private:
			typedef DerivedT<Height, Width, Real> Derived;

		public:
			MatrixBase()
				: data_()
			{
				for (integer i = 0;i < Height;++i)
				{
					for (integer j = 0;j < Width;++j)
					{
						(*this)(i, j) = (i == j) ?
							Real(1) : Real(0);
					}
				}
			}

			MatrixBase(const MatrixBase<Width, Height,
				Real,	DerivedT>& that,
				MatrixTransposeTag)
			{
				for (integer y = 0;y < Height;++y)
				{
					for (integer x = 0;x < Width;++x)
					{
						data_[y][x] = that[x][y];
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

			void swap(Derived& that)
			{
				for (integer i = 0;i < Height;++i)
				{
					data_[i].swap(that.data_[i]);
				}
			}

			void setTranspose(
				const MatrixBase<Width, Height,
				Real,	DerivedT>& that)
			{
				for (integer i = 0;i < Height;++i)
				{
					for (integer j = 0;j < Width;++j)
					{
						data_[i][j] = that.data_[j][i];
					}
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

			Derived& operator*=(
				const DerivedT<Width, Width, Real>& right)
			{
				Derived& left = (Derived&)*this;
				Derived copyLeft(left);
				for (integer i = 0;i < Height;++i)
				{
					for (integer j = 0;j < Width;++j)
					{
						left[i][j] = copyLeft[i][0] * right[0][j];
						for (integer k = 1;k < Width;++k)
						{
							left[i][j] += copyLeft[i][k] * right[k][j];
						}
					}
				}

				return left;
			}

			Derived& operator+=(
				const Derived& right)
			{
				for (integer y = 0;y < Height;++y)
				{
					data_[y] += right.data_[y];
				}

				return (Derived&)*this;
			}

			Derived& operator-=(
				const Derived& right)
			{
				for (integer y = 0;y < Height;++y)
				{
					data_[y] -= right.data_[y];
				}

				return (Derived&)*this;
			}

			// Matrices vs scalars

			// Matrix += scalar and Matrix -= scalar are not
			// supported because of the possibly ambiguity:
			// it is not clear whether it should mean
			// "add / subtract element-wise" or
			// "add / subtract by multiples of identity matrix".
			// For *= and /= these interpretations are equivalent.

			Derived& operator*=(
				const Real& right)
			{
				for (integer i = 0;i < Height;++i)
				{
					data_[i] *= right;
				}

				return (Derived&)*this;
			}

			Derived& operator/=(
				const Real& right)
			{
				Real invRight(Pastel::inverse(right));

				for (integer i = 0;i < Height;++i)
				{
					data_[i] *= invRight;
				}

				return (Derived&)*this;
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
