// Description: Distortion of a texture from cartesian to spherical space

#ifndef PASTELGFX_RADIAL_TEXTURE_H
#define PASTELGFX_RADIAL_TEXTURE_H

#include "pastel/gfx/texture/texture.h"
#include "pastel/gfx/color/colormixer.h"

#include "pastel/math/coordinate/coordinates.h"
#include "pastel/math/coordinate/coordinates_derivatives.h"

namespace Pastel
{

	template <typename Type, integer N = 2>
	class Radial_Texture
		: public Texture<Type, N>
	{
	public:
		Radial_Texture(
			const Texture<Type, N>& texture,
			const Vector<real, N>& center,
			const Vector<real, N>& sphericalStart,
			const Vector<real, N>& sphericalEnd)
			: texture_(texture)
			, center_(center)
			, sphericalStart_(sphericalStart)
			, sphericalEnd_(sphericalEnd)
		{
		}

		virtual ~Radial_Texture()
		{
		}

		virtual Type operator()(
			const Vector<real, N>& p,
			const Matrix<real>& m) const
		{
			Vector<real, N> spherical =
				sphericalStart_ + (sphericalEnd_ - sphericalStart_) * p;

			return texture_(
				center_ + sphericalToCartesian(spherical),
				sphericalToCartesianDerivative(spherical) * m);
		}

		virtual std::string name() const
		{
			return std::string("Radial(" + texture_.fullName() + ")");
		}

	private:
		const Texture<Type, N>& texture_;
		Vector<real, N> center_;
		Vector<real, N> sphericalStart_;
		Vector<real, N> sphericalEnd_;
	};

	template <typename Type, integer N>
	Radial_Texture<Type, N> radialTexture(
		const Texture<Type, N>& texture,
		const Vector<real, N>& center,
		const Vector<real, N>& sphericalStart,
		const Vector<real, N>& sphericalEnd)
	{
		return Radial_Texture<Type, N>(texture, center, sphericalStart, sphericalEnd);
	}

}

#endif
