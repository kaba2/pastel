#ifndef PASTELGFX_SAVEPCX_HPP
#define PASTELGFX_SAVEPCX_HPP

#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/color_palette.h"
#include "pastel/gfx/coloradapter.h"

#include "pastel/sys/adaptedarray.h"

#include <algorithm>

namespace Pastel
{

	template <typename Type, typename Adapter>
	bool saveBinaryPcx(
		const Array<Type, 2>& image,
		const std::string& fileName,
		const Adapter& adapter)
	{
		ConstAdaptedArray<2, bool, Type, Adapter> adaptedImage(
			image, adapter);

		return saveBinaryPcx(adaptedImage, fileName);
	}

	template <typename Type, typename Adapter>
	bool saveIndexedPcx(
		const Array<Type, 2>& image,
		const std::vector<Color>& palette,
		const std::string& fileName,
		const Adapter& adapter)
	{
		ConstAdaptedArray<2, uint8, Type, Adapter> adaptedImage(
			image, adapter);

		return saveIndexedPcx(adaptedImage, palette, fileName);
	}

	template <typename Type, typename Adapter>
	bool savePcx(
		const Array<Type, 2>& image,
		const std::string& fileName,
		const Adapter& adapter)
	{
		ConstAdaptedArray<2, Color, Type, Adapter> adaptedImage(
			image, adapter);

		return savePcx(adaptedImage, fileName);
	}

	template <typename Real>
	bool saveGrayscalePcx(
		const Array<Real>& image,
		const std::string& fileName,
		bool maximizeContrast)
	{
		Real maxValue = image.empty() ? 1 :

			std::max(*std::max_element(image.cbegin(), image.cend()), (Real)0);

		ReverseAdapter<Integer_To_Real<Real, uint8> > adapter(
			Integer_To_Real<Real, uint8>(256, maxValue));

		real32 paletteMaxValue = maximizeContrast ? 
			1 : maxValue;

		std::vector<Color> palette;
		grayscalePalette(palette, 256, paletteMaxValue);

		return saveIndexedPcx(image, palette, fileName, adapter);
	}

	template <typename Real>
	bool saveRainbowPcx(
		const Array<Real>& image,
		const std::string& fileName,
		bool maximizeContrast)
	{
		Real maxValue = 

			std::max(*std::max_element(image.begin(), image.end()), (Real)0);

		ReverseAdapter<Integer_To_Real<Real, uint8> > adapter(
			Integer_To_Real<Real, uint8>(256, maxValue));

		real32 paletteMaxValue = maximizeContrast ? 
			1 : maxValue;

		std::vector<Color> palette;
		smoothFalseColorPalette(palette, 256);

		return saveIndexedPcx(image, palette, fileName, adapter);
	}

}

#endif
