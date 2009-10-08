// Description: GlGfxRenderer class
// Detail: A 2d per-object renderer using OpenGl
// Documentation: rendering.txt

#ifndef PASTEL_GLGFXRENDERER_H
#define PASTEL_GLGFXRENDERER_H

#include "pastel/gl/gllibrary.h"

#include "pastel/gfx/gfxrenderer.h"

#include "pastel/sys/rectangle.h"

namespace Pastel
{

	class PASTELGL GlGfxRenderer
		: public GfxRenderer<Color>
	{
	public:
		GlGfxRenderer();
		virtual ~GlGfxRenderer();

		// Specialized functions

		void setGlViewport(
			const Rectangle2& viewport);

		// Transformation state

		virtual void onSetViewTransformation();
		virtual void onSetModelTransformation();
		virtual void onSetViewWindow();
		virtual void onSetResamplingMode();
		virtual void onSetTexture();

		// Drawing

		virtual integer uploadTexture(const Array<Color, 2>& image);

		virtual void clear() const;

		virtual void drawPoint(
			const Vector2& point) const;

		virtual void drawSegment(
			const Segment2& segment) const;

		virtual void drawTriangle(
			const Triangle2& triangle) const;

		virtual void drawTriangle(
			const Triangle2& triangle,
			const Tuple<Color, 3>& colorTriangle) const;

		virtual void drawTriangle(
			const Triangle2& triangle,
			const Triangle2& texture) const;

	private:
		// Prohibited
		GlGfxRenderer(const GlGfxRenderer&);

		//Prohibited
		GlGfxRenderer& operator=(const GlGfxRenderer&);

		void updateModelView();
	};

}

#endif
