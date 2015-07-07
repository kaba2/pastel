#ifndef PASTELGFX_LOADPCX_HPP
#define PASTELGFX_LOADPCX_HPP

#include "pastel/gfx/image_file/pcx/loadpcx.h"

#include "pastel/sys/array.h"
#include "pastel/sys/abstractarray.h"
#include "pastel/sys/adaptedarray.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/endian.h"
#include "pastel/sys/logging/log.h"
#include "pastel/sys/binaryfile.h"

#include "pastel/gfx/image_file/pcx/pcxheader.h"
#include "pastel/gfx/color/color_tools.h"
#include "pastel/gfx/color/coloradapter.h"
#include "pastel/gfx/color/color_palette.h"

#include <memory>

namespace Pastel
{

	namespace Pcx_
	{

		inline bool loadHeader(
			const std::string& fileName,
			BinaryFile& file,
			PcxHeader& pcxHeader,
			std::vector<Color>& palette)
		{
			if (fileName.empty())
			{
				log() << "No fileName given, aborting..."
					<< logNewLine;
				return false;
			}

			file.open(fileName.c_str(), true, false);
			if (!file.isOpen())
			{
				log() << "File " << fileName
					<< " could not be opened for reading. Aborting..."
					<< logNewLine;

				return false;
			}

			// Read the pcx header.

			pcxHeader.read(file);

			// Make sure that the encoding
			// uses run-length encoding
			// (there is no other known encoding
			// technique for pcx's).

			if (pcxHeader.encoding_ != 1)
			{
				log() << "Unsupported encoding type. Is "
					<< (integer)pcxHeader.encoding_
					<< ", must be 1. Aborting..."
					<< logNewLine;
				return false;
			}

			// Make sure the bits per pixel
			// is in legal range.

			if (pcxHeader.bitsPerPixel_ != 1 &&
				pcxHeader.bitsPerPixel_ != 2 &&
				pcxHeader.bitsPerPixel_ != 4 &&
				pcxHeader.bitsPerPixel_ != 8)
			{
				log() << "Unsupported number of bits per pixel. Is "
					<< (integer)pcxHeader.bitsPerPixel_
					<< ", must be 1, 2, 4, or 8. Aborting..."
					<< logNewLine;
				return false;
			}

			// Make sure the number of color planes
			// is in legal range.

			if (pcxHeader.colorPlanes_ != 1 &&
				pcxHeader.colorPlanes_ != 3)
			{
				log() << "Unsupported number of color planes. Is "
					<< (integer)pcxHeader.colorPlanes_ << ", must be 1 or 3. Aborting..."
					<< logNewLine;
				return false;
			}

			if (pcxHeader.colorPlanes_ == 3 && pcxHeader.bitsPerPixel_ != 8)
			{
				log() << "Unsupported color planes - bits per pixel combination: "
					<< "3 color planes, " << (integer)pcxHeader.bitsPerPixel_
					<< " bits per pixel. Aborting..."
					<< logNewLine;
				return false;
			}

			// Read the possible vga palette.

			integer colors = 1 << pcxHeader.bitsPerPixel_;

			palette.resize(colors);

			if (pcxHeader.bitsPerPixel_ == 1)
			{
				// Black and white palette for binary
				// images.
				palette[0].set(0);
				palette[1].set(1);
			}
			else if (
				pcxHeader.bitsPerPixel_ == 2 ||
				pcxHeader.bitsPerPixel_ == 4)
			{
				// For 2- and 4-bit images, use
				// the setExtent of the 16 color palette.

				integer toCopy = std::min(colors, (integer)16);
				for (integer i = 0;i < toCopy;++i)
				{
					palette[i] =
						Color(
						pcxHeader.palette16_[i * 3 + 0],
						pcxHeader.palette16_[i * 3 + 1],
						pcxHeader.palette16_[i * 3 + 2]) / 255;
				}
			}
			else
			{
				// For 8-bit images, check if there
				// is the extended vga palette available.

				bool foundPalette = false;
				if (pcxHeader.version_ >= 5)
				{
					file.setOffset(-769);
					uint8 paletteCode = 0;
					file >> paletteCode;
					if (paletteCode == 12)
					{
						foundPalette = true;
						uint8 red = 0;
						uint8 green = 0;
						uint8 blue = 0;
						for (integer i = 0;i < colors;++i)
						{
							file >> red >> green >> blue;

							palette[i] =
								Color(red, green, blue) / 255;
						}
					}
				}

				if (!foundPalette)
				{
					// If there is no palette,
					// use the default vga palette.

					vgaPalette(palette);
				}
			}

			file.setOffset(128);

			return true;
		}

		inline void loadScanline(
			BinaryFile& file,
			integer amount,
			std::vector<uint8>& scanline)
		{
			ASSERT1(amount >= 0, amount);
			ASSERT2(scanline.size() >= amount, scanline.size(), amount);

			integer bytesDecoded = 0;
			while (bytesDecoded < amount)
			{
				uint8 aData = 0;
				file >> aData;

				// If the two most signifact bits are set,
				// we have run-length encoding.
				if ((aData & 0xC0) == 0xC0)
				{
					// The 6 lower bits is
					// the repetition count

					// Extract repetition count
					// It can also be 0.
					const integer repetition = aData & 0x3F;

					// The next byte represents the
					// data that is to be repeated.

					uint8 bData = 0;
					file >> bData;

					for (integer i = 0;i < repetition;++i)
					{
						scanline[bytesDecoded] = bData;
						++bytesDecoded;
					}
				}
				else
				{
					// This is just normal data.

					scanline[bytesDecoded] = aData;
					++bytesDecoded;
				}
			}
		}

	}

	inline bool loadIndexedPcx(
		const std::string& fileName,
		AbstractArray<2, uint8>& image,
		std::vector<Color>* colorPalette)
	{
		BinaryFile file;
		PcxHeader pcxHeader;
		std::vector<Color> palette;

		if (!Pcx_::loadHeader(fileName, file, pcxHeader, palette))
		{
			return false;
		}

		if (pcxHeader.colorPlanes_ != 1)
		{
			log() << "An unsupported number of color planes for an indexed image. Is "
				<< (integer)pcxHeader.colorPlanes_ << ", must be 1. Aborting..."
				<< logNewLine;
			return false;
		}

		integer bytesPerScanline =
			(integer)pcxHeader.bytesPerScanlinePerPlane_ * (integer)pcxHeader.colorPlanes_;

		std::vector<uint8> scanline(bytesPerScanline, 0);

		integer width = pcxHeader.width();
		integer height = pcxHeader.height();

		image.setExtent(Vector2i(width, height));

		for (integer yPos = height - 1;yPos >= 0;--yPos)
		{
			Pcx_::loadScanline(file, bytesPerScanline, scanline);

			switch(pcxHeader.bitsPerPixel_)
			{
			case 1:
				{
					integer bytesToRead = (pcxHeader.width() + 7) / 8;

					integer xPos = 0;
					for (integer i = 0;i < bytesToRead - 1;++i)
					{
						uint8 data = scanline[i];

						image.write(Vector2i(xPos, yPos), (data >> 7) & 1);
						++xPos;
						image.write(Vector2i(xPos, yPos), (data >> 6) & 1);
						++xPos;
						image.write(Vector2i(xPos, yPos), (data >> 5) & 1);
						++xPos;
						image.write(Vector2i(xPos, yPos), (data >> 4) & 1);
						++xPos;
						image.write(Vector2i(xPos, yPos), (data >> 3) & 1);
						++xPos;
						image.write(Vector2i(xPos, yPos), (data >> 2) & 1);
						++xPos;
						image.write(Vector2i(xPos, yPos), (data >> 1) & 1);
						++xPos;
						image.write(Vector2i(xPos, yPos), (data >> 0) & 1);
						++xPos;
					}

					uint8 data = scanline[bytesToRead - 1];
					if (xPos < width)
					{

						image.write(Vector2i(xPos, yPos), (data >> 7) & 1);
						++xPos;
					}
					if (xPos < width)
					{
						image.write(Vector2i(xPos, yPos), (data >> 6) & 1);
						++xPos;
					}
					if (xPos < width)
					{
						image.write(Vector2i(xPos, yPos), (data >> 5) & 1);
						++xPos;
					}
					if (xPos < width)
					{
						image.write(Vector2i(xPos, yPos), (data >> 4) & 1);
						++xPos;
					}
					if (xPos < width)
					{
						image.write(Vector2i(xPos, yPos), (data >> 3) & 1);
						++xPos;
					}
					if (xPos < width)
					{
						image.write(Vector2i(xPos, yPos), (data >> 2) & 1);
						++xPos;
					}
					if (xPos < width)
					{
						image.write(Vector2i(xPos, yPos), (data >> 1) & 1);
						++xPos;
					}
					if (xPos < width)
					{
						image.write(Vector2i(xPos, yPos), (data >> 0) & 1);
						++xPos;
					}
				}
				break;
			case 2:
				{
					integer bytesToRead = (pcxHeader.width() + 3) / 4;

					integer xPos = 0;
					for (integer i = 0;i < bytesToRead - 1;++i)
					{
						uint8 data = scanline[i];

						image.write(Vector2i(xPos, yPos), (data >> 6) & 3);
						++xPos;
						image.write(Vector2i(xPos, yPos), (data >> 4) & 3);
						++xPos;
						image.write(Vector2i(xPos, yPos), (data >> 2) & 3);
						++xPos;
						image.write(Vector2i(xPos, yPos), (data >> 0) & 3);
						++xPos;
					}

					uint8 data = scanline[bytesToRead - 1];
					if (xPos < width)
					{

						image.write(Vector2i(xPos, yPos), (data >> 6) & 3);
						++xPos;
					}
					if (xPos < width)
					{
						image.write(Vector2i(xPos, yPos), (data >> 4) & 3);
						++xPos;
					}
					if (xPos < width)
					{
						image.write(Vector2i(xPos, yPos), (data >> 2) & 3);
						++xPos;
					}
					if (xPos < width)
					{
						image.write(Vector2i(xPos, yPos), (data >> 0) & 3);
						++xPos;
					}
				}
				break;
			case 4:
				{
					integer bytesToRead = (pcxHeader.width() + 1) / 2;

					integer xPos = 0;
					for (integer i = 0;i < bytesToRead - 1;++i)
					{
						uint8 data = scanline[i];

						image.write(Vector2i(xPos, yPos), (data >> 4) & 15);
						++xPos;
						image.write(Vector2i(xPos, yPos), (data >> 0) & 15);
						++xPos;
					}

					uint8 data = scanline[bytesToRead - 1];
					if (xPos < width)
					{

						image.write(Vector2i(xPos, yPos), (data >> 4) & 15);
						++xPos;
					}
					if (xPos < width)
					{
						image.write(Vector2i(xPos, yPos), (data >> 0) & 15);
						++xPos;
					}
				}
				break;
			case 8:
				{
					integer bytesToRead = pcxHeader.width();

					integer xPos = 0;
					for (integer i = 0;i < bytesToRead;++i)
					{
						uint8 data = scanline[i];

						image.write(Vector2i(xPos, yPos), data);
						++xPos;
					}
				}
				break;
			};
		}

		file.close();

		if (colorPalette)
		{

			palette.swap(*colorPalette);
		}

		return true;
	}

	inline bool loadPcx(
		const std::string& fileName,
		AbstractArray<2, Color>& image)
	{
		BinaryFile file;
		PcxHeader pcxHeader;
		std::vector<Color> palette;

		if (!Pcx_::loadHeader(fileName, file, pcxHeader, palette))
		{
			return false;
		}

		integer bytesPerScanline =
			(integer)pcxHeader.bytesPerScanlinePerPlane_ * (integer)pcxHeader.colorPlanes_;

		std::vector<uint8> scanline(bytesPerScanline, 0);

		integer width = pcxHeader.width();
		integer height = pcxHeader.height();

		image.setExtent(Vector<integer, 2>(width, height));

		for (integer yPos = height - 1;yPos >= 0;--yPos)
		{
			Pcx_::loadScanline(file, bytesPerScanline, scanline);

			// Copy scanline to the image

			if (pcxHeader.colorPlanes_ == 1)
			{
				switch(pcxHeader.bitsPerPixel_)
				{
				case 1:
					{
						integer bytesToRead = (width + 7) / 8;

						integer xPos = 0;
						for (integer i = 0;i < bytesToRead - 1;++i)
						{
							uint8 data = scanline[i];

							image.write(Vector2i(xPos, yPos), palette[(data >> 7) & 1]);
							++xPos;
							image.write(Vector2i(xPos, yPos), palette[(data >> 6) & 1]);
							++xPos;
							image.write(Vector2i(xPos, yPos), palette[(data >> 5) & 1]);
							++xPos;
							image.write(Vector2i(xPos, yPos), palette[(data >> 4) & 1]);
							++xPos;
							image.write(Vector2i(xPos, yPos), palette[(data >> 3) & 1]);
							++xPos;
							image.write(Vector2i(xPos, yPos), palette[(data >> 2) & 1]);
							++xPos;
							image.write(Vector2i(xPos, yPos), palette[(data >> 1) & 1]);
							++xPos;
							image.write(Vector2i(xPos, yPos), palette[(data >> 0) & 1]);
							++xPos;
						}

						uint8 data = scanline[bytesToRead - 1];
						if (xPos < width)
						{

							image.write(Vector2i(xPos, yPos), palette[(data >> 7) & 1]);
							++xPos;
						}
						if (xPos < width)
						{
							image.write(Vector2i(xPos, yPos), palette[(data >> 6) & 1]);
							++xPos;
						}
						if (xPos < width)
						{
							image.write(Vector2i(xPos, yPos), palette[(data >> 5) & 1]);
							++xPos;
						}
						if (xPos < width)
						{
							image.write(Vector2i(xPos, yPos), palette[(data >> 4) & 1]);
							++xPos;
						}
						if (xPos < width)
						{
							image.write(Vector2i(xPos, yPos), palette[(data >> 3) & 1]);
							++xPos;
						}
						if (xPos < width)
						{
							image.write(Vector2i(xPos, yPos), palette[(data >> 2) & 1]);
							++xPos;
						}
						if (xPos < width)
						{
							image.write(Vector2i(xPos, yPos), palette[(data >> 1) & 1]);
							++xPos;
						}
						if (xPos < width)
						{
							image.write(Vector2i(xPos, yPos), palette[(data >> 0) & 1]);
							++xPos;
						}
					}
					break;
				case 2:
					{
						integer bytesToRead = (width + 3) / 4;

						integer xPos = 0;
						for (integer i = 0;i < bytesToRead - 1;++i)
						{
							uint8 data = scanline[i];

							image.write(Vector2i(xPos, yPos), palette[(data >> 6) & 3]);
							++xPos;
							image.write(Vector2i(xPos, yPos), palette[(data >> 4) & 3]);
							++xPos;
							image.write(Vector2i(xPos, yPos), palette[(data >> 2) & 3]);
							++xPos;
							image.write(Vector2i(xPos, yPos), palette[(data >> 0) & 3]);
							++xPos;
						}

						uint8 data = scanline[bytesToRead - 1];
						if (xPos < width)
						{

							image.write(Vector2i(xPos, yPos), palette[(data >> 6) & 3]);
							++xPos;
						}
						if (xPos < width)
						{
							image.write(Vector2i(xPos, yPos), palette[(data >> 4) & 3]);
							++xPos;
						}
						if (xPos < width)
						{
							image.write(Vector2i(xPos, yPos), palette[(data >> 2) & 3]);
							++xPos;
						}
						if (xPos < width)
						{
							image.write(Vector2i(xPos, yPos), palette[(data >> 0) & 3]);
							++xPos;
						}
					}
					break;
				case 4:
					{
						integer bytesToRead = (width + 1) / 2;

						integer xPos = 0;
						for (integer i = 0;i < bytesToRead - 1;++i)
						{
							uint8 data = scanline[i];

							image.write(Vector2i(xPos, yPos), palette[(data >> 4) & 15]);
							++xPos;
							image.write(Vector2i(xPos, yPos), palette[(data >> 0) & 15]);
							++xPos;
						}

						uint8 data = scanline[bytesToRead - 1];
						if (xPos < width)
						{

							image.write(Vector2i(xPos, yPos), palette[(data >> 4) & 15]);
							++xPos;
						}
						if (xPos < width)
						{
							image.write(Vector2i(xPos, yPos), palette[(data >> 0) & 15]);
							++xPos;
						}
					}
					break;
				case 8:
					{
						integer bytesToRead = width;

						integer xPos = 0;
						for (integer i = 0;i < bytesToRead;++i)
						{
							uint8 data = scanline[i];

							image.write(Vector2i(xPos, yPos), palette[data]);
							++xPos;
						}
					}
					break;
				};
			}
			else if (pcxHeader.colorPlanes_ == 3)
			{

				uint8 *scanlineRed = &scanline[0];
				uint8 *scanlineGreen =
					&scanline[pcxHeader.bytesPerScanlinePerPlane_];
				uint8 *scanlineBlue =
					&scanline[pcxHeader.bytesPerScanlinePerPlane_ * 2];

				for (integer xPos = 0;xPos < width;++xPos)
				{
					image.write(Vector2i(xPos, yPos),
						Color(scanlineRed[xPos],
						scanlineGreen[xPos],
						scanlineBlue[xPos]) / 255);
				}
			}
		}

		file.close();

		return true;
	}

	inline bool loadIndexedPcx(
		const std::string& fileName,
		Array<bool, 2>& image,
		std::vector<Color>* colorPalette)
	{
		Integer_To_Bool<uint8> adapter;
		return loadIndexedPcx(fileName, image, adapter, colorPalette);
	}

	inline bool loadIndexedPcx(
		const std::string& fileName,
		Array<uint8, 2>& image,
		std::vector<Color>* colorPalette)
	{
		IdentityAdapter<uint8> adapter;
		return loadIndexedPcx(fileName, image, adapter, colorPalette);
	}

	inline bool loadPcx(
		const std::string& fileName,
		Array<Color, 2>& image)
	{
		IdentityAdapter<Color> adapter;
		return loadPcx(fileName, image, adapter);
	}

	inline bool loadPcx(
		const std::string& fileName,
		Array<real32>& image)
	{
		Luma_Color_Adapter adapter;
		return loadPcx(fileName, image, adapter);
	}

	inline bool loadPcx(
		const std::string& fileName,
		Array<ByteColor, 2>& image)
	{
		ByteColor_To_Color adapter;
		return loadPcx(fileName, image, adapter);
	}

	inline bool loadPcx(
		const std::string& fileName,
		Array<uint32, 2>& image)
	{
		Integer_To_Color<uint32, 8, 8, 8> adapter;
		return loadPcx(fileName, image, adapter);
	}

	inline bool loadPcx(
		const std::string& fileName,
		Array<uint16, 2>& image)
	{
		Integer_To_Color<uint16, 5, 6, 5> adapter;
		return loadPcx(fileName, image, adapter);
	}

}

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
