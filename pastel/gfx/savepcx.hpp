#ifndef PASTELGFX_SAVEPCX_HPP
#define PASTELGFX_SAVEPCX_HPP

#include "pastel/gfx/savepcx.h"

#include "pastel/sys/adaptedarray.h"

namespace Pastel
{

	template <typename Type, typename Adapter>
	bool saveBinaryPcx(
		const Array<2, Type>& image,
		const std::string& fileName,
		const Adapter& adapter)
	{
		ConstAdaptedArray<2, bool, Type, Adapter> adaptedImage(
			image, adapter);

		return saveBinaryPcx(adaptedImage, fileName);
	}

	template <typename Type, typename Adapter>
	bool saveIndexedPcx(
		const Array<2, Type>& image,
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
		const Array<2, Type>& image,
		const std::string& fileName,
		const Adapter& adapter)
	{
		ConstAdaptedArray<2, Color, Type, Adapter> adaptedImage(
			image, adapter);

		return savePcx(adaptedImage, fileName);
	}

}

#endif
