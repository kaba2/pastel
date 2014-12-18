// Description: Rectangular subset of another texture
// Documentation: distortion_textures.txt

#ifndef PASTELGFX_SUB_TEXTURE_H
#define PASTELGFX_SUB_TEXTURE_H

#include "pastel/gfx/texture.h"

#include "pastel/geometry/shapes/alignedbox.h"

namespace Pastel
{

	template <typename Type, int N = 2>
	class Sub_Texture
		: public Texture<Type, N>
	{
	public:
		Sub_Texture()
			: texture_(0)
			, window_(Vector<integer, N>(0), Vector<integer, N>(1))
		{
		}

		Sub_Texture(
			const Texture<Type, N>& texture,
			const AlignedBox<real, N>& window)
			: texture_(&texture)
			, window_(window)
		{
		}

		virtual ~Sub_Texture()
		{
		}

		virtual Type operator()(
			const Vector<real, N>& p,
			const Matrix<real>& m) const
		{
			Matrix<real> mNew(m);
			for (integer i = 0;i < mNew.height();++i)
			{
				mNew.column(i) *= window_.extent(i);
			}

			return (*texture_)(window_.at(p), mNew);
		}

		virtual std::string name() const
		{
			return std::string("Sub");
		}

		virtual std::string fullName() const
		{
			return std::string("Sub(") + texture_->fullName() + ")";
		}

	private:
		const Texture<Type, N>* texture_;
		AlignedBox<real, N> window_;
	};

	template <typename Type, int N>
	Sub_Texture<Type, N> subTexture(
		const Texture<Type, N>& texture, 
		const AlignedBox<real, N>& window)
	{
		return Sub_Texture<Type, N>(texture, window);
	}

}

#endif
