#ifndef PASTEL_LOADPCX_HPP
#define PASTEL_LOADPCX_HPP

#include "pastel/sys/array.h"
#include "pastel/sys/abstractarray.h"
#include "pastel/sys/adaptedarray.h"

#include <memory>

namespace Pastel
{

	template <typename Type, typename Adapter>
	bool loadPcx(
		const std::string& fileName,
		Array<Type, 2>& image,
		const Adapter& adapter)
	{
		AdaptedArray<2, Color, Type, Adapter> abstractImage(
			image, adapter);

		return loadPcx(fileName, abstractImage);
	}

	template <typename Type, typename Adapter>
	bool loadIndexedPcx(
		const std::string& fileName,
		Array<Type, 2>& image,
		const Adapter& adapter,
		std::vector<Color>* colorPalette)
	{
		AdaptedArray<2, uint8, Type, Adapter> abstractImage(
			image, adapter);

		return loadIndexedPcx(fileName, abstractImage, colorPalette);
	}

}

#endif
