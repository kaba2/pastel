#ifndef PASTELSYS_PCXHEADER_HPP
#define PASTELSYS_PCXHEADER_HPP

#include "pastel/gfx/image_file/pcx/pcxheader.h"
#include "pastel/sys/endian.h"
#include "pastel/sys/logging/log.h"

namespace Pastel
{

	inline PcxHeader::PcxHeader()
		: manufacturer_(0)
		, version_(0)
		, encoding_(0)
		, bitsPerPixel_(0)
		, leftMargin_(0)
		, upperMargin_(0)
		, rightMargin_(0)
		, lowerMargin_(0)
		, horizontalDpi_(0)
		, verticalDpi_(0)
		, palette16_()
		, reserved1_(0)
		, colorPlanes_(0)
		, bytesPerScanlinePerPlane_(0)
		, paletteInfo_(0)
		, hss_(0)
		, vss_(0)
		, reserved2_()
	{
		for (integer i = 0;i < Palette16Size;++i)
		{
			palette16_[i] = 0;
		}
		for (integer i = 0;i < Reserved2Size;++i)
		{
			reserved2_[i] = 0;
		}
	}

	inline void PcxHeader::read(BinaryFile& input)
	{
		input << binaryLittleEndian;

		input
			>> manufacturer_
			>> version_
			>> encoding_
			>> bitsPerPixel_
			>> leftMargin_
			>> upperMargin_
			>> rightMargin_
			>> lowerMargin_
			>> horizontalDpi_
			>> verticalDpi_;

		input.read((int8*)palette16_, Palette16Size);

		input
			>> reserved1_
			>> colorPlanes_
			>> bytesPerScanlinePerPlane_
			>> paletteInfo_
			>> hss_
			>> vss_;

		input.read((int8*)reserved2_, Reserved2Size);
	}

	inline void PcxHeader::write(BinaryFile& output)
	{
		output << binaryLittleEndian
			<< manufacturer_
			<< version_
			<< encoding_
			<< bitsPerPixel_
			<< leftMargin_
			<< upperMargin_
			<< rightMargin_
			<< lowerMargin_
			<< horizontalDpi_
			<< verticalDpi_;

		output.write((int8*)palette16_, Palette16Size);

		output
			<< reserved1_
			<< colorPlanes_
			<< bytesPerScanlinePerPlane_
			<< paletteInfo_
			<< hss_
			<< vss_;

		output.write((int8*)reserved2_, Reserved2Size);
	}

	inline integer PcxHeader::width() const
	{
		return (integer)((rightMargin_ - leftMargin_) + 1);
	}

	inline integer PcxHeader::height() const
	{
		return (integer)((lowerMargin_ - upperMargin_) + 1);
	}

}

#endif
