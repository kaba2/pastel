// Description: LinearColor_Texture class
// Detail: A linear color patch texture.
// Documentation: texture.txt

#ifndef PASTEL_LINEARCOLOR_TEXTURE_H
#define PASTEL_LINEARCOLOR_TEXTURE_H

#include "pastel/gfx/texture.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <typename Type, int N = 2>
	class LinearColor_Texture
		: public Texture<Type, N>
	{
	public:
		LinearColor_Texture()
			: data_()
		{
		}

		explicit LinearColor_Texture(
			const Tuple<Type, ModifyN<N, 1 << N>::Result>& data)
			: data_(data)
		{
		}

		virtual ~LinearColor_Texture()
		{
		}

		virtual Type operator()(
			const Vector<real, N>& p,
			const Matrix<real, N, N>& m) const
		{
			return linear(p, 
				randomAccessRange(data_.begin(), data_.end()));
		}

		virtual std::string name() const
		{
			return std::string("LinearColor");
		}

	private:
		Tuple<Type, ModifyN<N, 1 << N>::Result> data_;
	};

	template <typename Type, int N>
	LinearColor_Texture<Type, N> linearColorTexture(
		const Tuple<PASTEL_NO_DEDUCTION(Type), ModifyN<N, 1 << N>::Result>& data)
	{
		return LinearColor_Texture<Type, N>(data);
	}

}

#endif
