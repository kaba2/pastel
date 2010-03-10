// Description: A simple graphical user interface

#ifndef PASTEL_GFX_UI_H
#define PASTEL_GFX_UI_H

#include "pastel/gfxui/gfxuilibrary.h"

#include "pastel/gfx/gfxrenderer.h"
#include "pastel/gfx/color.h"

#include "pastel/device/ui.h"

namespace Pastel
{

	class PASTELGFXUI Gfx_Ui
		: public Ui
	{
	public:
		explicit Gfx_Ui(
			GfxRenderer<Color>* renderer,
			integer logicToRenderRatio = 1);

		void render();

		GfxRenderer<Color>& renderer();
		const GfxRenderer<Color>& renderer() const;

		Vector2 screenPointToWorld(const Vector2& screenPoint) const;
		Vector2 screenVectorToWorld(const Vector2& screenVector) const;

	private:
		void handleKeys();
		
		virtual void onLogic(real deltaTime);

		virtual void onRender();			
		virtual void onGfxLogic();

		GfxRenderer<Color>* renderer_;
		integer renderTime_;
		integer logicToRenderRatio_;
	};

}

#endif
