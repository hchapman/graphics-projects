#ifndef __SWRenderWidget_h__
#define __SWRenderWidget_h__

#include "RE330_global.h"
#include "RenderWidget.h"
#include "SWWidget.h"
#include <QtGui>


namespace RE330 {

	/** This class implements the abstract RenderWidget interface. It is
		an abstract class itself. You need to implement a subclass in your
		application.
	@remarks
		This class is also a subclass of QWidget. You will subclass this 
		class to re-implement the event handlers of QWidget, for example,
		for user interaction with the mouse.
	*/
	class RE330_EXPORT SWRenderWidget : public RenderWidget
	{
		friend class SWWidget;

	public:
		SWRenderWidget();

	protected:
		/** Do *not* overwrite these methods in your subclass!
		*/
		void updateScene();					
		void resizeEvent(QResizeEvent *e);	

	private:
		SWWidget *mswWidget;
	};

}

#endif
