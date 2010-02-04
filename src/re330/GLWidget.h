#ifndef __GLWidget_h__
#define __GLWidget_h__

#include "RE330_global.h"
#include "GLRenderContext.h"
#include <QtOpenGL>

namespace RE330 
{
	class GLRenderWidget;

	/** Internal class. No need to use it directly in applications.
	*/
	class RE330_EXPORT GLWidget : public QGLWidget
	{
		Q_OBJECT

	public:
		GLWidget(GLRenderWidget *parent);

	protected:
		void resizeEvent(QResizeEvent *e);
		void initializeGL();
		void paintGL();
	};

}

#endif

