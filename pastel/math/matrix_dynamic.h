#ifndef PASTEL_MATRIX_DYNAMIC_H
#define PASTEL_MATRIX_DYNAMIC_H

#include "pastel/math/matrix.h"

#include "pastel/sys/array.h"
#include "pastel/sys/arrayview.h"

namespace Pastel
{

	template <typename Real>
	class Matrix<Dynamic, Dynamic, Real>
		: public Detail::MatrixBase<Dynamic, Dynamic, Real, 
		Matrix<Dynamic, Dynamic, Real> >
	{
	private:
		typedef Detail::MatrixBase<Dynamic, Dynamic, Real, 
			Matrix<Dynamic, Dynamic, Real> > Base;

	public:
		typedef const Matrix& StorageType;

		typedef Pastel::View<2, Real, ArrayView<2, Array<2, Real> > > View;
		typedef Pastel::ConstView<2, Real, ConstArrayView<2, Array<2, Real> > > ConstView;

		// Using default copy constructor.
		// Using default assignment.
		// Using default destructor.

		Matrix()
			: data_()
		{
		}

		template <int Height, int Width, typename Expression>
		Matrix(const MatrixExpression<Height, Width, Real, Expression>& that)
			: data_(that.width(), that.height())
		{
			*this = that;
		}

		Matrix(integer height, integer width)
			: data_(width, height, 0)
		{
			const integer minSize = std::min(width, height);

			for (integer i = 0;i < minSize;++i)
			{
				data_(i, i) = 1;
			}
		}

		Matrix(
			integer height, integer width,
			const Alias<Real*>& dataAlias)
			: data_(width, height, dataAlias)
		{
		}

		Matrix& operator=(
			const Matrix& that)
		{
			data_ = that.data_;

			return *this;
		}

		template <int Height, int Width, typename Expression>
		Matrix& operator=(
			const MatrixExpression<Height, Width, Real, Expression>& that)
		{
			return (Matrix&)Base::operator=(that);
		}

		Real* data()
		{
			return &*data_.begin();
		}

		const Real* data() const
		{
			return &*data_.begin();
		}

		void swap(Matrix& that)
		{
			data_.swap(that.data_);
		}

		void clear()
		{
			data_.clear();
		}

		void setSize(integer newHeight, integer newWidth)
		{
			const integer oldMinSize = std::min(width(), height());

			data_.setExtent(newWidth, newHeight, 0);

			const integer newMinSize = std::min(newWidth, newHeight);

			for (integer i = oldMinSize;i < newMinSize;++i)
			{
				data_(i, i) = 1;
			}
		}
		
		integer width() const
		{
			return data_.width();
		}

		integer height() const
		{
			return data_.height();
		}

		integer size() const
		{
			return data_.size();
		}

		void set(const Real& that)
		{
			data_.set(that);
		}

		void reshape(integer height, integer width)
		{
			data_.reshape(Vector2i(width, height));
		}

		View view()
		{
			return arrayView(data_);
		}

		ConstView constView() const
		{
			return constArrayView(data_);
		}
		
		const Array<2, Real>& asArray() const
		{
			return data_;
		}

	private:
		Array<2, Real> data_;
	};

}

#endif
