// Description: BilinearTriangleTexture
// Detail: A triangle filled with a bilinear color patch 

#ifndef PASTEL_BILINEARTRIANGLETEXTURE_H
#define PASTEL_BILINEARTRIANGLETEXTURE_H

#include "pastel/gfx/texture.h"

#include "pastel/geometry/barycentric.h"

namespace Pastel
{

	template <typename Type>
	class BilinearTriangleTexture
		: public Texture<Type>
	{
	public:
		BilinearTriangleTexture()
			: colorTriangle_()
		{
		}

		explicit BilinearTriangleTexture(
			const Tuple<Type, 3>& colorTriangle)
			: colorTriangle_(colorTriangle)
		{
		}

		virtual ~BilinearTriangleTexture()
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
	BilinearTriangleTexture<Type> bilinearTriangleTexture(
		const Tuple<Type, 3>& colorTriangle)
	{
		return BilinearTriangleTexture<Type>(colorTriangle);
	}

}

#endif
