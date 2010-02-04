#ifndef RenderWidget0_H
#define RenderWidget0_H

#include <QWidget>
#include "GLRenderWidget.h"
#include "GLRenderWidget.h"
#include "SceneManager.h"
#include "Object.h"

using namespace RE330;

class RenderWidget0 : public GLRenderWidget
{

public:
    RenderWidget0();
    ~RenderWidget0();

	void startAnimation();
	void stopAnimation();

protected:
	// Event handlers. These are virtual methods of the base class.
	// They are called automatically to handle specific events.

	// Called when the render window is ready.
	void initSceneEvent();

	// Called when the scene in the render window needs to be re-rendered.
	void renderSceneEvent();

	// Called when the render widget is resized.
	void resizeRenderWidgetEvent(const QSize &s);

	// Virtual methods of QWidget class. This is a subset. Re-implement any
	// that you would like to handle.
	void timerEvent(QTimerEvent *t);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

private:
	SceneManager *sceneManager;
	Camera *camera;
	Object *object;
	int timerId;
};

#endif // BASICAPP_H
