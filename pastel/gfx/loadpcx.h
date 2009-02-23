/*!
\file
\brief Functions for loading pcx images.
*/

#ifndef PASTELGFX_LOADPCX_H
#define PASTELGFX_LOADPCX_H

#include "pastel/gfx/gfxlibrary.h"
#include "pastel/gfx/color.h"

#include "pastel/sys/array.h"
#include "pastel/sys/abstractarray.h"

#include <string>
#include <vector>

namespace Pastel
{

	//! Loads an indexed pcx into an index array.

	/*!
	returns:
	Whether succesful or not.

	fileName:
	The name of the pcx file to load.

	image:
	An array to store the image to.

	palette:
	An array to store the palette to.

	Exception safety:
	strong

	Indexed pcx images come in 4 different types:
	1, 2, 4 and 8 bits per pixel.
	This function reads all of them, using
	one byte per pixel also for 1, 2 and 4 bit depths.

	Binary images do not have
	a palette, but one will be returned for
	consistency, where zero is set to black
	and one is set to white.
	The palette for bit depths 2 and 4 is retrieved
	from the pcx header ("ega palette").
	For bit depth 8, the palette
	is retrieved from the optional vga palette,
	if present. If it is not present,
	the palette is set to the default palette
	as defined by the vga standard.

	If the loading is not succesful, it could be because:
	* The file does not exist
	* It can't be opened for reading for some reason
	* It is of unknown format
	* It is of wrong format (more color planes)
	*/

	PASTELGFX bool loadIndexedPcx(
		const std::string& fileName,
		AbstractArray<2, uint8>& image,
		std::vector<Color>* colorPalette = 0);

	template <typename Type, typename Adapter>
	bool loadIndexedPcx(
		const std::string& fileName,
		Array<2, Type>& image,
		const Adapter& adapter,
		std::vector<Color>* colorPalette = 0);

	//! Loads an indexed pcx image into a bool array.
	/*!
	This is a convenience function that
	uses Bool_Integer_Adapter<uint8>
	as the adapter.
	*/

	PASTELGFX bool loadIndexedPcx(
		const std::string& fileName,
		Array<2, bool>& image,
		std::vector<Color>* colorPalette = 0);

	//! Loads an indexed pcx image into an uint8 array.
	/*!
	This is a convenience function that
	uses Identity_Adapter<uint8>
	as the adapter.
	*/

	PASTELGFX bool loadIndexedPcx(
		const std::string& fileName,
		Array<2, uint8>& image,
		std::vector<Color>* colorPalette = 0);

	//! Loads any pcx to an image array.
	/*!
	returns:
	Whether succesful or not.

	fileName:
	The name of the pcx file to load.

	image:
	An array to store the image to.

	Exception safety:
	strong

	This function can read both indexed images
	and rgb images, that is, all the common types of pcx's.
	The result is given as a color image.

	If the loading is not successful, it could be because:
	* The file does not exist
	* It can't be opened for reading for some reason
	* It is of unknown format
	*/

	PASTELGFX bool loadPcx(
		const std::string& fileName,
		AbstractArray<2, Color>& image);

	template <typename Type, typename Adapter>
	bool loadPcx(
		const std::string& fileName,
		Array<2, Type>& image,
		const Adapter& adapter);

	//! Loads a pcx image into a Color array.
	/*!
	This is a convenience function that
	uses Color_ColorByte_Adapter
	as the adapter.
	*/

	PASTELGFX bool loadPcx(
		const std::string& fileName,
		Array<2, Color>& image);

	//! Loads a pcx image into a ByteColor array.
	/*!
	This is a convenience function that
	uses Identity_Adapter<ByteColor>
	as the adapter.
	*/

	PASTELGFX bool loadPcx(
		const std::string& fileName,
		Array<2, ByteColor>& image);

	//! Loads a pcx image into an rgb888 array.
	/*!
	This is a convenience function that
	uses ColorByte_Integer_Adapter<uint32, 8, 8, 8>
	as the adapter.
	*/

	PASTELGFX bool loadPcx(
		const std::string& fileName,
		Array<2, uint32>& image);

	//! Loads a pcx image into an rgb565 array.
	/*!
	This is a convenience function that
	uses ColorByte_Integer_Adapter<uint16, 5, 6, 5>
	as the adapter.
	*/

	PASTELGFX bool loadPcx(
		const std::string& fileName,
		Array<2, uint16>& image);

}

#include "pastel/gfx/loadpcx.hpp"

#endif
