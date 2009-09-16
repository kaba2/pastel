// Description: Texture class
// Detail: An abstract class for textures

#ifndef PASTEL_TEXTURE_H
#define PASTEL_TEXTURE_H

#include "pastel/sys/vector.h"
#include "pastel/sys/countedptr.h"

#include <string>

namespace Pastel
{

	template <typename Type>
	class Texture
		: public ReferenceCounted
	{
	public:
		typedef CountedPtr<Texture> Ptr;
		typedef CountedPtr<const Texture> ConstPtr;
		typedef Type Element;

		virtual ~Texture() {}

		virtual Type operator()(
			const Vector2& p,
			const Vector2& dpDx,
			const Vector2& dpDy) const = 0;

		virtual std::string name() const = 0;
	};

}

#endif
