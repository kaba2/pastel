#ifndef PASTELGL_GLGFXRENDERER_H
#define PASTELGL_GLGFXRENDERER_H

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

		virtual integer uploadTexture(const Array<2, Color>& image);

		virtual void clear();

		virtual void drawPoint(
			const Point2& point);

		virtual void drawSegment(
			const Segment2& segment);

		virtual void drawTriangle(
			const Triangle2& triangle);

		virtual void drawTriangle(
			const Triangle2& triangle,
			const Tuple<3, Color>& colorTriangle);

		virtual void drawTriangle(
			const Triangle2& triangle,
			const Triangle2& texture);

	private:
		// Prohibited
		GlGfxRenderer(const GlGfxRenderer&);

		//Prohibited
		GlGfxRenderer& operator=(const GlGfxRenderer&);

		void updateModelView();
	};

}

#endif
