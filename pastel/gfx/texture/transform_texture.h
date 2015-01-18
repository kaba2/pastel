// Description: Affine transform of another texture

#ifndef PASTELGFX_TRANSFORM_TEXTURE_H
#define PASTELGFX_TRANSFORM_TEXTURE_H

#include "pastel/gfx/texture/texture.h"

#include "pastel/math/affine_transformation.h"

namespace Pastel
{

	template <typename Type, int N = 2>
	class Transform_Texture
		: public Texture<Type, N>
	{
	public:
		Transform_Texture(
			const Texture<Type, N>& texture,
			const AffineTransformation<real>& transformation)
			: texture_(&texture)
			, invTransformation_(inverse(transformation))
		{
		}

		Type operator()(
			const Vector<real, N>& p,
			const Matrix<real>& m) const
		{
			return (*texture_)(
				transformPoint(invTransformation_, p),
				m * invTransformation_.matrix());
		}

		virtual std::string name() const
		{
			return std::string("Transform");
		}

	private:
		const Texture<Type, N>* texture_;
		AffineTransformation<real> invTransformation_;
	};

	template <typename Type, int N>
	Transform_Texture<Type, N> transformTexture(
		const Texture<Type, N>& texture,
		const AffineTransformation<real>& transformation)
	{
		return Transform_Texture<Type, N>(texture, transformation);
	}

}

#endif
