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
	sceneManager = new SceneManager();

	// Camera
	camera = sceneManager->createCamera();

	// A cube
    const int slices = 30;
    const int points = 30;
	const int nVerts = 2 + (slices - 1) * points;
    printf("No arrays yet.\n");
    fflush(NULL);
    float* sphere_vertices = makeSphereVertices(slices, points);
    float* sphere_colors = makeSphereColors(slices, points);
    int* sphere_indices = makeSphereIndices(slices, points);
    
	float vertices[] = {-1,-1,1, 1,-1,1, 1,1,1, -1,1,1,		// front face
		                -1,-1,-1, -1,-1,1, -1,1,1, -1,1,-1, // left face
						1,-1,-1,-1,-1,-1, -1,1,-1, 1,1,-1,  // back face
						1,-1,1, 1,-1,-1, 1,1,-1, 1,1,1,		// right face
						1,1,1, 1,1,-1, -1,1,-1, -1,1,1,		// top face
						-1,-1,1, -1,-1,-1, 1,-1,-1, 1,-1,1};// bottom face

	float colors[] = {1,0,0, 1,0,0, 1,0,0, 1,0,0,
					  0,1,0, 0,1,0, 0,1,0, 0,1,0,
					  1,0,0, 1,0,0, 1,0,0, 1,0,0,
					  0,1,0, 0,1,0, 0,1,0, 0,1,0,
					  0,0,1, 0,0,1, 0,0,1, 0,0,1,
					  0,0,1, 0,0,1, 0,0,1, 0,0,1};

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
	vertexData.createVertexBuffer(0, nVerts*3*sizeof(float), (unsigned char*)sphere_vertices);
	vertexData.createVertexBuffer(1, nVerts*3*sizeof(float), (unsigned char*)sphere_colors);
    printf("All arrays created, vertexData created.\n");
    fflush(NULL);
	// The index data that stores the connectivity of the triangles
	int indices[] = {0,2,3, 0,1,2,			// front face
		             4,6,7, 4,5,6,			// left face
					 8,10,11, 8,9,10,		// back face
					 12,14,15, 12,13,14,	// right face
					 16,18,19, 16,17,18,	// top face
					 20,22,23, 20,21,22};	// bottom face
	vertexData.createIndexBuffer(slices*points*2*3, sphere_indices);

    object->setTransformation(object->getTransformation()*Matrix4::scale(Vector3(3,3,3)));

	// Trigger timer event every 5ms.
	timerId = startTimer(5);
}

float* RenderWidget0::makeSphereVertices(int slices, int points) {
    float* array = new float[(2 + (slices - 1) * points) * 3];
    int index = 0;
    float phi, theta;

    // The very top of the sphere
    array[index++] = 0;
    array[index++] = 0;
    array[index++] = 1;

    // The sphere has a radius of 1
    for(int s = 1; s < slices; s++) {
        for(int p = 0; p < points; p++) {
            phi = (M_PI/slices) * s;
            theta = (M_PI/points) * p * 2;
            array[index++] = cos(theta) * sin(phi);               // X
            array[index++] = sin(theta) * sin(phi);               // Y
            array[index++] = cos(phi);                            // Z
            printf("(%f, %f, %f) ", array[index - 3], array[index - 2], array[index - 1]);
        }
        printf("\n");
    }
    fflush(NULL);
    
    // The very bottom of the sphere
    array[index++] = 0;
    array[index++] = 0;
    array[index++] = -1;
    printf("index of the last point %d", (index-3)/3);
        
    return array;
}

float* RenderWidget0::makeSphereColors(int slices, int points) {
    float* array = new float[slices * points * 3 * 3];
    float color;
    int index = 0;
    for(int i = 0; i < slices; i++) {
        for(int j = 0; j < points; j++) {
            array[index++] = 1;
            array[index++] = 0;
            array[index++] = 0;

            array[index++] = 0;
            array[index++] = 1;
            array[index++] = 0;

            array[index++] = 0;
            array[index++] = 0;
            array[index++] = 1;
        }
    }
    return array;
}

int* RenderWidget0::makeSphereIndices(int slices, int points) {
    int* array = new int[slices*points*2*3];
    int index = 0;

    // The top of the sphere
    for (int p = 0; p < points; p++) {
        array[index++] = 0;
        array[index++] = 1 + p;
        array[index++] = 1 + p+1;
    }

    // The middle of the sphere
    for (int s = 1; s < slices; s++) {
        for (int p = 0; p < points; p++) {
            array[index++] = 1 + (s - 1) * points + 1 + p;
            array[index++] = 1 + (s - 1) * points + p;
            array[index++] = 1 + (s) * points + p;

            //array[index++] = 1 + (s) * points + p;
            //array[index++] = 1 + (s) * points + p + 1;
            //array[index++] = 1 + (s - 1) * points + p + 1;
        }
    }

    // The bottom of the sphere
    for (int p = 0; p < points; p++) {
        array[index++] = 1 + (slices - 2) * points + 1+p;
        array[index++] = 1 + (slices - 2) * points + p;
        array[index++] = 1 + (slices - 1) * points;
    }

    printf("uhh %d", 1 + (slices-1) * points);

    return array;
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
    Matrix4 m = Matrix4::rotateA(Vector3(0, 1, 0)  , 0.01);
    Matrix4 m2 = Matrix4::rotateZ(0.01);
    Matrix4 s = Matrix4::scale(1.001, 1.001, 1.001);
    Matrix4 m3 = Matrix4::rotateA(Vector3(3, 2, 1), 0.01);
    // Matrix4 m(cos(0.01),-sin(0.01),0,0, sin(0.01),cos(0.01),0,0, 0,0,1,0, 0,0,0,1);
    // Matrix4 m2(1,0,0,0, 0,cos(0.01), -sin(0.01),0, 0,sin(0.01),cos(0.01),0, 0,0,0,1);
	object->setTransformation(object->getTransformation()*m);
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
