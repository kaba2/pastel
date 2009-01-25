#ifndef PASTELSYS_ARRAYEXTENDER_H
#define PASTELSYS_ARRAYEXTENDER_H

#include "pastel/sys/lineararray.h"
#include "pastel/sys/indexextender.h"
#include "pastel/sys/point.h"

namespace Pastel
{

	template <int N, typename Type>
	class ArrayExtender
	{
	public:
		ArrayExtender(
			const ConstIndexExtenderRef& uniformExtender = IndexExtenderRef(),
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
			extender_.set(IndexExtenderRef());
		}

		void clearExtender(integer index)
		{
			setExtender(index, IndexExtenderRef());
		}

		void setExtender(integer index, 
			const ConstIndexExtenderRef& extender)
		{
			ENSURE2(index >= 0 && index < extender_.size(), 
				index, extender_.size());

			extender_[index] = extender;
		}

		ConstIndexExtenderRef extender(integer index) const
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
			const LinearArray<N, Type>& image,
			const Point<N, integer>& position) const
		{
			const Vector<N, integer>& extent = image.extent();
			
			Point<N, integer> newPosition;
			for (integer i = 0;i < N;++i)
			{
				const ConstIndexExtenderRef& extender = extender_[i];
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
		Tuple<N, ConstIndexExtenderRef> extender_;
		Type border_;
	};



}

#endif
