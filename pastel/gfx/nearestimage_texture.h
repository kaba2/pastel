// Description: NearestImage_Texture class
// Detail: Image-based texture with nearest reconstruction and no filtering.
// Documentation: texture.txt

#ifndef PASTEL_NEARESTIMAGE_TEXTURE_H
#define PASTEL_NEARESTIMAGE_TEXTURE_H

#include "pastel/gfx/texture.h"

#include "pastel/sys/arrayextender.h"

namespace Pastel
{

	template <typename Type>
	class NearestImage_Texture
		: public Texture<Type>
	{
	private:
		typedef Texture<Type> Base;

	public:
		using Base::Element;

		NearestImage_Texture()
			: image_(0)
			, extender_()
			, extent_()
		{
		}

		explicit NearestImage_Texture(
			const Array<Type, 2>& image,
			const ArrayExtender<2, Type>& extender = ArrayExtender<2, Type>())
			: image_(&image)
			, extender_(extender)
			, extent_(image.extent())
		{
		}

		virtual ~NearestImage_Texture()
		{
		}

		virtual Element operator()(
			const Vector2& p,
			const Vector2& dpdx,
			const Vector2& dpdy) const
		{
			const Vector<integer, 2> x(p * extent_);

			return extender_(*image_, x);
		}

		void setImage(const Array<Type, 2>& image)
		{
			image_ = &image;
			extent_ = Vector2(image.extent());
		}

		void setExtender(const ArrayExtender<2, Type>& extender)
		{
			extender_ = extender;
		}

		virtual std::string name() const
		{
			return std::string("NearestImage");
		}

	private:
		const Array<Type, 2>* image_;
		ArrayExtender<2, Type> extender_;
		Vector2 extent_;
	};

	template <typename Type>
	NearestImage_Texture<Type> nearestImageTexture(
		const Array<Type, 2>& image,
		const ArrayExtender<2, PASTEL_NO_DEDUCTION(Type)>& extender = ArrayExtender<2, Type>())
	{
		return NearestImage_Texture<Type>(image, extender);
	}

}

#include "pastel/gfx/nearestimage_texture.hpp"

#endif
