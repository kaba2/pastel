// Description: AdaptedArray class
// Detail: An adapter for an AbstractArray to change data type to another

#ifndef PASTELSYS_ADAPTEDARRAY_H
#define PASTELSYS_ADAPTEDARRAY_H

#include "pastel/sys/abstractarray.h"

namespace Pastel
{

	template <int N, typename FromType, typename ToType, typename Adapter>
	class ConstAdaptedArray
		: public ConstAbstractArray<N, FromType>
	{
	public:
		ConstAdaptedArray()
			: image_(0)
			, adapter_()
		{
		}

		ConstAdaptedArray(
			const Array<ToType, N>& image,
			const Adapter& adapter)
			: image_(&image)
			, adapter_(adapter)
		{
		}

		virtual ~ConstAdaptedArray()
		{
		}

		virtual Vector<integer, N> extent() const
		{
			return image_->extent();
		}

		virtual FromType operator()(
			const Vector<integer, N>& position) const
		{
			return adapter_.convert((*image_)(position));
		}

	protected:
		const Array<ToType, N>* image_;
		Adapter adapter_;
	};

	template <int N, typename FromType, typename ToType, typename Adapter>
	class AdaptedArray
		: public AbstractArray<N, FromType>
	{
	public:
		AdaptedArray()
			: image_(0)
			, adapter_()
		{
		}

		AdaptedArray(
			Array<ToType, N>& image,
			const Adapter& adapter)
			: image_(&image)
			, adapter_(adapter)
		{
		}

		virtual ~AdaptedArray()
		{
		}

		virtual void setExtent(
			const Vector<integer, N>& extent)
		{
			image_->setExtent(extent);
		}

		virtual Vector<integer, N> extent() const
		{
			return image_->extent();
		}

		virtual void write(
			const Vector<integer, N>& position,
			const FromType& data)
		{
			(*image_)(position) = adapter_.revert(data);
		}

		virtual FromType operator()(
			const Vector<integer, N>& position) const
		{
			return adapter_.convert((*image_)(position));
		}

	private:
		Array<ToType, N>* image_;
		Adapter adapter_;
	};

}

#endif
