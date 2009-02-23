#include "pastel/gfx/loadpcx.h"
#include "pastel/gfx/pcxheader.h"
#include "pastel/gfx/color_tools.h"
#include "pastel/gfx/coloradapter.h"
#include "pastel/gfx/palette.h"

#include "pastel/sys/ensure.h"
#include "pastel/sys/endianconvert.h"
#include "pastel/sys/log.h"
#include "pastel/sys/binaryfile.h"

using namespace std;

namespace Pastel
{

	namespace
	{

		bool loadHeader(
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

			const integer colors = 1 << pcxHeader.bitsPerPixel_;

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

				const integer toCopy = std::min(colors, 16);
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
						log() << "VGA Palette available" << logNewLine;
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

		void loadScanline(
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

	PASTELGFX bool loadIndexedPcx(
		const std::string& fileName,
		AbstractArray<2, uint8>& image,
		std::vector<Color>* colorPalette)
	{
		log() << "Loading an indexed pcx image from "
			<< fileName << "." << logNewLine;

		BinaryFile file;
		PcxHeader pcxHeader;
		std::vector<Color> palette;

		if (!loadHeader(fileName, file, pcxHeader, palette))
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

		const integer bytesPerScanline =
			(integer)pcxHeader.bytesPerScanlinePerPlane_ * (integer)pcxHeader.colorPlanes_;

		std::vector<uint8> scanline(bytesPerScanline, 0);

		const integer width = pcxHeader.width();
		const integer height = pcxHeader.height();

		image.setExtent(Vector<2, integer>(width, height));

		for (integer yPos = height - 1;yPos >= 0;--yPos)
		{
			loadScanline(file, bytesPerScanline, scanline);

			switch(pcxHeader.bitsPerPixel_)
			{
			case 1:
				{
					const integer bytesToRead = (pcxHeader.width() + 7) / 8;

					integer xPos = 0;
					for (integer i = 0;i < bytesToRead - 1;++i)
					{
						const uint8 data = scanline[i];

						image.write(IPoint2(xPos, yPos), (data >> 7) & 1);
						++xPos;
						image.write(IPoint2(xPos, yPos), (data >> 6) & 1);
						++xPos;
						image.write(IPoint2(xPos, yPos), (data >> 5) & 1);
						++xPos;
						image.write(IPoint2(xPos, yPos), (data >> 4) & 1);
						++xPos;
						image.write(IPoint2(xPos, yPos), (data >> 3) & 1);
						++xPos;
						image.write(IPoint2(xPos, yPos), (data >> 2) & 1);
						++xPos;
						image.write(IPoint2(xPos, yPos), (data >> 1) & 1);
						++xPos;
						image.write(IPoint2(xPos, yPos), (data >> 0) & 1);
						++xPos;
					}

					const uint8 data = scanline[bytesToRead - 1];
					if (xPos < width)
					{
						image.write(IPoint2(xPos, yPos), (data >> 7) & 1);
						++xPos;
					}
					if (xPos < width)
					{
						image.write(IPoint2(xPos, yPos), (data >> 6) & 1);
						++xPos;
					}
					if (xPos < width)
					{
						image.write(IPoint2(xPos, yPos), (data >> 5) & 1);
						++xPos;
					}
					if (xPos < width)
					{
						image.write(IPoint2(xPos, yPos), (data >> 4) & 1);
						++xPos;
					}
					if (xPos < width)
					{
						image.write(IPoint2(xPos, yPos), (data >> 3) & 1);
						++xPos;
					}
					if (xPos < width)
					{
						image.write(IPoint2(xPos, yPos), (data >> 2) & 1);
						++xPos;
					}
					if (xPos < width)
					{
						image.write(IPoint2(xPos, yPos), (data >> 1) & 1);
						++xPos;
					}
					if (xPos < width)
					{
						image.write(IPoint2(xPos, yPos), (data >> 0) & 1);
						++xPos;
					}
				}
				break;
			case 2:
				{
					const integer bytesToRead = (pcxHeader.width() + 3) / 4;

					integer xPos = 0;
					for (integer i = 0;i < bytesToRead - 1;++i)
					{
						const uint8 data = scanline[i];

						image.write(IPoint2(xPos, yPos), (data >> 6) & 3);
						++xPos;
						image.write(IPoint2(xPos, yPos), (data >> 4) & 3);
						++xPos;
						image.write(IPoint2(xPos, yPos), (data >> 2) & 3);
						++xPos;
						image.write(IPoint2(xPos, yPos), (data >> 0) & 3);
						++xPos;
					}

					const uint8 data = scanline[bytesToRead - 1];
					if (xPos < width)
					{
						image.write(IPoint2(xPos, yPos), (data >> 6) & 3);
						++xPos;
					}
					if (xPos < width)
					{
						image.write(IPoint2(xPos, yPos), (data >> 4) & 3);
						++xPos;
					}
					if (xPos < width)
					{
						image.write(IPoint2(xPos, yPos), (data >> 2) & 3);
						++xPos;
					}
					if (xPos < width)
					{
						image.write(IPoint2(xPos, yPos), (data >> 0) & 3);
						++xPos;
					}
				}
				break;
			case 4:
				{
					const integer bytesToRead = (pcxHeader.width() + 1) / 2;

					integer xPos = 0;
					for (integer i = 0;i < bytesToRead - 1;++i)
					{
						const uint8 data = scanline[i];

						image.write(IPoint2(xPos, yPos), (data >> 4) & 15);
						++xPos;
						image.write(IPoint2(xPos, yPos), (data >> 0) & 15);
						++xPos;
					}

					const uint8 data = scanline[bytesToRead - 1];
					if (xPos < width)
					{
						image.write(IPoint2(xPos, yPos), (data >> 4) & 15);
						++xPos;
					}
					if (xPos < width)
					{
						image.write(IPoint2(xPos, yPos), (data >> 0) & 15);
						++xPos;
					}
				}
				break;
			case 8:
				{
					const integer bytesToRead = pcxHeader.width();

					integer xPos = 0;
					for (integer i = 0;i < bytesToRead;++i)
					{
						const uint8 data = scanline[i];

						image.write(IPoint2(xPos, yPos), data);
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

		log() << "Indexed pcx image loading complete." << logNewLine;

		return true;
	}

	PASTELGFX bool loadPcx(
		const std::string& fileName,
		AbstractArray<2, Color>& image)
	{
		log() << "Loading a pcx image from "
			<< fileName << "." << logNewLine;

		BinaryFile file;
		PcxHeader pcxHeader;
		std::vector<Color> palette;

		if (!loadHeader(fileName, file, pcxHeader, palette))
		{
			return false;
		}

		const integer bytesPerScanline =
			(integer)pcxHeader.bytesPerScanlinePerPlane_ * (integer)pcxHeader.colorPlanes_;

		std::vector<uint8> scanline(bytesPerScanline, 0);

		const integer width = pcxHeader.width();
		const integer height = pcxHeader.height();

		image.setExtent(Vector<2, integer>(width, height));

		for (integer yPos = height - 1;yPos >= 0;--yPos)
		{
			loadScanline(file, bytesPerScanline, scanline);

			// Copy scanline to the image

			if (pcxHeader.colorPlanes_ == 1)
			{
				switch(pcxHeader.bitsPerPixel_)
				{
				case 1:
					{
						const integer bytesToRead = (width + 7) / 8;

						integer xPos = 0;
						for (integer i = 0;i < bytesToRead - 1;++i)
						{
							const uint8 data = scanline[i];

							image.write(IPoint2(xPos, yPos), palette[(data >> 7) & 1]);
							++xPos;
							image.write(IPoint2(xPos, yPos), palette[(data >> 6) & 1]);
							++xPos;
							image.write(IPoint2(xPos, yPos), palette[(data >> 5) & 1]);
							++xPos;
							image.write(IPoint2(xPos, yPos), palette[(data >> 4) & 1]);
							++xPos;
							image.write(IPoint2(xPos, yPos), palette[(data >> 3) & 1]);
							++xPos;
							image.write(IPoint2(xPos, yPos), palette[(data >> 2) & 1]);
							++xPos;
							image.write(IPoint2(xPos, yPos), palette[(data >> 1) & 1]);
							++xPos;
							image.write(IPoint2(xPos, yPos), palette[(data >> 0) & 1]);
							++xPos;
						}

						const uint8 data = scanline[bytesToRead - 1];
						if (xPos < width)
						{
							image.write(IPoint2(xPos, yPos), palette[(data >> 7) & 1]);
							++xPos;
						}
						if (xPos < width)
						{
							image.write(IPoint2(xPos, yPos), palette[(data >> 6) & 1]);
							++xPos;
						}
						if (xPos < width)
						{
							image.write(IPoint2(xPos, yPos), palette[(data >> 5) & 1]);
							++xPos;
						}
						if (xPos < width)
						{
							image.write(IPoint2(xPos, yPos), palette[(data >> 4) & 1]);
							++xPos;
						}
						if (xPos < width)
						{
							image.write(IPoint2(xPos, yPos), palette[(data >> 3) & 1]);
							++xPos;
						}
						if (xPos < width)
						{
							image.write(IPoint2(xPos, yPos), palette[(data >> 2) & 1]);
							++xPos;
						}
						if (xPos < width)
						{
							image.write(IPoint2(xPos, yPos), palette[(data >> 1) & 1]);
							++xPos;
						}
						if (xPos < width)
						{
							image.write(IPoint2(xPos, yPos), palette[(data >> 0) & 1]);
							++xPos;
						}
					}
					break;
				case 2:
					{
						const integer bytesToRead = (width + 3) / 4;

						integer xPos = 0;
						for (integer i = 0;i < bytesToRead - 1;++i)
						{
							const uint8 data = scanline[i];

							image.write(IPoint2(xPos, yPos), palette[(data >> 6) & 3]);
							++xPos;
							image.write(IPoint2(xPos, yPos), palette[(data >> 4) & 3]);
							++xPos;
							image.write(IPoint2(xPos, yPos), palette[(data >> 2) & 3]);
							++xPos;
							image.write(IPoint2(xPos, yPos), palette[(data >> 0) & 3]);
							++xPos;
						}

						const uint8 data = scanline[bytesToRead - 1];
						if (xPos < width)
						{
							image.write(IPoint2(xPos, yPos), palette[(data >> 6) & 3]);
							++xPos;
						}
						if (xPos < width)
						{
							image.write(IPoint2(xPos, yPos), palette[(data >> 4) & 3]);
							++xPos;
						}
						if (xPos < width)
						{
							image.write(IPoint2(xPos, yPos), palette[(data >> 2) & 3]);
							++xPos;
						}
						if (xPos < width)
						{
							image.write(IPoint2(xPos, yPos), palette[(data >> 0) & 3]);
							++xPos;
						}
					}
					break;
				case 4:
					{
						const integer bytesToRead = (width + 1) / 2;

						integer xPos = 0;
						for (integer i = 0;i < bytesToRead - 1;++i)
						{
							const uint8 data = scanline[i];

							image.write(IPoint2(xPos, yPos), palette[(data >> 4) & 15]);
							++xPos;
							image.write(IPoint2(xPos, yPos), palette[(data >> 0) & 15]);
							++xPos;
						}

						const uint8 data = scanline[bytesToRead - 1];
						if (xPos < width)
						{
							image.write(IPoint2(xPos, yPos), palette[(data >> 4) & 15]);
							++xPos;
						}
						if (xPos < width)
						{
							image.write(IPoint2(xPos, yPos), palette[(data >> 0) & 15]);
							++xPos;
						}
					}
					break;
				case 8:
					{
						const integer bytesToRead = width;

						integer xPos = 0;
						for (integer i = 0;i < bytesToRead;++i)
						{
							const uint8 data = scanline[i];

							image.write(IPoint2(xPos, yPos), palette[data]);
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
					image.write(IPoint2(xPos, yPos),
						Color(scanlineRed[xPos],
						scanlineGreen[xPos],
						scanlineBlue[xPos]) / 255);
				}
			}
		}

		file.close();

		log() << "Pcx image loading complete." << logNewLine;

		return true;
	}

	PASTELGFX bool loadIndexedPcx(
		const std::string& fileName,
		Array<2, bool>& image,
		std::vector<Color>* colorPalette)
	{
		Bool_Integer_Adapter<uint8> adapter;
		return loadIndexedPcx(fileName, image, adapter, colorPalette);
	}

	PASTELGFX bool loadIndexedPcx(
		const std::string& fileName,
		Array<2, uint8>& image,
		std::vector<Color>* colorPalette)
	{
		IdentityAdapter<uint8> adapter;
		return loadIndexedPcx(fileName, image, adapter, colorPalette);
	}

	PASTELGFX bool loadPcx(
		const std::string& fileName,
		Array<2, Color>& image)
	{
		IdentityAdapter<Color> adapter;
		return loadPcx(fileName, image, adapter);
	}

	PASTELGFX bool loadPcx(
		const std::string& fileName,
		Array<2, ByteColor>& image)
	{
		Color_ByteColor_Adapter adapter;
		return loadPcx(fileName, image, adapter);
	}

	PASTELGFX bool loadPcx(
		const std::string& fileName,
		Array<2, uint32>& image)
	{
		Color_Integer_Adapter<uint32, 8, 8, 8> adapter;
		return loadPcx(fileName, image, adapter);
	}

	PASTELGFX bool loadPcx(
		const std::string& fileName,
		Array<2, uint16>& image)
	{
		Color_Integer_Adapter<uint16, 5, 6, 5> adapter;
		return loadPcx(fileName, image, adapter);
	}

}
