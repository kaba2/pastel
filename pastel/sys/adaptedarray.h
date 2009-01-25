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
			const LinearArray<N, ToType>& image,
			const Adapter& adapter)
			: image_(&image)
			, adapter_(adapter)
		{
		}

		virtual ~ConstAdaptedArray()
		{
		}

		virtual Vector<N, integer> extent() const
		{
			return image_->extent();
		}

		virtual FromType operator()(
			const Point<N, integer>& position) const
		{
			return adapter_.toLogical((*image_)(position));
		}

	protected:
		const LinearArray<N, ToType>* image_;
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
			LinearArray<N, ToType>& image,
			const Adapter& adapter)
			: image_(&image)
			, adapter_(adapter)
		{
		}

		virtual ~AdaptedArray()
		{
		}

		virtual void setExtent(
			const Vector<N, integer>& extent)
		{
			image_->setExtent(extent);
		}

		virtual Vector<N, integer> extent() const
		{
			return image_->extent();
		}

		virtual void write(
			const Point<N, integer>& position,
			const FromType& data)
		{
			(*image_)(position) = adapter_.toPhysical(data);
		}

		virtual FromType operator()(
			const Point<N, integer>& position) const
		{
			return adapter_.toLogical((*image_)(position));
		}

	private:
		LinearArray<N, ToType>* image_;
		Adapter adapter_;
	};

}

#endif
