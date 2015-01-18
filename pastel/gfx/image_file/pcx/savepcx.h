// Description: Saving of images in PCX format
// Documentation: pcx.txt

#ifndef PASTELGFX_SAVEPCX_H
#define PASTELGFX_SAVEPCX_H

#include "pastel/gfx/color.h"

#include "pastel/sys/abstractarray.h"
#include "pastel/sys/array.h"

#include <string>

namespace Pastel
{

	bool saveBinaryPcx(
		const ConstAbstractArray<2, bool>& image,
		const std::string& fileName);

	template <typename Type, typename Adapter>
	bool saveBinaryPcx(
		const Array<Type, 2>& image,
		const std::string& fileName,
		const Adapter& adapter);

	bool saveBinaryPcx(
		const Array<bool, 2>& image,
		const std::string& fileName);

	bool saveIndexedPcx(
		const ConstAbstractArray<2, uint8>& image,
		const std::vector<Color>& palette,
		const std::string& fileName);

	template <typename Type, typename Adapter>
	bool saveIndexedPcx(
		const Array<Type, 2>& image,
		const std::vector<Color>& palette,
		const std::string& fileName,
		const Adapter& adapter);

	bool saveIndexedPcx(
		const Array<uint8, 2>& image,
		const std::vector<Color>& palette,
		const std::string& fileName);

	bool savePcx(
		const ConstAbstractArray<2, Color>& image,
		const std::string& fileName);

	template <typename Type, typename Adapter>
	bool savePcx(
		const Array<Type, 2>& image,
		const std::string& fileName,
		const Adapter& adapter);

	bool savePcx(
		const Array<uint32, 2>& image,
		const std::string& fileName);

	bool savePcx(
		const Array<uint16, 2>& image,
		const std::string& fileName);

	bool savePcx(
		const Array<ByteColor, 2>& image,
		const std::string& fileName);

	bool savePcx(
		const Array<Color, 2>& image,
		const std::string& fileName);

	template <typename Real>
	bool saveGrayscalePcx(
		const Array<Real>& image,
		const std::string& fileName,
		bool maximizeContrast = false);

}

#include "pastel/gfx/savepcx.hpp"

#endif
