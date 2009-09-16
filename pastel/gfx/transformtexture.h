// Description: TransformTexture
// Detail: A texture which has gone through an affine transformation

#ifndef PASTEL_TRANSFORMTEXTURE_H
#define PASTEL_TRANSFORMTEXTURE_H

#include "pastel/gfx/texture.h"

#include "pastel/math/affinetransformation_tools.h"

namespace Pastel
{

	template <typename Type>
	class TransformTexture
		: public Texture<Type>
	{
	public:
		TransformTexture(
			const Texture<Type>& texture,
			const AffineTransformation2& transformation)
			: texture_(&texture)
			, transformation_(transformation)
			, invTransformation_(inverse(transformation))
		{
		}

		Type operator()(
			const Vector2& uv,
			const Vector2& dpDx,
			const Vector2& dpDy) const
		{
			return (*texture_)(
				uv * invTransformation_,
				dpDx * invTransformation_,
				dpDy * invTransformation_);
		}

		virtual std::string name() const
		{
			return std::string("Transform");
		}

	private:
		const Texture<Type>* texture_;
		AffineTransformation2 transformation_;
		AffineTransformation2 invTransformation_;
	};

	template <typename Type>
	TransformTexture<Type> transformTexture(
		const Texture<Type>& texture,
		const AffineTransformation2& transformation)
	{
		return TransformTexture<Type>(texture, transformation);
	}

}

#endif
