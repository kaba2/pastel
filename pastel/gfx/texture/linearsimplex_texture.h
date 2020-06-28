// Description: Simplex with linear coloring
// Documentation: synthetic_textures.txt

#ifndef PASTELGFX_LINEARSIMPLEX_TEXTURE_H
#define PASTELGFX_LINEARSIMPLEX_TEXTURE_H

#include "pastel/gfx/texture/texture.h"

#include "pastel/geometry/barycentric.h"

namespace Pastel
{

	template <typename Type, int N = 2>
	class LinearSimplex_Texture
		: public Texture<Type, N>
	{
	public:
		LinearSimplex_Texture()
			: colorSimplex_()
		{
		}

		explicit LinearSimplex_Texture(
			const Tuple<Type, AddN<N>>& colorSimplex)
			: colorSimplex_(colorSimplex)
		{
		}

		virtual ~LinearSimplex_Texture()
		{
		}

		virtual Type operator()(
			const Vector<dreal, N>& p,
			const Matrix<dreal, N, N>& m) const
		{
			integer n = p.size();
			PENSURE_OP(n, ==, m.cols());
			PENSURE_OP(n, ==, m.rows());

			Vector<dreal, AddN<N>> bary = 
				barycentric(p);

			if (anyLess(bary, 0))
			{
				return Type(0);
			}

			Type result = bary[0] * colorSimplex_[0];
			for (integer i = 1;i < n;++i)
			{
				result += bary[i] * colorSimplex_[i];
			}

			return result;
		}

		virtual std::string name() const
		{
			return std::string("LinearSimplex");
		}

	private:
		Tuple<Type, AddN<N>> colorSimplex_;
	};

	template <typename Type, int N>
	LinearSimplex_Texture<Type, SubN<N>> linearSimplexTexture(
		const Tuple<Type, N>& colorSimplex)
	{
		return LinearSimplex_Texture<Type, SubN<N>>(
			colorSimplex);
	}

}

#endif
