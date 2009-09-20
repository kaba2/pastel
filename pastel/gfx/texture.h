// Description: Texture class
// Detail: An abstract class for textures

#ifndef PASTEL_TEXTURE_H
#define PASTEL_TEXTURE_H

#include "pastel/sys/vector.h"
#include "pastel/sys/countedptr.h"

#include "pastel/math/matrix.h"

#include <string>

namespace Pastel
{

	template <typename Type, int N = 2>
	class Texture
		: public ReferenceCounted
	{
	public:
		typedef CountedPtr<Texture> Ptr;
		typedef CountedPtr<const Texture> ConstPtr;
		typedef Type Element;

		virtual ~Texture() {}

		virtual Type operator()(
			const Vector<real, N>& p,
			const Matrix<real, N, N>& m) const = 0;

		virtual std::string name() const = 0;
	};

}

#endif
