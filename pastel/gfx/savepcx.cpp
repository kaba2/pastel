#include "pastel/gfx/savepcx.h"
#include "pastel/gfx/pcxheader.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/palette.h"
#include "pastel/gfx/coloradapter.h"

#include "pastel/sys/log.h"
#include "pastel/sys/binaryfile.h"
#include "pastel/sys/tuple.h"
#include "pastel/sys/view_tools.h"
#include "pastel/sys/arrayview.h"

#include "pastel/sys/vector.h"

#include <string>

namespace Pastel
{

	namespace
	{

		void saveScanline(BinaryFile& file,
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

	PASTELGFX bool saveBinaryPcx(
		const ConstAbstractArray<2, bool>& image,
		const std::string& fileName)
	{
		// TODO: This does not work with empty images!

		log() << "Saving a binary image to file "
			<< fileName
			<< " in pcx format."
			<< logNewLine;

		if (fileName.empty())
		{
			log() << "No filename given. Aborting..."
				<< logNewLine;

			return false;
		}

		const integer width = image.extent().x();
		const integer height = image.extent().y();

		const integer bytesPerScanlinePerPlane =
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

		const integer bytesPerScanline =
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
					if (image(IPoint2(xPos + i, yPos)))
					{
						data += 1;
					}
				}
				data <<= 8 - i;

				scanline[xPos >> 3] = data;
			}

			saveScanline(file, scanline, bytesPerScanline);
		}

		file.close();

		log() << "Pcx image saving complete." << logNewLine;

		return true;
	}

	PASTELGFX bool saveBinaryPcx(
		const Array<2, bool>& image,
		const std::string& fileName)
	{
		IdentityAdapter<bool> adapter;
		return saveBinaryPcx(image, fileName, adapter);
	}

	PASTELGFX bool saveIndexedPcx(
		const ConstAbstractArray<2, uint8>& image,
		const std::vector<Color>& colorPalette,
		const std::string& fileName)
	{
		// TODO: This does not work with empty images!

		log() << "Saving an indexed image to file "
			<< fileName << " in pcx format."
			<< logNewLine;

		if (fileName.empty())
		{
			log() << "No filename given. Aborting..."
				<< logNewLine;

			return false;
		}

		const integer width = image.extent().x();
		const integer height = image.extent().y();

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

		const integer colors = std::min((integer)16, (integer)colorPalette.size());

		for (integer i = 0;i < colors;++i)
		{
			const Color color = fitColor(colorPalette[i]);

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

		const integer bytesPerScanline =
			(integer)pcxHeader.bytesPerScanlinePerPlane_ * (integer)pcxHeader.colorPlanes_;

		std::vector<uint8> scanline(pcxHeader.bytesPerScanlinePerPlane_, 0);
		for (integer yPos = height - 1;yPos >= 0;--yPos)
		{
			for (integer xPos = 0;xPos < width;++xPos)
			{
				scanline[xPos] = image(IPoint2(xPos, yPos));
			}

			saveScanline(file, scanline, bytesPerScanline);
		}

		file << (char)0xC;

		// There can be more than 256 colors in the palette,
		// so write at most 256 colors.

		const integer colorsToWrite = std::min((integer)colorPalette.size(), (integer)256);

		for (integer i = 0;i < colorsToWrite;++i)
		{
			const Color color = fitColor(colorPalette[i]);

			const uint8 red = quantizeUnsigned(color[0], 256);
			const uint8 green = quantizeUnsigned(color[1], 256);
			const uint8 blue = quantizeUnsigned(color[2], 256);

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

		log() << "Pcx image saving complete." << logNewLine;

		return true;
	}

	PASTELGFX bool saveIndexedPcx(
		const Array<2, uint8>& image,
		const std::vector<Color>& palette,
		const std::string& fileName)
	{
		IdentityAdapter<uint8> adapter;
		return saveIndexedPcx(image, palette, fileName, adapter);
	}

	PASTELGFX bool savePcx(
		const ConstAbstractArray<2, Color>& image,
		const std::string& fileName)
	{
		// TODO: This does not work with empty images!

		log() << "Saving an rgb image to file "
			<< fileName
			<< " in pcx format."
			<< logNewLine;

		if (fileName.empty())
		{
			log() << "No filename given. Aborting..."
				<< logNewLine;

			return false;
		}

		const integer width = image.extent().x();
		const integer height = image.extent().y();

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

		const integer bytesPerScanline =
			(integer)pcxHeader.bytesPerScanlinePerPlane_ * (integer)pcxHeader.colorPlanes_;

		std::vector<uint8> scanline(bytesPerScanline, 0);
		for (integer yPos = height - 1;yPos >= 0;--yPos)
		{
			for (integer xPos = 0;xPos < width;++xPos)
			{
				const Color color = image(IPoint2(xPos, yPos));

				scanline[xPos] =
					quantizeUnsigned(color[0], 256);
				scanline[xPos + pcxHeader.bytesPerScanlinePerPlane_] =
					quantizeUnsigned(color[1], 256);
				scanline[xPos + pcxHeader.bytesPerScanlinePerPlane_ * 2] =
					quantizeUnsigned(color[2], 256);
			}

			saveScanline(file, scanline, bytesPerScanline);
		}

		file.close();

		log() << "Pcx image saving complete." << logNewLine;

		return true;
	}

	PASTELGFX bool savePcx(
		const Array<2, uint32>& image,
		const std::string& fileName)
	{
		Color_Integer_Adapter<uint32, 8, 8, 8> adapter;
		return savePcx(image, fileName, adapter);
	}

	PASTELGFX bool savePcx(
		const Array<2, uint16>& image,
		const std::string& fileName)
	{
		Color_Integer_Adapter<uint16, 5, 6, 5> adapter;
		return savePcx(image, fileName, adapter);
	}

	PASTELGFX bool savePcx(
		const Array<2, Color>& image,
		const std::string& fileName)
	{
		IdentityAdapter<Color> adapter;
		return savePcx(image, fileName, adapter);
	}

	PASTELGFX bool savePcx(
		const Array<2, ByteColor>& image,
		const std::string& fileName)
	{
		Color_ByteColor_Adapter adapter;
		return savePcx(image, fileName, adapter);
	}

	PASTELGFX bool saveGrayscalePcx(
		const Array<2, real32>& image,
		const std::string& fileName)
	{
		Real_Integer_Adapter<real32, uint8> adapter(256);

		std::vector<Color> palette;
		grayscalePalette(palette, 256);

		return saveIndexedPcx(image, palette, fileName, adapter);
	}

}
