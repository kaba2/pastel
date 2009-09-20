// Description: BilinearTriangle_Texture class
// Detail: A triangle filled with a bilinear color patch.
// Documentation: texture.txt

#ifndef PASTEL_BILINEARTRIANGLE_TEXTURE_H
#define PASTEL_BILINEARTRIANGLE_TEXTURE_H

#include "pastel/gfx/texture.h"

#include "pastel/geometry/barycentric.h"

namespace Pastel
{

	template <typename Type>
	class BilinearTriangle_Texture
		: public Texture<Type>
	{
	public:
		BilinearTriangle_Texture()
			: colorTriangle_()
		{
		}

		explicit BilinearTriangle_Texture(
			const Tuple<Type, 3>& colorTriangle)
			: colorTriangle_(colorTriangle)
		{
		}

		virtual ~BilinearTriangle_Texture()
		{
		}

		virtual Type operator()(
			const Vector2& p,
			const Vector2& dpdx,
			const Vector2& dpdy) const
		{
			const Vector3 bary = barycentric(p);

			Type result(0);

			for (integer i = 0;i < 3;++i)
			{
				result += bary[i] * colorTriangle_[i];
			}

			return result;
		}

		virtual std::string name() const
		{
			return std::string("BilinearTriangle");
		}

	private:
		Tuple<Type, 3> colorTriangle_;
	};

	template <typename Type>
	BilinearTriangle_Texture<Type> bilinearTriangleTexture(
		const Tuple<Type, 3>& colorTriangle)
	{
		return BilinearTriangle_Texture<Type>(colorTriangle);
	}

}

#endif
