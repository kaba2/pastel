#include "pastel/gfx/palette.h"
#include "pastel/gfx/color_tools.h"

#include "pastel/sys/random.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	namespace
	{

		// The default vga palette as retrieved from the hardware.
		// The colors are stored as consecutive rgb-triples, with
		// components in the range [0, 64[ (the vga only has
		// 6 bits for each component).
		// Thanks to Daniel Pitts for extracting this information!

		uint8 defaultVgaPalette[4 * 64 * 3] =
		{
			0x00, 0x00, 0x00, 0x00, 0x00, 0x2A, 0x00, 0x2A, 0x00, 0x00, 0x2A, 0x2A, 
			0x2A, 0x00, 0x00, 0x2A, 0x00, 0x2A, 0x2A, 0x15, 0x00, 0x2A, 0x2A, 0x2A, 
			0x15, 0x15, 0x15, 0x15, 0x15, 0x3F, 0x15, 0x3F, 0x15, 0x15, 0x3F, 0x3F, 
			0x3F, 0x15, 0x15, 0x3F, 0x15, 0x3F, 0x3F, 0x3F, 0x15, 0x3F, 0x3F, 0x3F,
			0x00, 0x00, 0x00, 0x05, 0x05, 0x05, 0x08, 0x08, 0x08, 0x0B, 0x0B, 0x0B, 
			0x0E, 0x0E, 0x0E, 0x11, 0x11, 0x11, 0x14, 0x14, 0x14, 0x18, 0x18, 0x18, 
			0x1C, 0x1C, 0x1C, 0x20, 0x20, 0x20, 0x24, 0x24, 0x24, 0x28, 0x28, 0x28, 
			0x2D, 0x2D, 0x2D, 0x32, 0x32, 0x32, 0x38, 0x38, 0x38, 0x3F, 0x3F, 0x3F,
			0x00, 0x00, 0x3F, 0x10, 0x00, 0x3F, 0x1F, 0x00, 0x3F, 0x2F, 0x00, 0x3F, 
			0x3F, 0x00, 0x3F, 0x3F, 0x00, 0x2F, 0x3F, 0x00, 0x1F, 0x3F, 0x00, 0x10, 
			0x3F, 0x00, 0x00, 0x3F, 0x10, 0x00, 0x3F, 0x1F, 0x00, 0x3F, 0x2F, 0x00, 
			0x3F, 0x3F, 0x00, 0x2F, 0x3F, 0x00, 0x1F, 0x3F, 0x00, 0x10, 0x3F, 0x00,
			0x00, 0x3F, 0x00, 0x00, 0x3F, 0x10, 0x00, 0x3F, 0x1F, 0x00, 0x3F, 0x2F, 
			0x00, 0x3F, 0x3F, 0x00, 0x2F, 0x3F, 0x00, 0x1F, 0x3F, 0x00, 0x10, 0x3F, 
			0x1F, 0x1F, 0x3F, 0x27, 0x1F, 0x3F, 0x2F, 0x1F, 0x3F, 0x37, 0x1F, 0x3F, 
			0x3F, 0x1F, 0x3F, 0x3F, 0x1F, 0x37, 0x3F, 0x1F, 0x2F, 0x3F, 0x1F, 0x27,
			0x3F, 0x1F, 0x1F, 0x3F, 0x27, 0x1F, 0x3F, 0x2F, 0x1F, 0x3F, 0x37, 0x1F, 
			0x3F, 0x3F, 0x1F, 0x37, 0x3F, 0x1F, 0x2F, 0x3F, 0x1F, 0x27, 0x3F, 0x1F, 
			0x1F, 0x3F, 0x1F, 0x1F, 0x3F, 0x27, 0x1F, 0x3F, 0x2F, 0x1F, 0x3F, 0x37, 
			0x1F, 0x3F, 0x3F, 0x1F, 0x37, 0x3F, 0x1F, 0x2F, 0x3F, 0x1F, 0x27, 0x3F,
			0x2D, 0x2D, 0x3F, 0x31, 0x2D, 0x3F, 0x36, 0x2D, 0x3F, 0x3A, 0x2D, 0x3F, 
			0x3F, 0x2D, 0x3F, 0x3F,	0x2D, 0x3A, 0x3F, 0x2D, 0x36, 0x3F, 0x2D, 0x31, 
			0x3F, 0x2D, 0x2D, 0x3F, 0x31, 0x2D, 0x3F, 0x36,	0x2D, 0x3F, 0x3A, 0x2D, 
			0x3F, 0x3F, 0x2D, 0x3A, 0x3F, 0x2D, 0x36, 0x3F, 0x2D, 0x31, 0x3F, 0x2D,
			0x2D, 0x3F, 0x2D, 0x2D, 0x3F, 0x31, 0x2D, 0x3F, 0x36, 0x2D, 0x3F, 0x3A, 
			0x2D, 0x3F, 0x3F, 0x2D,	0x3A, 0x3F, 0x2D, 0x36, 0x3F, 0x2D, 0x31, 0x3F, 
			0x00, 0x00, 0x1C, 0x07, 0x00, 0x1C, 0x0E, 0x00,	0x1C, 0x15, 0x00, 0x1C, 
			0x1C, 0x00, 0x1C, 0x1C, 0x00, 0x15, 0x1C, 0x00, 0x0E, 0x1C, 0x00, 0x07,
			0x1C, 0x00, 0x00, 0x1C, 0x07, 0x00, 0x1C, 0x0E, 0x00, 0x1C, 0x15, 0x00, 
			0x1C, 0x1C, 0x00, 0x15,	0x1C, 0x00, 0x0E, 0x1C, 0x00, 0x07, 0x1C, 0x00, 
			0x00, 0x1C, 0x00, 0x00, 0x1C, 0x07, 0x00, 0x1C,	0x0E, 0x00, 0x1C, 0x15, 
			0x00, 0x1C, 0x1C, 0x00, 0x15, 0x1C, 0x00, 0x0E, 0x1C, 0x00, 0x07, 0x1C,
			0x0E, 0x0E, 0x1C, 0x11, 0x0E, 0x1C, 0x15, 0x0E, 0x1C, 0x18, 0x0E, 0x1C, 
			0x1C, 0x0E, 0x1C, 0x1C,	0x0E, 0x18, 0x1C, 0x0E, 0x15, 0x1C, 0x0E, 0x11, 
			0x1C, 0x0E, 0x0E, 0x1C, 0x11, 0x0E, 0x1C, 0x15,	0x0E, 0x1C, 0x18, 0x0E, 
			0x1C, 0x1C, 0x0E, 0x18, 0x1C, 0x0E, 0x15, 0x1C, 0x0E, 0x11, 0x1C, 0x0E,
			0x0E, 0x1C, 0x0E, 0x0E, 0x1C, 0x11, 0x0E, 0x1C, 0x15, 0x0E, 0x1C, 0x18, 
			0x0E, 0x1C, 0x1C, 0x0E,	0x18, 0x1C, 0x0E, 0x15, 0x1C, 0x0E, 0x11, 0x1C, 
			0x14, 0x14, 0x1C, 0x16, 0x14, 0x1C, 0x18, 0x14,	0x1C, 0x1A, 0x14, 0x1C, 
			0x1C, 0x14, 0x1C, 0x1C, 0x14, 0x1A, 0x1C, 0x14, 0x18, 0x1C, 0x14, 0x16,
			0x1C, 0x14, 0x14, 0x1C, 0x16, 0x14, 0x1C, 0x18, 0x14, 0x1C, 0x1A, 0x14, 
			0x1C, 0x1C, 0x14, 0x1A,	0x1C, 0x14, 0x18, 0x1C, 0x14, 0x16, 0x1C, 0x14, 
			0x14, 0x1C, 0x14, 0x14, 0x1C, 0x16, 0x14, 0x1C,	0x18, 0x14, 0x1C, 0x1A, 
			0x14, 0x1C, 0x1C, 0x14, 0x1A, 0x1C, 0x14, 0x18, 0x1C, 0x14, 0x16, 0x1C,
			0x00, 0x00, 0x10, 0x04, 0x00, 0x10, 0x08, 0x00, 0x10, 0x0C, 0x00, 0x10, 
			0x10, 0x00, 0x10, 0x10,	0x00, 0x0C, 0x10, 0x00, 0x08, 0x10, 0x00, 0x04, 
			0x10, 0x00, 0x00, 0x10, 0x04, 0x00, 0x10, 0x08,	0x00, 0x10, 0x0C, 0x00, 
			0x10, 0x10, 0x00, 0x0C, 0x10, 0x00, 0x08, 0x10, 0x00, 0x04, 0x10, 0x00,
			0x00, 0x10, 0x00, 0x00, 0x10, 0x04, 0x00, 0x10, 0x08, 0x00, 0x10, 0x0C, 
			0x00, 0x10, 0x10, 0x00,	0x0C, 0x10, 0x00, 0x08, 0x10, 0x00, 0x04, 0x10, 
			0x08, 0x08, 0x10, 0x0A, 0x08, 0x10, 0x0C, 0x08,	0x10, 0x0E, 0x08, 0x10, 
			0x10, 0x08, 0x10, 0x10, 0x08, 0x0E, 0x10, 0x08, 0x0C, 0x10, 0x08, 0x0A,
			0x10, 0x08, 0x08, 0x10, 0x0A, 0x08, 0x10, 0x0C, 0x08, 0x10, 0x0E, 0x08, 
			0x10, 0x10, 0x08, 0x0E,	0x10, 0x08, 0x0C, 0x10, 0x08, 0x0A, 0x10, 0x08, 
			0x08, 0x10, 0x08, 0x08, 0x10, 0x0A, 0x08, 0x10,	0x0C, 0x08, 0x10, 0x0E, 
			0x08, 0x10, 0x10, 0x08, 0x0E, 0x10, 0x08, 0x0C, 0x10, 0x08, 0x0A, 0x10,
			0x0B, 0x0B, 0x10, 0x0C, 0x0B, 0x10, 0x0D, 0x0B, 0x10, 0x0F, 0x0B, 0x10, 
			0x10, 0x0B, 0x10, 0x10,	0x0B, 0x0F, 0x10, 0x0B, 0x0D, 0x10, 0x0B, 0x0C, 
			0x10, 0x0B, 0x0B, 0x10, 0x0C, 0x0B, 0x10, 0x0D,	0x0B, 0x10, 0x0F, 0x0B, 
			0x10, 0x10, 0x0B, 0x0F, 0x10, 0x0B, 0x0D, 0x10, 0x0B, 0x0C, 0x10, 0x0B,
			0x0B, 0x10, 0x0B, 0x0B, 0x10, 0x0C, 0x0B, 0x10, 0x0D, 0x0B, 0x10, 0x0F, 
			0x0B, 0x10, 0x10, 0x0B,	0x0F, 0x10, 0x0B, 0x0D, 0x10, 0x0B, 0x0C, 0x10, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		};

	}
	
	PASTELGFX void vgaPalette(std::vector<Color>& palette)
	{
		std::vector<Color> resultPalette;
		resultPalette.reserve(256);
		for (integer i = 0;i < 256;++i)
		{
			resultPalette.push_back(
				Color(
				defaultVgaPalette[i * 3],
				defaultVgaPalette[i * 3 + 1],
				defaultVgaPalette[i * 3 + 2]) / 63);
		}

		resultPalette.swap(palette);
	}		
			
	PASTELGFX void randomPalette(std::vector<Color>& palette, integer count)
    {		
        ENSURE1(count >= 0, count);

        std::vector<Color> resultPalette;

        if (count > 0)
        {
            resultPalette.reserve(count);
            resultPalette.push_back(Color(0));
            
            for (integer i = 1;i < count;++i)
            {
                resultPalette.push_back(randomRgbColor());
            }
        }

        resultPalette.swap(palette);
    }

    PASTELGFX void grayscalePalette(std::vector<Color>& palette, integer count)
    {
        ENSURE1(count >= 0, count);

        std::vector<Color> resultPalette;
        resultPalette.reserve(count);
        
        for (integer i = 0;i < count;++i)
        {
            resultPalette.push_back(Color((real32)i / (count - 1)));
        }

        resultPalette.swap(palette);
    }

    PASTELGFX void smoothFalseColorPalette(std::vector<Color>& palette, integer count)
    {
        ENSURE1(count >= 0, count);

        std::vector<Color> resultPalette;

		if (count > 0)
        {
			resultPalette.resize(count);
			resultPalette[0].set(0);
		}
		if (count > 1)
		{
			// We don't want to go through all hues, because
			// hues are circular. Rather, we stop at 5/6,
			// which is the peak of violet.

			const real32 hueStep = ((real32)5 / 6) / (count - 1);
			real32 hue = 0;

            for (integer i = 1;i < count;++i)
            {
				resultPalette[i] = hsvToRgb(Color(hue, 1, 1));
				hue += hueStep;
            }
        }

        resultPalette.swap(palette);
    }

	PASTELGFX void falseColorPalette(std::vector<Color>& palette, integer count, integer seed)
    {
        ENSURE1(count >= 0, count);

        RandomIntegerGenerator generator(seed);

        std::vector<Color> resultPalette;

		smoothFalseColorPalette(resultPalette, count);

        for (integer i = 1;i < count;++i)
        {
			const integer k = 1 + generator() % (count - 1);
			std::swap(resultPalette[i], resultPalette[k]);
        }

        resultPalette.swap(palette);
    }

}
