// Description: Texture that affinely transforms another texture
// Documentation: distortion_textures.txt

#ifndef PASTEL_TRANSFORM_TEXTURE_H
#define PASTEL_TRANSFORM_TEXTURE_H

#include "pastel/gfx/texture.h"

#include "pastel/math/affinetransformation_tools.h"

namespace Pastel
{

	template <typename Type, int N = 2>
	class Transform_Texture
		: public Texture<Type, N>
	{
	public:
		Transform_Texture(
			const Texture<Type, N>& texture,
			const AffineTransformation2& transformation)
			: texture_(&texture)
			, invTransformation_(inverse(transformation))
		{
		}

		Type operator()(
			const Vector<real, N>& p,
			const Matrix<real, N, N>& m) const
		{
			return (*texture_)(
				transformPoint(p, invTransformation_),
				m * invTransformation_.transformation())
		}

		virtual std::string name() const
		{
			return std::string("Transform");
		}

	private:
		const Texture<Type, N>* texture_;
		AffineTransformation<real, N> invTransformation_;
	};

	template <typename Type, int N>
	Transform_Texture<Type, N> transformTexture(
		const Texture<Type, N>& texture,
		const AffineTransformation<real, N>& transformation)
	{
		return Transform_Texture<Type, N>(texture, transformation);
	}

}

#endif
