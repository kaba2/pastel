// Description: BilinearImage_Texture class
// Detail: Image-based texture with bilinear reconstruction and no filtering.
// Documentation: texture.txt

#ifndef PASTEL_BILINEARIMAGE_TEXTURE_H
#define PASTEL_BILINEARIMAGE_TEXTURE_H

#include "pastel/gfx/texture.h"

#include "pastel/sys/arrayextender.h"

namespace Pastel
{

	template <typename Type>
	Type sampleBilinear(
		const Vector2& uv,
		const Array<Type, 2>& image,
		const ArrayExtender<2, Type>& extender);

	template <typename Type>
	class BilinearImage_Texture
		: public Texture<Type>
	{
	private:
		typedef Texture<Type> Base;

	public:
		using Base::Element;

		BilinearImage_Texture()
			: image_(0)
			, extender_()
			, extent_()
		{
		}

		explicit BilinearImage_Texture(
			const Array<Type, 2>& image,
			const ArrayExtender<2, Type>& extender = ArrayExtender<2, Type>())
			: image_(&image)
			, extender_(extender)
			, extent_(image.extent())
		{
		}

		virtual ~BilinearImage_Texture()
		{
		}

		virtual Element operator()(
			const Vector2& p,
			const Vector2& dpdx,
			const Vector2& dpdy) const
		{
			return sampleBilinear(
				p * extent_,
				*image_, extender_);
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
			return std::string("BilinearImage");
		}

	private:
		const Array<Type, 2>* image_;
		ArrayExtender<2, Type> extender_;
		Vector2 extent_;
	};

	template <typename Type>
	BilinearImage_Texture<Type> bilinearImageTexture(
		const Array<Type, 2>& image,
		const ArrayExtender<2, Type>& extender = ArrayExtender<2, Type>())
	{
		return BilinearImage_Texture<Type>(image, extender);
	}

}

#include "pastel/gfx/bilinearimage_texture.hpp"

#endif
