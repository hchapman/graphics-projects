#ifndef __RenderWidget_h__
#define __RenderWidget_h__

#include <QWidget>

namespace RE330
{
	/** This is an abstract class providing a widget for displaying
		rendering output. Applications should not directly inherit
		this class. Instead, applications should inherit, for example,
		the OpenGL render widget GLRenderWidget.
	*/
	class RE330_EXPORT RenderWidget : public QWidget
	{
	protected:
		/** To be re-implemented by RE330 subclass. */
		virtual void updateScene() = 0;	

		/** To be re-implemented by client subclass. */
		virtual void initSceneEvent() = 0;	
		virtual void renderSceneEvent() = 0;
		virtual void resizeRenderWidgetEvent(const QSize &s) = 0;
	};
	
}

#endif
