#ifndef RenderWidget0_H
#define RenderWidget0_H

#include <QWidget>
#include "GLRenderWidget.h"
#include "SWRenderWidget.h"
#include "SceneManager.h"
#include "Object.h"
using namespace RE330;

#include <map>
using namespace std;

class RenderWidget0 : public SWRenderWidget
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

    void toDecimal(int num_colors, float color_list[][3]);
    void setupCamera();
    void setupObjects();

private:
    SceneManager *sceneManager;
    Camera *camera;

    // allows more managed storage of objects
    map <string, Object(*)> objects;

    // allows easy updating of all objects in scene
    map <string, Object(*)>::iterator it;

    Object *object;
    int timerId;

    // For the virtual trackball
    bool tracking;
    QPoint track_start;

    // Use a composite sphere/hyperbolic sheet for the trackball
    // as suggested by http://www.opengl.org/wiki/Trackball
    static const bool USE_COMPOSITE = true;

    bool sceneCreated;

    string HOUSE;
};

#endif // RenderWidget0_H
