// Description: Texture class
// Detail: An abstract class for textures

#ifndef PASTELGFX_TEXTURE_H
#define PASTELGFX_TEXTURE_H

#include "pastel/sys/vector.h"

#include "pastel/math/matrix.h"

#include <string>

namespace Pastel
{

	template <typename Type, int N = 2>
	class Texture
	{
	public:
		typedef Type Element;

		virtual ~Texture() {}

		virtual Type operator()(
			const Vector<real, N>& p,
			const Matrix<real>& m) const = 0;

		virtual std::string name() const = 0;

		virtual std::string fullName() const
		{
			return name();
		}
	};

}

#endif
