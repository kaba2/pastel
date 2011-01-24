// Description: Cube with linear coloring
// Documentation: synthetic_textures.txt

#ifndef PASTEL_LINEARCOLOR_TEXTURE_H
#define PASTEL_LINEARCOLOR_TEXTURE_H

#include "pastel/gfx/texture.h"

#include "pastel/sys/math_functions.h"
#include "pastel/sys/iteratorrange.h"

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

		template <typename Iterator>
		explicit LinearColor_Texture(
			const RandomAccessIterator_Range<Iterator>& data)
			: data_(data.begin(), data.end())
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
				range(data_.begin(), data_.end()));
		}

		virtual std::string name() const
		{
			return std::string("LinearColor");
		}

	private:
		std::vector<Type> data_;
	};

	template <typename Type, int N, typename Iterator>
	LinearColor_Texture<Type, N> linearColorTexture(
		const RandomAccessIterator_Range<Iterator>& data)
	{
		return LinearColor_Texture<Type, N>(data);
	}

}

#endif
