// Description: Saving of images in PCX format

#ifndef PASTEL_SAVEPCX_H
#define PASTEL_SAVEPCX_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/color.h"

#include "pastel/sys/abstractarray.h"
#include "pastel/sys/array.h"

#include <string>

namespace Pastel
{

	PASTELGFX bool saveBinaryPcx(
		const ConstAbstractArray<2, bool>& image,
		const std::string& fileName);

	template <typename Type, typename Adapter>
	bool saveBinaryPcx(
		const Array<Type, 2>& image,
		const std::string& fileName,
		const Adapter& adapter);

	PASTELGFX bool saveBinaryPcx(
		const Array<bool, 2>& image,
		const std::string& fileName);

	PASTELGFX bool saveIndexedPcx(
		const ConstAbstractArray<2, uint8>& image,
		const std::vector<Color>& palette,
		const std::string& fileName);

	template <typename Type, typename Adapter>
	bool saveIndexedPcx(
		const Array<Type, 2>& image,
		const std::vector<Color>& palette,
		const std::string& fileName,
		const Adapter& adapter);

	PASTELGFX bool saveIndexedPcx(
		const Array<uint8, 2>& image,
		const std::vector<Color>& palette,
		const std::string& fileName);

	PASTELGFX bool savePcx(
		const ConstAbstractArray<2, Color>& image,
		const std::string& fileName);

	template <typename Type, typename Adapter>
	bool savePcx(
		const Array<Type, 2>& image,
		const std::string& fileName,
		const Adapter& adapter);

	PASTELGFX bool savePcx(
		const Array<uint32, 2>& image,
		const std::string& fileName);

	PASTELGFX bool savePcx(
		const Array<uint16, 2>& image,
		const std::string& fileName);

	PASTELGFX bool savePcx(
		const Array<ByteColor, 2>& image,
		const std::string& fileName);

	PASTELGFX bool savePcx(
		const Array<Color, 2>& image,
		const std::string& fileName);

	PASTELGFX bool saveGrayscalePcx(
		const Array<real32, 2>& image,
		const std::string& fileName);

}

#include "pastel/gfx/savepcx.hpp"

#endif
