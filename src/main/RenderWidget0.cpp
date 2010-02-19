#include "RenderWidget0.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "Camera.h"
#include "GLWidget.h"
#include "GLRenderContext.h"
#include "VertexData.h"
#include "Shapes.h"

#include <stdio.h>

#include <QtOpenGL>


RenderWidget0::RenderWidget0()
{
	RenderContext *rs = new GLRenderContext();
	sceneManager = 0;
    mars_theta = 0.2;
}

RenderWidget0::~RenderWidget0()
{
	if(sceneManager)
	{
		delete sceneManager;
	}
}

void RenderWidget0::initSceneEvent() 
{
    counter = 0;
	sceneManager = new SceneManager();

	// Camera
	camera = sceneManager->createCamera();

    const int sun_colors = 13;
    float sun_list[sun_colors][3] = {
        {255, 200, 0},
        {224, 150, 0},
        {255, 200, 0},
        {255, 200, 0},
        {255, 200, 0},
        {224, 150, 0},
        {255, 200, 0},
        {255, 200, 0},
        {230, 123, 2},
        {194, 42, 0},
        {128, 41, 17},
        {255, 200, 0},
        {224, 150, 0}
    };
    toDecimal(sun_colors, sun_list);
    const int earth_colors = 14;
    float earth_list[earth_colors][3] = {
        {41, 121, 153},
        {41, 121, 153},
        {41, 121, 153},
        {41, 121, 153},
        {41, 121, 153},
        {68, 158, 194},
        {41, 121, 153},
        {68, 158, 194},
        {41, 121, 153},
        {41, 121, 153},
        {41, 121, 153},
        {70, 127, 30},
        {17, 104, 0},
        {100, 77, 15}
    };
    toDecimal(earth_colors, earth_list);
    const int moon_colors = 12;
    float moon_list[moon_colors][3] = {
        {255, 254, 247},
        {176, 180, 214},
        {255, 254, 247},
        {176, 180, 214},
        {255, 254, 247},
        {176, 180, 214},
        {255, 254, 247},
        {255, 254, 247},
        {255, 254, 247},
        {255, 254, 247},
        {176, 180, 214},
        {114, 122, 179},
    };
    toDecimal(moon_colors, moon_list);
    const int mars_colors = 9;
    float mars_list[mars_colors][3] = {
        {249, 84, 16},
        {249, 84, 16},
        {249, 84, 16},
        {249, 84, 16},
        {159, 48, 2},
        {159, 48, 2},
        {112, 4, 19},
        {79, 6, 16},
        {37, 0, 4},
    };
    toDecimal(mars_colors, mars_list);
    
    const int jupiter_colors = 5;
    float jupiter_list[jupiter_colors][3] = {
        {106, 125, 142},
        {194, 152, 102},
        {104,74,52},
        {146,64,32},
        {194,152,102}
    };
    toDecimal(jupiter_colors, jupiter_list);
    
    sun = Shapes::createSphere(sceneManager, 2, 64, 128, sun_colors, sun_list, 1);
    earth = Shapes::createSphere(sceneManager, 0.5, 16, 64, earth_colors, earth_list, 1);
    moon = Shapes::createSphere(sceneManager, 0.125, 16, 64, moon_colors, moon_list, 1);
    satellite = Shapes::createBox(sceneManager, 0.1, 0.075, 0.075, moon_colors, moon_list, 1);
    mars = Shapes::createSphere(sceneManager, 0.3, 16, 64, mars_colors, mars_list, 1);
    // mars = Shapes::createCone(sceneManager, 0.25, 0.125, 30, mars_colors, mars_list, 1);
    jupiter = Shapes::createSphere(sceneManager, 1, 128, 128, jupiter_colors, jupiter_list, 1);
    
	// Trigger timer event every 5ms.
	timerId = startTimer(5);
}

void RenderWidget0::toDecimal(int num_colors, float color_list[][3]) {
    for (int i = 0; i < num_colors; i++) {
        color_list[i][0] = color_list[i][0] / 255;
        color_list[i][1] = color_list[i][1] / 255;
        color_list[i][2] = color_list[i][2] / 255;
    }
}

void RenderWidget0::renderSceneEvent()
{
	sceneManager->renderScene();
}

void RenderWidget0::resizeRenderWidgetEvent(const QSize &s)
{
}

void RenderWidget0::timerEvent(QTimerEvent *t)
{
    Matrix4 m = Matrix4::rotateY(0.001);
    Matrix4 m2 = Matrix4::rotateZ(0.002);
    Matrix4 m3 = Matrix4::rotateY(-0.01);
    Matrix4 earth_back = Matrix4::translate(-4, 0, 0);
    Matrix4 earth_final = Matrix4::translate(4, 0, 0);
    Matrix4 tr = Matrix4::translate(0, 0, 0.05);
    Matrix4 tr2 = Matrix4::translate(0, 0, -0.05);
    Vector3 mars_axis = Vector3(0.2, 1, 0.1);
    Vector3 mars_offset = Vector3(-0.1, 0, 0.2);
    mars_offset.normalize();
    mars_offset *= 5 * (1 + abs(sin(mars_theta))/2);
    Matrix4 jupiter_final = Matrix4::translate(8, 0, 0);

    earth_theta += 0.007;
    earth_axis_theta += 0.005;
    moon_theta += 0.01;
    mars_theta += 0.0035;
    jupiter_theta += 0.0015;
    jupiter_z_theta += 0.005;

    sun->setTransformation(sun->getTransformation() * m * m2);
    earth->setTransformation(Matrix4::rotateY(earth_theta) * earth_final * Matrix4::rotateA(Vector3(1, 1.73, 0), earth_axis_theta));
    moon->setTransformation(
        Matrix4::translate(cos(earth_theta) * 4, 0, -sin(earth_theta) * 4) * 
        Matrix4::rotateA(Vector3(0, 1, 1), moon_theta) *
        Matrix4::translate(0.75, 0, 0));
    satellite->setTransformation(
        Matrix4::translate(cos(earth_theta) * 4, 0, -sin(earth_theta) * 4) *
        Matrix4::rotateX(moon_theta) *
        Matrix4::translate(0, 0.75, 0)
    );
    mars->setTransformation(
        Matrix4::rotateA(mars_axis, mars_theta) * Matrix4::translate(mars_offset)
    );
    jupiter->setTransformation(
        Matrix4::rotateY(jupiter_theta) * jupiter_final * Matrix4::rotateX(jupiter_z_theta)
    );
	updateScene();
}

void RenderWidget0::mousePressEvent(QMouseEvent *e)
{
}

void RenderWidget0::mouseMoveEvent(QMouseEvent *e)
{
}

void RenderWidget0::mouseReleaseEvent(QMouseEvent *e)
{
}

void RenderWidget0::startAnimation()
{
	if(!timerId)
	{
		timerId = startTimer(5);
	}
}

void RenderWidget0::stopAnimation()
{
	if(timerId)
	{
		killTimer(timerId);
		timerId = 0;
	}
}
