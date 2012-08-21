// Description: Diffusee material
// Documentation: materials.txt

#ifndef PASTEL_DIFFUSE_MATERIAL_H
#define PASTEL_DIFFUSE_MATERIAL_H

#include "pastel/ray/material.h"

#include "pastel/gfx/texture.h"

namespace Pastel
{

	class PASTELRAY Diffuse_Material
		: public Material
	{
	public:
		Diffuse_Material();

		explicit Diffuse_Material(
			const Texture<Spectrum>::Ptr& color);

		virtual ~Diffuse_Material();

		virtual Spectrum brdf(
			const Vector2& q,
			const Vector2& qx,
			const Vector2& qy,
			const Vector3& in,
			const Vector3& out) const;

	private:
		Texture<Spectrum>::Ptr color_;
	};

	typedef CountedPtr<Diffuse_Material> Diffuse_MaterialPtr;

}

#endif
