#ifndef PASTELGFX_IMAGEGFXRENDERER_HPP
#define PASTELGFX_IMAGEGFXRENDERER_HPP

#include "pastel/gfx/imagegfxrenderer.h"
#include "pastel/gfx/drawing.h"
#include "pastel/gfx/textures.h"

#include "pastel/sys/indexextender_all.h"

#include "pastel/math/matrix_tools.h"
#include "pastel/math/affinetransformation_tools.h"

namespace Pastel
{

	template <typename Type>
	ImageGfxRenderer<Type>::ImageGfxRenderer(Array<2, Type>* image)
		: GfxRenderer<Type>()
		, screenTransformation_()
		, image_(image)
		, textureImage_()
		, mipMap_()
		, nearestTexture_()
		, bilinearTexture_()
		, ewaTexture_(MipMap<2, Type>(), ArrayExtender<2, Type>(clampExtender()))
		, texture_(0)
	{
	}

	template <typename Type>
	ImageGfxRenderer<Type>::~ImageGfxRenderer()
	{
	}

	template <typename Type>
	void ImageGfxRenderer<Type>::setImage(Array<2, Type>* image)
	{
		image_ = image;
	}

	template <typename Type>
	Array<2, Type>* ImageGfxRenderer<Type>::image() const
	{
		return image_;
	}

	template <typename Type>
	void ImageGfxRenderer<Type>::onSetViewTransformation()
	{
		updateModelView();
	}

	template <typename Type>
	void ImageGfxRenderer<Type>::onSetModelTransformation()
	{
		updateModelView();
	}

	template <typename Type>
	void ImageGfxRenderer<Type>::onSetTexture()
	{
		nearestTexture_.setImage(*textureImage_[Base::texture()]);
		bilinearTexture_.setImage(*textureImage_[Base::texture()]);
		mipTexture_.setMipMap(*mipMap_[Base::texture()]);
		ewaTexture_.setMipMap(*mipMap_[Base::texture()]);
	}

	template <typename Type>
	void ImageGfxRenderer<Type>::onSetResamplingMode()
	{
		switch(Base::resamplingMode())
		{
		case ResamplingMode::Nearest:
			texture_ = &nearestTexture_;
			break;
		case ResamplingMode::Bilinear:
			texture_ = &bilinearTexture_;
			break;
		case ResamplingMode::Trilinear:
			texture_ = &mipTexture_;
			break;
		case ResamplingMode::Anisotropic:
			texture_ = &ewaTexture_;
			break;
		};
	}

	template <typename Type>
	void ImageGfxRenderer<Type>::uploadMipMap(
		integer textureId, const MipMap<2, Type>& mipMap)
	{
		ENSURE2(textureId >= 0 && textureId < textureImage_.size(),
			textureId, textureImage_.size());

		mipMap_[textureId] = &mipMap;
	}

	// Drawing

	template <typename Type>
	integer ImageGfxRenderer<Type>::uploadTexture(const Array<2, Type>& image)
	{
		textureImage_.push_back(&image);
		mipMap_.push_back(0);

		return textureImage_.size() - 1;
	}

	template <typename Type>
	void ImageGfxRenderer<Type>::clear()
	{
		const Type& currentColor = Base::color();
		const integer width = image_->width();
		const integer height = image_->height();

		for (integer y = 0;y < height;++y)
		{
			for (integer x = 0;x < width;++x)
			{
				(*image_)(x, y) = currentColor;
			}
		}
	}

	template <typename Type>
	void ImageGfxRenderer<Type>::drawPoint(
		const Point2& point)
	{
		const Type& currentColor = Base::color();
		const Point2 screenPoint = transform(point);

		// Set the closest pixel.

		drawPixel(screenPoint,
			currentColor, arrayView(*image_));
	}

	template <typename Type>
	void ImageGfxRenderer<Type>::drawSegment(
		const Segment2& segment)
	{
		const Type& currentColor = Base::color();
		const Segment2 screenSegment = Segment2(
			transform(segment.start()),
			transform(segment.end()));

		Pastel::drawSegment(screenSegment, currentColor, arrayView(*image_));
	}

	template <typename Type>
	void ImageGfxRenderer<Type>::drawTriangle(
		const Triangle2& triangle)
	{
		const Type& currentColor = Base::color();
		const Triangle2 screenTriangle(
			transform(triangle[0]),
			transform(triangle[1]),
			transform(triangle[2]));

		Pastel::drawTriangle(
			screenTriangle,
			Triangle2(),
			constantColorTexture<Type>(currentColor),
			arrayView(*image_));
	}

	template <typename Type>
	void ImageGfxRenderer<Type>::drawTriangle(
		const Triangle2& triangle,
		const Tuple<3, Type>& colorTriangle)
	{
		const Triangle2 screenTriangle(
			transform(triangle[0]),
			transform(triangle[1]),
			transform(triangle[2]));

		Pastel::drawTriangle(
			screenTriangle,
			Triangle2(Point2(0, 0), Point2(1, 0), Point2(0, 1)),
			bilinearTriangleTexture<Type>(colorTriangle),
			arrayView(*image_));
	}

	template <typename Type>
	void ImageGfxRenderer<Type>::drawTriangle(
		const Triangle2& triangle,
		const Triangle2& textureTriangle)
	{
		if (Base::texture() >= textureImage_.size() ||
			textureImage_[Base::texture()]->empty())
		{
			drawTriangle(triangle);
		}

		const Triangle2 screenTriangle(
			transform(triangle[0]),
			transform(triangle[1]),
			transform(triangle[2]));

		Pastel::drawTriangle(
			screenTriangle,
			textureTriangle,
			*texture_,
			arrayView(*image_));
	}

	// Private

	template <typename Type>
	void ImageGfxRenderer<Type>::updateModelView()
	{
		screenTransformation_ = Base::modelTransformation() *
			inverse(Base::viewTransformation());
	}

	template <typename Type>
	Point2 ImageGfxRenderer<Type>::transform(const Point2& point) const
	{
		const Point2& viewPoint = point * screenTransformation_;
		const Vector2 uv = (viewPoint - Base::viewWindow().min()) /
			Base::viewWindow().extent();
		return Point2(uv * Vector2(image_->width(), image_->height()));
	}

}

#endif
