#include "GLWidget.h"
#include "GLRenderWidget.h"

using namespace RE330;

GLWidget::GLWidget(GLRenderWidget *parent)
	: QGLWidget(parent)
{
}

void GLWidget::initializeGL()
{	
	GLRenderContext::getSingleton().init();

	(static_cast<GLRenderWidget *>(parent()))->initSceneEvent();

	QSize s = (static_cast<GLRenderWidget *>(parent()))->size();
	GLRenderContext::getSingleton().setViewport(s.width(), s.height());
}

void GLWidget::paintGL()
{ 
	(static_cast<GLRenderWidget *>(parent()))->renderSceneEvent(); 
}

void GLWidget::resizeEvent(QResizeEvent *e)
{
	GLRenderContext::getSingleton().setViewport(e->size().width(), e->size().height());
}

