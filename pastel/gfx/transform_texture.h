// Description: Transform_Texture class
// Detail: An affinely transformed texture.
// Documentation: texture.txt

#ifndef PASTEL_TRANSFORM_TEXTURE_H
#define PASTEL_TRANSFORM_TEXTURE_H

#include "pastel/gfx/texture.h"

#include "pastel/math/affinetransformation_tools.h"

namespace Pastel
{

	template <typename Type>
	class Transform_Texture
		: public Texture<Type>
	{
	public:
		Transform_Texture(
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
	Transform_Texture<Type> transformTexture(
		const Texture<Type>& texture,
		const AffineTransformation2& transformation)
	{
		return Transform_Texture<Type>(texture, transformation);
	}

}

#endif
