// Description: NearestImage_Texture class
// Detail: Image-based texture with nearest reconstruction and no filtering.

#ifndef PASTELGFX_NEARESTIMAGE_TEXTURE_H
#define PASTELGFX_NEARESTIMAGE_TEXTURE_H

#include "pastel/gfx/texture/texture.h"

#include "pastel/sys/extender/arrayextender.h"

namespace Pastel
{

	template <typename Type, int N = 2>
	class NearestImage_Texture
		: public Texture<Type, N>
	{
	private:
		using Base = Texture<Type, N>;

	public:
		using Element = typename Base::Element;
		using ArrayExtender_ = ArrayExtender<N, Type>;

		NearestImage_Texture()
			: image_(0)
			, extender_()
			, extent_()
		{
		}

		explicit NearestImage_Texture(
			const Array<Type, N>& image,
			const ArrayExtender_& extender = ArrayExtender_())
			: image_(&image)
			, extender_(extender)
			, extent_(image.extent())
		{
		}

		virtual ~NearestImage_Texture()
		{
		}

		virtual Element operator()(
			const Vector<dreal, N>& p,
			const Matrix<dreal, N, N>& m) const
		{
			const Vector<integer, N> x(p * extent_);

			return extender_(*image_, x);
		}

		void setImage(const Array<Type, N>& image)
		{
			image_ = &image;
			extent_ = Vector<dreal, N>(image.extent());
		}

		void setExtender(const ArrayExtender_& extender)
		{
			extender_ = extender;
		}

		virtual std::string name() const
		{
			return std::string("NearestImage");
		}

	private:
		const Array<Type, N>* image_;
		ArrayExtender_ extender_;
		Vector<dreal, N> extent_;
	};

	template <typename Type, int N>
	NearestImage_Texture<Type, N> nearestImageTexture(
		const Array<Type, N>& image,
		const ArrayExtender<N, NoDeduction<Type>>& extender = ArrayExtender<N, Type>())
	{
		return NearestImage_Texture<Type, N>(image, extender);
	}

}

#include "pastel/gfx/texture/nearestimage_texture.hpp"

#endif
