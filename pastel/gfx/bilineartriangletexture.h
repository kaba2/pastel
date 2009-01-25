#ifndef PASTELGFX_BILINEARTRIANGLETEXTURE_H
#define PASTELGFX_BILINEARTRIANGLETEXTURE_H

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
			const Tuple<3, Type>& colorTriangle)
			: colorTriangle_(colorTriangle)
		{
		}

		virtual ~BilinearTriangleTexture()
		{
		}

		virtual Type operator()(
			const Point2& p, 
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
		Tuple<3, Type> colorTriangle_;
	};

	template <typename Type>
	BilinearTriangleTexture<Type> bilinearTriangleTexture(
		const Tuple<3, Type>& colorTriangle)
	{
		return BilinearTriangleTexture<Type>(colorTriangle);
	}

}

#endif
