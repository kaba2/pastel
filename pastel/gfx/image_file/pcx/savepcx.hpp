#ifndef PASTELGFX_SAVEPCX_HPP
#define PASTELGFX_SAVEPCX_HPP

#include "pastel/gfx/image_file/pcx/savepcx.h"
#include "pastel/gfx/image_file/pcx/pcxheader.h"
#include "pastel/gfx/color/color_palette.h"
#include "pastel/gfx/color/coloradapter.h"
#include "pastel/gfx/color/color_tools.h"
#include "pastel/gfx/color/color_palette.h"
#include "pastel/gfx/color/coloradapter.h"

#include "pastel/sys/adaptedarray.h"
#include "pastel/sys/binaryfile.h"
#include "pastel/sys/tuple.h"
#include "pastel/sys/view/view_tools.h"
#include "pastel/sys/view/arrayview.h"
#include "pastel/sys/vector.h"

#include <algorithm>
#include <string>

namespace Pastel
{

	namespace Pcx_
	{

		inline void saveScanline(BinaryFile& file,
			const std::vector<uint8>& scanline,
			integer width)
		{
			integer xPos = 0;
			while(xPos < width)
			{
				uint8 aData = scanline[xPos];
				++xPos;

				integer repetition = 1;
				while (xPos < width && scanline[xPos] == aData)
				{
					++xPos;
					++repetition;
				}

				if (((aData & 0xC0) == 0xC0) ||
					repetition > 1)
				{
					while (repetition > 63)
					{
						file << (char)(0xC0 | 63);
						file << aData;
						repetition -= 63;
					}
					if (repetition > 0)
					{
						file << (char)(0xC0 | repetition);
						file << aData;
					}
				}
				else
				{
					file << aData;
				}
			}
		}

	}

	inline bool saveBinaryPcx(
		const ConstAbstractArray<2, bool>& image,
		const std::string& fileName)
	{
		// TODO: This does not work with empty images!

		/*
		std::cout << "Saving a binary image to file "
			<< fileName
			<< " in pcx format."
			<< std::endl;
		*/

		if (fileName.empty())
		{
			std::cout << "No filename given. Aborting..."
				<< std::endl;

			return false;
		}

		integer width = image.extent().x();
		integer height = image.extent().y();

		integer bytesPerScanlinePerPlane =
			roundUpToEven((width + 7) / 8);

		PcxHeader pcxHeader;

		pcxHeader.manufacturer_ = 10;
		pcxHeader.version_ = 5;
		pcxHeader.encoding_ = 1;
		pcxHeader.bitsPerPixel_ = 1;
		pcxHeader.leftMargin_ = 0;
		pcxHeader.upperMargin_ = 0;
		pcxHeader.rightMargin_ = (int16)(width - 1);
		pcxHeader.lowerMargin_ = (int16)(height - 1);
		pcxHeader.hss_ = 72;
		pcxHeader.vss_ = 72;
		pcxHeader.colorPlanes_ = 1;
		pcxHeader.bytesPerScanlinePerPlane_ =
			(uint16)bytesPerScanlinePerPlane;
		pcxHeader.paletteInfo_ = 1;

		BinaryFile file(fileName, false, true);

		// Define the palette.

		pcxHeader.palette16_[0 * 3 + 0] = 0;
		pcxHeader.palette16_[0 * 3 + 1] = 0;
		pcxHeader.palette16_[0 * 3 + 2] = 0;

		pcxHeader.palette16_[1 * 3 + 0] = 255;
		pcxHeader.palette16_[1 * 3 + 1] = 255;
		pcxHeader.palette16_[1 * 3 + 2] = 255;

		for (integer i = 2;i < 16;++i)
		{
			pcxHeader.palette16_[i * 3 + 0] = 0;
			pcxHeader.palette16_[i * 3 + 1] = 0;
			pcxHeader.palette16_[i * 3 + 2] = 0;
		}

		pcxHeader.write(file);

		integer bytesPerScanline =
			(integer)pcxHeader.bytesPerScanlinePerPlane_ * (integer)pcxHeader.colorPlanes_;

		std::vector<uint8> scanline(pcxHeader.bytesPerScanlinePerPlane_, 0);
		for (integer yPos = height - 1;yPos >= 0;--yPos)
		{
			for (integer xPos = 0;xPos < width;xPos += 8)
			{
				uint8 data = 0;
				integer i = 0;
				for (i = 0;i < 8 && xPos + i < width;++i)
				{
					data <<= 1;
					if (image(Vector2i(xPos + i, yPos)))
					{
						data += 1;
					}
				}
				data <<= 8 - i;

				scanline[xPos >> 3] = data;
			}

			Pcx_::saveScanline(file, scanline, bytesPerScanline);
		}

		file.close();

		//std::cout << "Pcx image saving complete." << std::endl;

		return true;
	}

	inline bool saveBinaryPcx(
		const Array<bool, 2>& image,
		const std::string& fileName)
	{
		IdentityAdapter<bool> adapter;
		return saveBinaryPcx(image, fileName, adapter);
	}

	inline bool saveIndexedPcx(
		const ConstAbstractArray<2, uint8>& image,
		const std::vector<Color>& colorPalette,
		const std::string& fileName)
	{
		// TODO: This does not work with empty images!

		/*
		std::cout << "Saving an indexed image to file "
			<< fileName << " in pcx format."
			<< std::endl;
		*/

		if (fileName.empty())
		{
			std::cout << "No filename given. Aborting..."
				<< std::endl;

			return false;
		}

		integer width = image.extent().x();
		integer height = image.extent().y();

		PcxHeader pcxHeader;

		pcxHeader.manufacturer_ = 10;
		pcxHeader.version_ = 5;
		pcxHeader.encoding_ = 1;
		pcxHeader.bitsPerPixel_ = 8;
		pcxHeader.leftMargin_ = 0;
		pcxHeader.upperMargin_ = 0;
		pcxHeader.rightMargin_ = (int16)(width - 1);
		pcxHeader.lowerMargin_ = (int16)(height - 1);
		pcxHeader.hss_ = 72;
		pcxHeader.vss_ = 72;
		pcxHeader.colorPlanes_ = 1;
		pcxHeader.bytesPerScanlinePerPlane_ =
			odd(width) ?
			(int16)(width + 1) : (int16)width;
		pcxHeader.paletteInfo_ = 1;

		integer colors = std::min((integer)16, (integer)colorPalette.size());

		for (integer i = 0;i < colors;++i)
		{
			Color color = fitColor(colorPalette[i]);

			pcxHeader.palette16_[i * 3 + 0] = quantizeUnsigned(color[0], 256);
			pcxHeader.palette16_[i * 3 + 1] = quantizeUnsigned(color[1], 256);
			pcxHeader.palette16_[i * 3 + 2] = quantizeUnsigned(color[2], 256);
		}

		for (integer i = colors;i < 16;++i)
		{
			pcxHeader.palette16_[i * 3 + 0] = 0;
			pcxHeader.palette16_[i * 3 + 1] = 0;
			pcxHeader.palette16_[i * 3 + 2] = 0;
		}

		BinaryFile file(fileName, false, true);

		pcxHeader.write(file);

		integer bytesPerScanline =
			(integer)pcxHeader.bytesPerScanlinePerPlane_ * (integer)pcxHeader.colorPlanes_;

		std::vector<uint8> scanline(pcxHeader.bytesPerScanlinePerPlane_, 0);
		for (integer yPos = height - 1;yPos >= 0;--yPos)
		{
			for (integer xPos = 0;xPos < width;++xPos)
			{
				scanline[xPos] = image(Vector2i(xPos, yPos));
			}

			Pcx_::saveScanline(file, scanline, bytesPerScanline);
		}

		file << (char)0xC;

		// There can be more than 256 colors in the palette,
		// so write at most 256 colors.

		integer colorsToWrite = std::min((integer)colorPalette.size(), (integer)256);

		for (integer i = 0;i < colorsToWrite;++i)
		{
			Color color = fitColor(colorPalette[i]);

			uint8 red = quantizeUnsigned(color[0], 256);
			uint8 green = quantizeUnsigned(color[1], 256);
			uint8 blue = quantizeUnsigned(color[2], 256);

			file << red << green << blue;
		}

		// If the palette does not define all 256 colors,
		// define the rest as (0, 0, 0).

		for (integer i = colorsToWrite;i < 256;++i)
		{
			file << (uint8)0;
			file << (uint8)0;
			file << (uint8)0;
		}

		file.close();

		//std::cout << "Pcx image saving complete." << std::endl;

		return true;
	}

	inline bool saveIndexedPcx(
		const Array<uint8, 2>& image,
		const std::vector<Color>& palette,
		const std::string& fileName)
	{
		IdentityAdapter<uint8> adapter;
		return saveIndexedPcx(image, palette, fileName, adapter);
	}

	inline bool savePcx(
		const ConstAbstractArray<2, Color>& image,
		const std::string& fileName)
	{
		// TODO: This does not work with empty images!

		/*
		std::cout << "Saving an rgb image to file "
			<< fileName
			<< " in pcx format."
			<< std::endl;
		*/

		if (fileName.empty())
		{
			std::cout << "No filename given. Aborting..."
				<< std::endl;

			return false;
		}

		integer width = image.extent().x();
		integer height = image.extent().y();

		PcxHeader pcxHeader;

		pcxHeader.manufacturer_ = 10;
		pcxHeader.version_ = 5;
		pcxHeader.encoding_ = 1;
		pcxHeader.bitsPerPixel_ = 8;
		pcxHeader.leftMargin_ = 0;
		pcxHeader.upperMargin_ = 0;
		pcxHeader.rightMargin_ = (int16)(width - 1);
		pcxHeader.lowerMargin_ = (int16)(height - 1);
		pcxHeader.hss_ = 72;
		pcxHeader.vss_ = 72;
		pcxHeader.colorPlanes_ = 3;
		pcxHeader.bytesPerScanlinePerPlane_ =
			odd(width) ?
			(int16)(width + 1) : (int16)width;
		pcxHeader.paletteInfo_ = 1;

		BinaryFile file(fileName, false, true);

		pcxHeader.write(file);

		integer bytesPerScanline =
			(integer)pcxHeader.bytesPerScanlinePerPlane_ * (integer)pcxHeader.colorPlanes_;

		std::vector<uint8> scanline(bytesPerScanline, 0);
		for (integer yPos = height - 1;yPos >= 0;--yPos)
		{
			for (integer xPos = 0;xPos < width;++xPos)
			{
				Color color = image(Vector2i(xPos, yPos));

				scanline[xPos] =
					quantizeUnsigned(color[0], 256);
				scanline[xPos + pcxHeader.bytesPerScanlinePerPlane_] =
					quantizeUnsigned(color[1], 256);

				scanline[xPos + pcxHeader.bytesPerScanlinePerPlane_ * 2] =
					quantizeUnsigned(color[2], 256);
			}

			Pcx_::saveScanline(file, scanline, bytesPerScanline);
		}

		file.close();

		//std::cout << "Pcx image saving complete." << std::endl;

		return true;
	}

	inline bool savePcx(
		const Array<uint32, 2>& image,
		const std::string& fileName)
	{
		Integer_To_Color<uint32, 8, 8, 8> adapter;
		return savePcx(image, fileName, adapter);
	}

	inline bool savePcx(
		const Array<uint16, 2>& image,
		const std::string& fileName)
	{
		Integer_To_Color<uint16, 5, 6, 5> adapter;
		return savePcx(image, fileName, adapter);
	}

	inline bool savePcx(
		const Array<Color, 2>& image,
		const std::string& fileName)
	{
		IdentityAdapter<Color> adapter;
		return savePcx(image, fileName, adapter);
	}

	inline bool savePcx(
		const Array<ByteColor, 2>& image,
		const std::string& fileName)
	{
		ByteColor_To_Color adapter;
		return savePcx(image, fileName, adapter);
	}

}

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

		std::vector<Color> palette;
		smoothFalseColorPalette(palette, 256);

		return saveIndexedPcx(image, palette, fileName, adapter);
	}

}

#endif
