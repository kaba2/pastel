// Description: Commmon structuring elements
// Detail: Especially for dilation and eroding
// Documentation: structuring_elements.txt

#ifndef PASTELGFX_STRUCTURING_ELEMENTS_H
#define PASTELGFX_STRUCTURING_ELEMENTS_H

#include "pastel/sys/view/view.h"

namespace Pastel
{

	//! Creates an ellipsoidal structuring element.
	/*!
	The structuring element is positioned at
	the center of the view.
	*/
	template <int N, typename Image_View>
	void ellipsoidElement(
		const View<N, bool, Image_View>& image,
		const NoDeduction<Vector<dreal, N>>& diameter);

	//! Creates an ellipsoidal structuring element.
	/*!
	This function simply calls
	ellipsoidElement(image, image.extent())
	*/
	template <int N, typename Image_View>
	void ellipsoidElement(
		const View<N, bool, Image_View>& image);

	//! Creates an ellipsoid-like structuring element.
	/*!
	Preconditions:
	power >= 1

	The structuring element is positioned at
	the center of the view.

	If power == 1, this is equivalent to
	calling diamondElement(), but slower.
	If power == 2, this is equivalent to
	calling ellipsoidElement(), but slower.
	For power e ]1, 2[, the surface
	interpolates between the two surfaces.
	As power -> oo, the shape approaches
	the boxElement().

	To be more precise, the resulting structuring elements
	are those that are bounded by the level sets
	of p-norms.

	For p e [1, 2[, the level set surface contains corners.
	For p >= 2, the level set surface is smooth.
	*/
	template <int N, typename Image_View>
	void pEllipsoidElement(
		const View<N, bool, Image_View>& image,
		dreal power,
		const NoDeduction<Vector<dreal, N>>& diameter);

	//! Creates an ellipsoid-like structuring element.
	/*!
	This function simply calls
	pEllipsoidElement(image, power, image.extent())
	*/
	template <int N, typename Image_View>
	void pEllipsoidElement(
		const View<N, bool, Image_View>& image,
		dreal power);

	//! Creates a diamond-shaped structuring element.
	/*!
	The structuring element is positioned at
	the center of the view.
	*/
	template <int N, typename Image_View>
	void diamondElement(
		const View<N, bool, Image_View>& image,
		const NoDeduction<Vector<dreal, N>>& diameter);

	//! Creates a diamond-shaped structuring element.
	/*!
	This function simply calls
	diamondElement(image, image.extent())
	*/
	template <int N, typename Image_View>
	void diamondElement(
		const View<N, bool, Image_View>& image);

	//! Creates a box-shaped structuring element.
	/*!
	The structuring element is positioned at
	the center of the view.
	*/
	template <int N, typename Image_View>
	void boxElement(
		const View<N, bool, Image_View>& image,
		const NoDeduction<Vector<dreal, N>>& diameter);

	//! Creates a box-shaped structuring element.
	/*!
	This function simply calls
	boxElement(image, image.extent())
	*/
	template <int N, typename Image_View>
	void boxElement(
		const View<N, bool, Image_View>& image);

}

#include "pastel/gfx/image_processing/structuring_elements.hpp"

#endif
