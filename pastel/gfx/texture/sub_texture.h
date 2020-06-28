// Description: Rectangular subset of another texture
// Documentation: distortion_textures.txt

#ifndef PASTELGFX_SUB_TEXTURE_H
#define PASTELGFX_SUB_TEXTURE_H

#include "pastel/gfx/texture/texture.h"

#include "pastel/geometry/shape/alignedbox.h"

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
			const AlignedBox<dreal, N>& window)
			: texture_(&texture)
			, window_(window)
		{
		}

		virtual ~Sub_Texture()
		{
		}

		virtual Type operator()(
			const Vector<dreal, N>& p,
			const Matrix<dreal, N, N>& m) const
		{
			Matrix<dreal, N, N> mNew(m);
			for (integer i = 0;i < mNew.rows();++i)
			{
				mNew.col(i) *= window_.extent(i);
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
		AlignedBox<dreal, N> window_;
	};

	template <typename Type, int N>
	Sub_Texture<Type, N> subTexture(
		const Texture<Type, N>& texture, 
		const AlignedBox<dreal, N>& window)
	{
		return Sub_Texture<Type, N>(texture, window);
	}

}

#endif
