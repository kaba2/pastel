// Description: LinearImage_Texture class
// Detail: Image-based texture with linear reconstruction and no filtering.

#ifndef PASTELGFX_LINEARIMAGE_TEXTURE_H
#define PASTELGFX_LINEARIMAGE_TEXTURE_H

#include "pastel/gfx/texture/texture.h"

#include "pastel/sys/extender/arrayextender.h"

namespace Pastel
{

	template <typename Type, integer N>
	Type sampleLinear(
		const Vector<dreal, N>& uv,
		const Array<Type, N>& image,
		const ArrayExtender<N, Type>& extender);

	template <typename Type, integer N = 2>
	class LinearImage_Texture
		: public Texture<Type, N>
	{
	private:
		using Base = Texture<Type, N>;

	public:
		using Element = typename Base::Element;
		using ArrayExtender_ = ArrayExtender<N, Type>;

		LinearImage_Texture()
			: image_(0)
			, extender_()
			, extent_()
		{
		}

		explicit LinearImage_Texture(
			const Array<Type, N>& image,
			const ArrayExtender_& extender = ArrayExtender_())
			: image_(&image)
			, extender_(extender)
			, extent_(image.extent())
		{
		}

		virtual ~LinearImage_Texture()
		{
		}

		virtual Element operator()(
			const Vector<dreal, N>& p,
			const Matrix<dreal>& m) const
		{
			return sampleLinear(
				evaluate(p * extent_),
				*image_, extender_);
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
			return std::string("LinearImage");
		}

	private:
		const Array<Type, N>* image_;
		ArrayExtender_ extender_;
		Vector<dreal, N> extent_;
	};

	template <typename Type, integer N>
	LinearImage_Texture<Type, N> linearImageTexture(
		const Array<Type, N>& image,
		const ArrayExtender<N, Type>& extender = ArrayExtender<N, Type>())
	{
		return LinearImage_Texture<Type, N>(image, extender);
	}

}

#include "pastel/gfx/texture/linearimage_texture.hpp"

#endif
