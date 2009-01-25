#ifndef PASTELSYS_BORDEREXTENDER_H
#define PASTELSYS_BORDEREXTENDER_H

#include "pastel/sys/arrayextender.h"
#include "pastel/sys/point_tools.h"
#include "pastel/sys/lineararray.h"

namespace Pastel
{

	class PASTELSYS BorderExtender
		: public IndexExtender
	{
	public:
		explicit BorderExtender(
		{
		}

		virtual ~BorderExtender()
		{
		}

		virtual Type operator()(
			const LinearArray<N, Type>& image,
			const Point<N, integer>& position) const
		{
			if (anyLess(position, 0) ||
				anyGreaterEqual(position, image.extent()))
			{
				return borderColor_;
			}

			return image(position);
		}

	private:
		Type borderColor_;
	};

}

#endif
