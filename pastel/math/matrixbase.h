#ifndef PASTEL_MATRIXBASE_H
#define PASTEL_MATRIXBASE_H

#include "pastel/math/matrix.h"
#include "pastel/math/matrixexpression.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/vector_tools.h"
#include "pastel/sys/commafiller.h"
#include "pastel/sys/sparseiterator.h"
#include "pastel/sys/memory_overlaps.h"

#include <boost/static_assert.hpp>
#include <boost/operators.hpp>

namespace Pastel
{

	template <int Height, int Width, typename Real>
	class Matrix;

	namespace Detail
	{

		template <int Height, int Width, typename Real, typename Derived>
		class MatrixBase
			: public MatrixExpression<Height, Width, Real, Derived>
		{
		public:
			typedef const MatrixBase& StorageType;
			typedef Real* Iterator;
			typedef const Real* ConstIterator;
			typedef VectorView<Width, Real> Row;
			typedef ConstVectorView<Width, Real> ConstRow;
			typedef Iterator RowIterator;
			typedef ConstIterator ConstRowIterator;
			typedef SparseIterator<Real> ColumnIterator;
			typedef ConstSparseIterator<Real> ConstColumnIterator;

			~MatrixBase()
			{
				BOOST_STATIC_ASSERT((Width > 0 && Height > 0) 
					|| (Width == Dynamic && Height == Dynamic));
			}

			Iterator begin()
			{
				return data();
			}

			ConstIterator begin() const
			{
				return data();
			}

			Iterator end()
			{
				return data() + size();
			}

			ConstIterator end() const
			{
				return data() + size();
			}

			RowIterator rowBegin(integer y)
			{
				return data() + y * width();
			}

			ConstRowIterator rowBegin(integer y) const
			{
				return data() + y * width();
			}

			RowIterator rowEnd(integer y)
			{
				return data() + (y + 1) * width();
			}

			ConstRowIterator rowEnd(integer y) const
			{
				return data() + (y + 1) * width();
			}

			ColumnIterator columnBegin(integer x)
			{
				return ColumnIterator(data() + x, width());
			}

			ConstColumnIterator columnBegin(integer x) const
			{
				return ConstColumnIterator(data() + x, width());
			}

			ColumnIterator columnEnd(integer x)
			{
				return ColumnIterator(data() + x + width() * height(), width());
			}

			ConstColumnIterator columnEnd(integer x) const
			{
				return ConstColumnIterator(data() + x + width() * height(), width());
			}

			integer width() const
			{
				return ((Derived&)*this).width();
			}

			integer height() const
			{
				return ((Derived&)*this).height();
			}

			integer size() const
			{
				return ((Derived&)*this).size();
			}

			Real* data()
			{
				return ((Derived&)*this).data();
			}

			const Real* data() const
			{
				return ((const Derived&)*this).data();
			}

			bool involves(const void* memoryBegin, const void* memoryEnd) const
			{
				return Pastel::memoryOverlaps(
					memoryBegin, memoryEnd,
					data(), data() + size());
			}

			bool involvesNonTrivially(const void* memoryBegin, const void* memoryEnd) const
			{
				return false;
			}

			// The parameter is deliberately taken by value because
			// a reference could be from this matrix.
			void set(const Real that)
			{
				std::fill(begin(), end(), that);
			}

			CommaFiller<Real, Iterator> operator|=(
				const Real& that)
			{
				return commaFiller<Real>(begin(), end(), that);
			}

			Real& operator()(integer i)
			{
				PENSURE2(i >= 0 && i < size(), i, size());

				return data()[i];
			}

			const Real& operator()(integer i) const
			{
				PENSURE2(i >= 0 && i < size(), i, size());

				return data()[i];
			}

			Real& operator()(integer y, integer x)
			{
				PENSURE2(y >= 0 && y < height(), y, height());
				PENSURE2(x >= 0 && x < width(), x, width());

				return data()[y * width() + x];
			}

			const Real& operator()(integer y, integer x) const
			{
				PENSURE2(y >= 0 && y < height(), y, height());
				PENSURE2(x >= 0 && x < width(), x, width());

				return data()[y * width() + x];
			}

			Row operator[](integer y)
			{
				PENSURE2(y >= 0 && y < height(), y, height());

				return Row(
					data() + y * width(), width());
			}

			ConstRow operator[](integer y) const
			{
				PENSURE2(y >= 0 && y < height(), y, height());

				return ConstRow(
					data() + y * width(), width());
			}

			template <typename RightExpression>
			Derived& operator=(
				const MatrixExpression<Height, Width, Real, RightExpression>& right)
			{
				// We allow the size of the matrix to
				// change in assignment.

				if (right.involvesNonTrivially(&*begin(), &*end()) ||
					width() != right.width() ||
					height() != right.height())
				{
					// The right expression contains this matrix
					// as a subexpression. We thus need to evaluate
					// the expression first.
					
					((Derived&)*this) = Derived(right);
				}
				else
				{
					const integer m = height();
					const integer n = width();
					Iterator iter = begin();
					const Iterator iterEnd = end();

					for (integer i = 0;i < m;++i)
					{
						for (integer j = 0;j < n;++j)
						{
							*iter = right(i, j);
							++iter;
						}
					}
				}

				return (Derived&)*this;
			}

			template <typename RightExpression>
			Derived& operator*=(
				const MatrixExpression<Width, Width, Real, RightExpression>& right)
			{
				PENSURE2(width() == right.height(), width(), right.height());

				Derived& left = 
					(Derived&)*this;

				left = left * right;

				return left;
			}

			template <typename RightExpression>
			Derived& operator+=(
				const MatrixExpression<Height, Width, Real, RightExpression>& right)
			{
				PENSURE2(width() == right.width(), width(), right.width());
				PENSURE2(height() == right.height(), height(), right.height());

				if (right.involvesNonTrivially(&*begin(), &*end()))
				{
					*this += Derived(right);
				}
				else
				{
					const integer m = height();
					const integer n = width();
					Iterator iter = begin();
					const Iterator iterEnd = end();

					for (integer i = 0;i < m;++i)
					{
						for (integer j = 0;j < n;++j)
						{
							*iter += right(i, j);
							++iter;
						}
					}
				}

				return (Derived&)*this;
			}

			template <typename RightExpression>
			Derived& operator-=(
				const MatrixExpression<Height, Width, Real, RightExpression>& right)
			{
				PENSURE2(width() == right.width(), width(), right.width());
				PENSURE2(height() == right.height(), height(), right.height());

				if (right.involvesNonTrivially(&*begin(), &*end()))
				{
					*this -= Derived(right);
				}
				else
				{
					const integer m = height();
					const integer n = width();
					Iterator iter = begin();
					const Iterator iterEnd = end();

					for (integer i = 0;i < m;++i)
					{
						for (integer j = 0;j < n;++j)
						{
							*iter -= right(i, j);
							++iter;
						}
					}
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

			// The parameter is deliberately taken by value because
			// a reference could be from this matrix.
			Derived& operator*=(
				const Real right)
			{
				Iterator iter = begin();
				const Iterator iterEnd = end();

				while(iter != iterEnd)
				{
					(*iter) *= right;
					++iter;
				}

				return (Derived&)*this;
			}

			// No need to take the parameter by value,
			// because we construct the inverse.
			Derived& operator/=(
				const Real& right)
			{
				return (*this) *= inverse(right);
			}
		};

	}

}

#endif
