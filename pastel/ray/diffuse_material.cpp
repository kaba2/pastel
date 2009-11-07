#include "pastel/ray/diffuse_material.h"

#include "pastel/gfx/color_texture.h"

namespace Pastel
{

	Diffuse_Material::Diffuse_Material()
		: color_()
	{
		color_ = Texture<Spectrum>::Ptr(new Color_Texture<Spectrum>(Spectrum()));
	}

	Diffuse_Material::Diffuse_Material(const Texture<Spectrum>::Ptr& color)
		: color_(color)
	{
	}

	Diffuse_Material::~Diffuse_Material()
	{
	}

	Spectrum Diffuse_Material::brdf(
		const Vector2& q,
		const Vector2& qx,
		const Vector2& qy,
		const Vector3& in,
		const Vector3& out) const
	{
		const real f = in.z();
		if (f < 0)
		{
			return Spectrum();
		}
		
		return (*color_)(q, Matrix2(qx, qy)) * f;
	}

}
