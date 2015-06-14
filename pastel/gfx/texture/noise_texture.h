// Description: Perlin's classic noise texture
// Documentation: synthetic_textures.txt

#ifndef PASTELGFX_NOISE_TEXTURE_H
#define PASTELGFX_NOISE_TEXTURE_H

#include "pastel/gfx/texture/texture.h"
#include "pastel/gfx/noise.h"

namespace Pastel
{

	template <integer N = 2>
	class Noise_Texture
		: public Texture<real, N>
	{
	public:
		Noise_Texture()
			: scale_(100)
			, translation_(0)
		{
		}

		virtual ~Noise_Texture()
		{
		}

		virtual real operator()(
			const Vector<real, N>& p,
			const Matrix<real>& m) const
		{
			return perlinNoise(
				evaluate(p * scale_ + translation_));
		}

		void setScale(real scale)
		{
			ENSURE_OP(scale, >=, 0);
			scale_ = scale;
		}

		real scale() const
		{
			return scale_;
		}

		void setTranslation(const Vector<real, N>& translation)
		{
			translation_ = translation;
		}

		const Vector<real, N>& translation() const
		{
			return translation_;
		}

		virtual std::string name() const
		{
			return std::string("Noise");
		}

	private:
		real scale_;
		Vector<real, N> translation_;
	};

	template <integer N>
	Noise_Texture<N> noiseTexture()
	{
		return Noise_Texture<N>();
	}

}

#endif
