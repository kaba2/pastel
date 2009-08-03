#ifndef PASTEL_QUANTIZE_HPP
#define PASTEL_QUANTIZE_HPP

#include "pastel/gfx/quantize.h"

namespace Pastel
{

	template <typename Real>
	void quantizeRounding(
		Array<Real, 2>& image,
		const Real& interval)
	{
		ENSURE_OP(interval, >, 0);

		const integer width = image.width();
		const integer height = image.height();

		Real halfInterval(interval * 0.5);

		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				Real value(image(x, y));
				Real remainder(value - std::floor((value / interval)) * interval);

				if (remainder >= halfInterval)
				{
					image(x, y) += (interval - remainder);
				}
				else
				{
					image(x, y) -= remainder;
				}
			}
		}
	}

	template <typename Real>
	void quantizeErrorDiffusion(
		Array<Real, 2>& image,
		const Real& interval)
	{
		ENSURE_OP(interval, >, 0);

		const integer width = image.width();
		const integer height = image.height();

		Real halfInterval(interval * 0.5);

		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				Real value(image(x, y));
				Real remainder(value - std::floor((value / interval)) * interval);
				Real error(remainder);

				if (remainder >= halfInterval)
				{
					error -=interval;
				}

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
