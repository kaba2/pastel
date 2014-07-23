#ifndef PASTELGFX_COMPONENT_LABELING_HPP
#define PASTELGFX_COMPONENT_LABELING_HPP

#include "pastel/gfx/component_labeling.h"

namespace Pastel
{

	template <
		typename Image_ConstView,
		typename Result_View
	>
	void labelConnectedComponents(
		const ConstView<2, bool, Image_ConstView>& image,
		const View<2, uint32, Result_View>& result)
	{
		integer width = image.width();
		integer height = image.height();

		result.setExtent(width, height);

		if (width == 0 ||
			height == 0)
		{
			return;
		}

		integer NoComponent = 0;
		integer nextComponent = 1;

		// Handle the bottom left pixel.

		if (image(0, 0))
		{
			result(0, 0) = nextComponent;
			++nextComponent;
		}
		else
		{
			result(0, 0) = 0;
		}

		// Handle the bottom row.

		for (integer x = 1;x < width;++x)
		{
			if (image(x, 0))
			{
				if (result(x - 1, 0) != NoComponent)
				{
					result(x, 0) = result(x - 1, 0);
				}
				else
				{
					result(x, 0) = nextComponent;
					++nextComponent;
				}
			}
			else
			{
				result(x, 0) = NoComponent;
			}
		}

		// Handle the rest of the pixels.

		for (integer y = 1;y < height;++y)
		{
			// Handle the left-most pixel.

			if (image(0, y))
			{
				const integer below = result(0, y - 1);
				if (below != NoComponent)
				{
					result(0, y) = below;
				}
				else
				{
					result(0, y) = nextComponent;
					++nextComponent;
				}
			}
			else
			{
				result(0, y) = NoComponent;
			}

			for (integer x = 1;x < width;++x)
			{
				if (image(x, y))
				{
					const integer left = result(x - 1, y);
					const integer below = result(x, y - 1);

					if (left != NoComponent)
					{
						if (below != NoComponent)
						{
							if (left == below)
							{
								result(x, y) = left;
							}
							else
							{
								// left and below components
								// are the same component with
								// different labeling.
							}
						}
						else
						{
							result(x, y) = left;
						}
					}
					else
					{
						if (below != NoComponent)
						{
							result(x, y) = below;
						}
						else
						{
							result(x, y) = nextComponent;
							++nextComponent;
						}
					}
				}
				else
				{
					result(x, y) = NoComponent;
				}
			}
		}
	}

}

#endif
