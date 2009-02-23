#ifndef PASTELGFX_SAVEPCX_H
#define PASTELGFX_SAVEPCX_H

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
		const Array<2, Type>& image,
		const std::string& fileName,
		const Adapter& adapter);

	PASTELGFX bool saveBinaryPcx(
		const Array<2, bool>& image,
		const std::string& fileName);

	PASTELGFX bool saveIndexedPcx(
		const ConstAbstractArray<2, uint8>& image,
		const std::vector<Color>& palette,
		const std::string& fileName);

	template <typename Type, typename Adapter>
	bool saveIndexedPcx(
		const Array<2, Type>& image,
		const std::vector<Color>& palette,
		const std::string& fileName,
		const Adapter& adapter);

	PASTELGFX bool saveIndexedPcx(
		const Array<2, uint8>& image,
		const std::vector<Color>& palette,
		const std::string& fileName);

	PASTELGFX bool savePcx(
		const ConstAbstractArray<2, Color>& image,
		const std::string& fileName);

	template <typename Type, typename Adapter>
	bool savePcx(
		const Array<2, Type>& image,
		const std::string& fileName,
		const Adapter& adapter);

	PASTELGFX bool savePcx(
		const Array<2, uint32>& image,
		const std::string& fileName);

	PASTELGFX bool savePcx(
		const Array<2, uint16>& image,
		const std::string& fileName);

	PASTELGFX bool savePcx(
		const Array<2, ByteColor>& image,
		const std::string& fileName);

	PASTELGFX bool savePcx(
		const Array<2, Color>& image,
		const std::string& fileName);

	PASTELGFX bool saveGrayscalePcx(
		const Array<2, real32>& image,
		const std::string& fileName);

}

#include "pastel/gfx/savepcx.hpp"

#endif
