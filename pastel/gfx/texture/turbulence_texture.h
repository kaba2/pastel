// Description: Turbulence texture
// Documentation: synthetic_textures.txt

#ifndef PASTELGFX_TURBULENCE_TEXTURE_H
#define PASTELGFX_TURBULENCE_TEXTURE_H

#include "pastel/gfx/texture/texture.h"
#include "pastel/gfx/noise.h"

namespace Pastel
{

	template <int N = 2>
	class Turbulence_Texture
		: public Texture<dreal, N>
	{
	public:
		Turbulence_Texture()
			: noise_()
			, octaves_(5)
			, lacunarity_(2)
		{
		}

		virtual ~Turbulence_Texture()
		{
		}

		virtual dreal operator()(
			const Vector<dreal, N>& p,
			const Matrix<dreal, N, N>& m) const
		{
			dreal scale = 1;
			dreal value = 0;
			dreal amplitude = 1;

			for (integer i = 0;i < octaves_;++i)
			{
				value += noise_(p * scale, m) * amplitude;
				amplitude /= lacunarity_;
				scale *= 2.01423;
			}

			return value / 2;
		}

		void setOctaves(integer octaves)
		{
			ENSURE_OP(octaves, >=, 0);
			octaves_ = octaves;
		}

		integer octaves() const
		{
			return octaves_;
		}

		void setLacunarity(dreal lacunarity)
		{
			lacunarity_ = lacunarity;
		}

		dreal lacunarity() const
		{
			return lacunarity_;
		}

		virtual std::string name() const
		{
			return std::string("Turbulence");
		}

	private:
		Noise_Texture<N> noise_;
		integer octaves_;
		dreal lacunarity_;
	};

	template <int N>
	Turbulence_Texture<N> turbulenceTexture()
	{
		return Turbulence_Texture<N>();
	}

}

#endif
