#ifndef __GLRenderWidget_h__
#define __GLRenderWidget_h__

#include "RE330_global.h"
#include "RenderWidget.h"
#include "GLWidget.h"

namespace RE330 
{
	/** This class implements the abstract RenderWidget interface. It is
		an abstract class itself. You need to implement a subclass in your
		application.
	@remarks
		This class is also a subclass of QWidget. You will subclass this 
		class to re-implement the event handlers of QWidget, for example,
		for user interaction with the mouse.
	*/
	class RE330_EXPORT GLRenderWidget : public RenderWidget
	{
		friend class GLWidget;

	public:
		GLRenderWidget() : RenderWidget() { mGLWidget = new GLWidget(this); }

	protected:
		/** Do *not* overwrite these methods in your subclass! They are used
			to pass on events to the underlying OpenGL widget.
		*/
		void updateScene()					
		{ 
			mGLWidget->updateGL(); 
		}
		void resizeEvent(QResizeEvent *e)	
		{ 
			mGLWidget->resize(e->size()); 
			resizeRenderWidgetEvent(e->size()); 
		}

	private:
		GLWidget *mGLWidget;
	};

}

#endif

