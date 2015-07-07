// Description: Array extender
// Documentation: indexextenders.txt

#ifndef PASTELSYS_ARRAYEXTENDER_H
#define PASTELSYS_ARRAYEXTENDER_H

#include "pastel/sys/array.h"
#include "pastel/sys/extender/indexextender.h"
#include "pastel/sys/view/view.h"

namespace Pastel
{

	template <integer N, typename Type>
	class ArrayExtender
	{
	public:
		ArrayExtender()
			: extender_()
			, border_()
		{
		}

		explicit ArrayExtender(
			const Type& border)
			: extender_()
			, border_(border)
		{
		}

		// Implicit conversion
		ArrayExtender(
			const ConstIndexExtenderPtr& uniformExtender,
			const Type& border = Type())
			: extender_(uniformExtender)
			, border_(border)
		{
		}

		void swap(ArrayExtender& that)
		{
			using std::swap;

			extender_.swap(that.extender_);
			swap(border_, that.border_);
		}

		void clear()
		{
			extender_.set(IndexExtenderPtr());
		}

		void clearExtender(integer index)
		{
			setExtender(index, IndexExtenderPtr());
		}

		void setExtender(integer index,
			const ConstIndexExtenderPtr& extender)
		{
			ENSURE2(index >= 0 && index < extender_.size(),
				index, extender_.size());

			extender_[index] = extender;
		}

		ConstIndexExtenderPtr extender(integer index) const
		{
			ENSURE2(index >= 0 && index < extender_.size(),
				index, extender_.size());

			return extender_[index];
		}

		void setBorder(const Type& border)
		{
			border_ = border;
		}

		const Type& border() const
		{
			return border_;
		}

		Type operator()(
			const Array<Type, N>& image,
			const Vector<integer, N>& position) const
		{
			return (*this)(constArrayView(image), position);
		}

		template <typename Image_ConstView>
		Type operator()(
			const ConstView<N, Type, Image_ConstView>& image,
			const Vector<integer, N>& position) const
		{
			const Vector<integer, N>& extent = image.extent();

			Vector<integer, N> newPosition;
			for (integer i = 0;i < N;++i)
			{
				const ConstIndexExtenderPtr& extender = extender_[i];
				if (extender.empty())
				{
					if (position[i] < 0 || position[i] >= extent[i])
					{
						return border_;
					}

					newPosition[i] = position[i];
				}
				else
				{
					newPosition[i] = (*extender)(position[i], extent[i]);
				}
			}

			return image(newPosition);
		}

	private:
		Tuple<ConstIndexExtenderPtr, N> extender_;
		Type border_;
	};

}

#endif
