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
	
	// Creates the sphere's vertices
    float* makeSphereVertices(int slices, int points);
    int* makeSphereIndices(int slices, int points);
    float* makeSphereColors(int slices, int points);
    float* makePyramidVertices(int height, int base_points);
    int* makePyramidIndices(int height, int base_points);
    float* makePyramidColors(int height, int base_points);
    void toDecimal(int num_colors, float color_list[][3]);
private:
	SceneManager *sceneManager;
	Camera *camera;
	Object *cube;
    Object *object;
	int timerId;
    int counter;

    // For the virtual trackball
    bool tracking;
    QPoint track_start;
    
    // Use a composite sphere/hyperbolic sheet for the trackball
    // as suggested by http://www.opengl.org/wiki/Trackball
    static const bool USE_COMPOSITE = true;
};

#endif // RenderWidget0_H
