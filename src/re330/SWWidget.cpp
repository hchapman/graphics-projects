#include "SWWidget.h"
#include "SWRenderWidget.h"

using namespace RE330;

SWWidget::SWWidget(SWRenderWidget *parent)
	: QLabel(parent)
{
	initSceneEventCalled = false;
	image = 0;
}

SWWidget::~SWWidget()
{
	if(image!=0)
	{
		delete image;
	}
}

void SWWidget::resizeEvent(QResizeEvent *e)
{
	if(image!=0)
	{
		delete image;
	}
	image = new QImage(e->size().width(),e->size().height(),QImage::Format_RGB32);
	image->fill(qRgb(0,0,0));
	setPixmap(QPixmap::fromImage(*image));
	
	SWRenderContext::getSingleton().setViewport(e->size().width(), e->size().height());
}

void SWWidget::showEvent(QShowEvent *e)
{
	// Make sure initSceneEvent is called only once
	if(!initSceneEventCalled)
	{
		(static_cast<SWRenderWidget *>(parent()))->initSceneEvent();
	}
	initSceneEventCalled = true;
}

void SWWidget::updateScene()
{
	SWRenderContext::getSingleton().setWidget(this);
	(static_cast<SWRenderWidget*>(this->parentWidget()))->renderSceneEvent();
}

QImage* SWWidget::getImage()
{
	return image;
}

