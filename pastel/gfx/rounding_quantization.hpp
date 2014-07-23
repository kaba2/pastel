#ifndef PASTELGFX_ROUNDING_QUANTIZATION_HPP
#define PASTELGFX_ROUNDING_QUANTIZATION_HPP

#include "pastel/gfx/rounding_quantization.h"

namespace Pastel
{

	template <typename Vector>
	void quantizeRounding(
		Array<Vector, 2>& image,
		const PASTEL_NO_DEDUCTION(Vector)& interval)
	{
		ENSURE(allGreater(interval, 0));

		integer width = image.width();
		const integer height = image.height();


		Vector halfInterval(interval * 0.5);

		// By referring to floor without a namespace,
		// we make it possible to also find other
		// floor-functions such as the one for the
		// vectors.
		using std::floor;

		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				Vector value = image(x, y);
				Vector remainder =
					value - floor((value / interval)) * interval;

				image(x, y) += (interval - remainder) * (remainder >= halfInterval);
				image(x, y) -= remainder * (remainder < halfInterval);
			}
		}
	}

}

#endif
