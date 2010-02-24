#include "RenderWidget0.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "Camera.h"
#include "GLWidget.h"
#include "GLRenderContext.h"
#include "VertexData.h"
#include "Shapes.h"

#include <stdio.h>
#include <iostream>

#include <QtOpenGL>


RenderWidget0::RenderWidget0()
{
	RenderContext *rs = new GLRenderContext();
	sceneManager = 0;
    tracking = false;
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

    const int CAMERA_POSITION = 1;

	// Camera
	camera = sceneManager->createCamera();
    
    std::cout << *camera << std::endl;

    if (CAMERA_POSITION == 1) {
        // First camera test setting
        camera->createViewMatrix(
            Vector4(0,0,40,1),
            Vector4(0,0,0,1),
            Vector4(0,1,0,0));
    }
    else if (CAMERA_POSITION == 2) {
        // Second camera test setting
        camera->createViewMatrix(
            Vector4(-10,40,40,1),                     
            Vector4(-5,0,0,1),
            Vector4(0,1,0,0));
    }

    camera->createProjectionMatrix(
        1, 100, 1, 60.0/180.0*M_PI);

    std::cout << *camera << std::endl;

	// A house
	int nVerts = 42;
	float vertices[] = {-4,-4,4, 4,-4,4, 4,4,4, -4,4,4,		 // front face
		                -4,-4,-4, -4,-4,4, -4,4,4, -4,4,-4,  // left face
						4,-4,-4,-4,-4,-4, -4,4,-4, 4,4,-4,   // back face
						4,-4,4, 4,-4,-4, 4,4,-4, 4,4,4,		 // right face
						4,4,4, 4,4,-4, -4,4,-4, -4,4,4,		 // top face
						-4,-4,4, -4,-4,-4, 4,-4,-4, 4,-4,4,  // bottom face

						-20,-4,20, 20,-4,20, 20,-4,-20, -20,-4,-20,  // ground floor
						-4,4,4, 4,4,4, 0,8,4,						 // roof
						4,4,4, 4,4,-4, 0,8,-4, 0,8,4,
						-4,4,4, 0,8,4, 0,8,-4, -4,4,-4,
						4,4,-4, -4,4,-4, 0,8,-4};

	float colors[] = {1,0,0, 1,0,0, 1,0,0, 1,0,0,	// front face
					  0,1,0, 0,1,0, 0,1,0, 0,1,0,	// left face
					  1,0,0, 1,0,0, 1,0,0, 1,0,0,	// back face
					  0,1,0, 0,1,0, 0,1,0, 0,1,0,	// right face
					  0,0,1, 0,0,1, 0,0,1, 0,0,1,	// top face
					  0,0,1, 0,0,1, 0,0,1, 0,0,1,	// bottom face
	
					  0,0.5,0, 0,0.5,0, 0,0.5,0, 0,0.5,0,	// ground floor
					  0,0,1, 0,0,1, 0,0,1,					// roof
					  1,0,0, 1,0,0, 1,0,0, 1,0,0,
					  0,1,0, 0,1,0, 0,1,0, 0,1,0,
					  0,0,1, 0,0,1, 0,0,1,};

	// Add an object to the scene
	object = sceneManager->createObject();

	// Set up the vertex data
	VertexData& vertexData = object->vertexData;

	// Specify the elements of the vertex data:
	// - one element for vertex positions
	vertexData.vertexDeclaration.addElement(0, 0, 3, 3*sizeof(float), RE330::VES_POSITION);
	// - one element for vertex colors
	vertexData.vertexDeclaration.addElement(1, 0, 3, 3*sizeof(int), RE330::VES_DIFFUSE);
	
	// Create the buffers and load the data
	vertexData.createVertexBuffer(0, nVerts*3*sizeof(float), (unsigned char*)vertices);
	vertexData.createVertexBuffer(1, nVerts*3*sizeof(float), (unsigned char*)colors);

	// The index data that stores the connectivity of the triangles
	int indices[] = {0,2,3, 0,1,2,			// front face
		             4,6,7, 4,5,6,			// left face
					 8,10,11, 8,9,10,		// back face
					 12,14,15, 12,13,14,	// right face
					 16,18,19, 16,17,18,	// top face
					 20,22,23, 20,21,22,	// bottom face
	                 
					 24,26,27, 24,25,26,	// ground floor
					 28,29,30,				// roof
					 31,33,34, 31,32,33,
					 35,37,38, 35,36,37,
					 39,40,41};	

	vertexData.createIndexBuffer(60, indices);
    
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
    // If we have a camera and are resizing the widget, be sure to
    // update the aspect ratio!
    if (camera)
        camera->setAspectRatio((float)s.width()/(float)s.height());
}

void RenderWidget0::timerEvent(QTimerEvent *t)
{
	updateScene();
}

void RenderWidget0::mousePressEvent(QMouseEvent *e)
{
    // If we're pressing the left button, then we should start tracking
    if (e->buttons() & Qt::LeftButton) {
        tracking = true;

        // Store the initial mouse position
        track_start = e->pos();
    }

}

void RenderWidget0::mouseMoveEvent(QMouseEvent *e)
{
    if (tracking && e->buttons() & Qt::LeftButton && e->pos() != track_start) {
        Vector3 start, stop; // Temporary vectors representing
                             // points on the virtual trackball
        float _x, _y, _z; // Temporary 3-points
        const float diameter = (float)(width() < height() ? width() : height());

        // x coord of the start point on the virtual ball
        _x = ((float)track_start.x()*2.f - (float)width())/diameter;
        // y coord of the start point on the virtual ball
        _y = -(((float)track_start.y()*2.f - (float)height())/diameter);

        if (RenderWidget0::USE_COMPOSITE) {
            // Use a composite sphere/hyperbolic sheet for the trackball
            // as suggested by http://www.opengl.org/wiki/Trackball
            if (_x*_x + _y*_y <= 1.f/2.f) {
                _z = sqrt(1 - _x*_x - _y*_y);
            } else {
                _z = 1.f/(2*sqrt(_x*_x + _y*_y));
            }
        } else {
            // Make sure we're not clicking outside of the sphere
            // (There should be a better way to handle this)
            if (_x*_x + _y*_y > 1) {
                _z = 0;
            } else {
                // z coord of the start point on the virtual ball
                _z = sqrt(1 - _x*_x - _y*_y);
            }
        }

        // Create the starting point vector
        start = Vector3(_x, _y, _z).normalize();

        // x coord of the stop point on the virtual ball
        _x = ((float)e->pos().x()*2.f - (float)width())/diameter;
        // y coord of the stop point on the virtual ball
        _y = -(((float)e->pos().y()*2.f - (float)height())/diameter);

        if (RenderWidget0::USE_COMPOSITE) {
            // Use a composite sphere/hyperbolic sheet for the trackball
            // as suggested by http://www.opengl.org/wiki/Trackball
            if (_x*_x + _y*_y <= 1.f/2.f) {
                _z = sqrt(1 - _x*_x - _y*_y);
            } else {
                _z = 1.f/(2*sqrt(_x*_x + _y*_y));
            }
        } else {
            // Make sure we're not clicking outside of the sphere
            // (There should be a better way to handle this)
            if (_x*_x + _y*_y > 1) {
                _z = 0;
            } else {
                // z coord of the stop point on the virtual ball
                _z = sqrt(1 - _x *_x - _y*_y);  
            }
        }

        // Create the stopping point vector
        stop = Vector3(_x, _y, _z).normalize();

        // Make sure that the vectors aren't equal (if they are,
        // the cross product doesn't exist!)
        // Also, make sure that the cross product isn't 1, as acos(1) = nan
        if (start != stop && 
            !(1 - (start^stop)) < EPSILON & (1 - (start^stop)) > -EPSILON) {
            // Prep the trackball rotation matrix
            Matrix4 trackRotation = Matrix4::rotateA(start*stop, 
                                                     acos(start^stop));

            std::cout << start << "x" << stop << "=" << start*stop << std::endl;
            std::cout << " " << start << "^" << stop << "=" << 
                (start^stop) << std::endl;
            std::cout << trackRotation << std::endl;

            // Here's where the object rotation code should go. Right now,
            // we're hard coded in to use the 'object' object, but maybe
            // we should expand this to some sort of "scene" collection?
            object->setTransformation(trackRotation * 
                                      object->getTransformation());

            // Update the (now old) mouse position
            track_start = e->pos();
        }
    } else if (tracking && !(e->buttons() & Qt::LeftButton))
        // If we're tracking but somehow not clicking, then we shouldn't
        // actually still be tracking
        tracking = false;
}

void RenderWidget0::mouseReleaseEvent(QMouseEvent *e)
{
    // If we've released the left mouse button, then we shouldn't be tracking
    if (!(e->buttons() & Qt::LeftButton))
        tracking = false;
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
