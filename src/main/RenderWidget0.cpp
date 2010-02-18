#include "RenderWidget0.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "Camera.h"
#include "GLWidget.h"
#include "GLRenderContext.h"
#include "VertexData.h"

#include <stdio.h>

#include <QtOpenGL>


RenderWidget0::RenderWidget0()
{
	RenderContext *rs = new GLRenderContext();
	sceneManager = 0;
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

    const int num_colors = 5;
    float color_list[num_colors][3] = {
        {((float) 0x11) / 0xFF, ((float) 0x22) / 0xFF, ((float) 0x33) / 0xFF},
        {((float) 0xD) / 0xF, ((float) 0xD) / 0xF, ((float) 0xD) / 0xF},
        {1, 0, 1},
        {0, 1, 1},
        {1, 1, 0}
    };
    object = sceneManager->createSphere(2, 8, 16, num_colors, color_list);
    object->setTransformation(object->getTransformation() * Matrix4::translate(3, 0, 0));
    cone = sceneManager->createCone(2, 10, num_colors, color_list);
    cone->setTransformation(cone->getTransformation() * Matrix4::translate(-3, 0, 0));
    // object = sceneManager->createBox(2, 2, 2, num_colors, color_list);


	// Trigger timer event every 5ms.
	timerId = startTimer(5);
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
    Matrix4 m = Matrix4::rotateY(0.01);
    Matrix4 m2 = Matrix4::rotateX(0.01);
    Matrix4 m3 = Matrix4::rotateY(-0.01);
    Matrix4 m4 = Matrix4::rotateX(0.01);
    counter++;
    counter = counter % 2000;
    Matrix4 s = Matrix4::scale(1.002, 1.002, 1.002);
    if (counter > 1000) {
        s = Matrix4::scale(1/1.002, 1/1.002, 1/1.002);        
    }
    Matrix4 ma = Matrix4::rotateA(Vector3(3, 2, 1), 0.01);
    // Matrix4 m(cos(0.01),-sin(0.01),0,0, sin(0.01),cos(0.01),0,0, 0,0,1,0, 0,0,0,1);
    // Matrix4 m2(1,0,0,0, 0,cos(0.01), -sin(0.01),0, 0,sin(0.01),cos(0.01),0, 0,0,0,1);
	object->setTransformation(object->getTransformation()*m*m2);
    cone->setTransformation(cone->getTransformation()*m3*m4);
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
