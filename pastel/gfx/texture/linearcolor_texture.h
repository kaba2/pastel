// Description: Cube with linear coloring
// Documentation: synthetic_textures.txt

#ifndef PASTELGFX_LINEARCOLOR_TEXTURE_H
#define PASTELGFX_LINEARCOLOR_TEXTURE_H

#include "pastel/gfx/texture/texture.h"

#include "pastel/sys/math_functions.h"
#include "pastel/sys/range.h"

#include <vector>

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

		template <typename Type_Range>
		explicit LinearColor_Texture(
			const Type_Range& data)
			: data_(data.begin(), data.end())
		{
		}

		virtual ~LinearColor_Texture()
		{
		}

		virtual Type operator()(
			const Vector<real, N>& p,
			const Matrix<real>& m) const
		{
			return linear(p, 
				range(data_.begin(), data_.end()));
		}

		virtual std::string name() const
		{
			return std::string("LinearColor");
		}

	private:
		std::vector<Type> data_;
	};

	template <typename Type, int N, typename Type_Range>
	LinearColor_Texture<Type, N> linearColorTexture(
		const Type_Range& data)
	{
		return LinearColor_Texture<Type, N>(data);
	}

}

#endif
