#ifndef PASTELGFX_PCXHEADER_H
#define PASTELGFX_PCXHEADER_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/binaryfile.h"

namespace Pastel
{

	class PcxHeader
	{
	public:
		// Using default copy constructor
		// Using default destructor
		// Using default assignment

		PcxHeader();

		void read(BinaryFile& input);
		void write(BinaryFile& output);

		integer width() const;
		integer height() const;

		PASTEL_CONSTEXPR int Palette16Size = 48;
		PASTEL_CONSTEXPR int Reserved2Size = 54;

		uint8 manufacturer_;
		uint8 version_;
		uint8 encoding_;
		uint8 bitsPerPixel_;
		int16 leftMargin_;
		int16 upperMargin_;
		int16 rightMargin_;
		int16 lowerMargin_;
		uint16 horizontalDpi_;
		uint16 verticalDpi_;
		uint8 palette16_[Palette16Size];
		uint8 reserved1_;
		uint8 colorPlanes_;
		uint16 bytesPerScanlinePerPlane_;
		uint16 paletteInfo_;
		uint16 hss_;
		uint16 vss_;
		uint8 reserved2_[Reserved2Size];

	private:
		void endianFix();
	};

}

#endif
