#ifndef PASTELGFX_DIFFUSION_QUANTIZATION_HPP
#define PASTELGFX_DIFFUSION_QUANTIZATION_HPP

#include "pastel/gfx/diffusion_quantization.h"

namespace Pastel
{

	template <typename Vector>
	void quantizeErrorDiffusion(
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
				Vector error = 
					remainder - interval * (Vector)(remainder >= halfInterval);

				image(x, y) -= error;

				error /= 16;

				if (x < width - 1)
				{
					image(x + 1, y) += 7 * error;
				}
				if (y < height - 1)
				{
					if (x < width - 1)
					{
						image(x + 1, y + 1) += error;
					}
					image(x, y + 1) += 5 * error;
				}
				if (x > 0 && y < height - 1)
				{
					image(x - 1, y + 1) += 3 * error;
				}
			}
		}
	}

}

#endif
