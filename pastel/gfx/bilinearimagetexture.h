// Description: BilinearImageTexture class
// Detail: Sampling of a texture reconstructed with the triangle filter

#ifndef PASTEL_BILINEARIMAGETEXTURE_H
#define PASTEL_BILINEARIMAGETEXTURE_H

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
	class BilinearImageTexture
		: public Texture<Type>
	{
	private:
		typedef Texture<Type> Base;

	public:
		using Base::Element;

		BilinearImageTexture()
			: image_(0)
			, extender_()
			, extent_()
		{
		}

		explicit BilinearImageTexture(
			const Array<Type, 2>& image,
			const ArrayExtender<2, Type>& extender = ArrayExtender<2, Type>())
			: image_(&image)
			, extender_(extender)
			, extent_(image.extent())
		{
		}

		virtual ~BilinearImageTexture()
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
	BilinearImageTexture<Type> bilinearImageTexture(
		const Array<Type, 2>& image,
		const ArrayExtender<2, Type>& extender = ArrayExtender<2, Type>())
	{
		return BilinearImageTexture<Type>(image, extender);
	}

}

#include "pastel/gfx/bilinearimagetexture.hpp"

#endif
