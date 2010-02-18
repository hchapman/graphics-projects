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

	// A cube
    const int slices = 8;
    const int points = 64;
    const int height = 2;
    const int base_points = 4;
    const int nVerts = 2 + 2 * base_points;
	const int nVertsSphere = (slices + 1) * points * 2;
    // float* sphere_vertices = makeSphereVertices(slices, points);
    // float* sphere_colors = makeSphereColors(slices, points);
    // int* sphere_indices = makeSphereIndices(slices, points);
    // float* pyr_vertices = makePyramidVertices(height, base_points);
    // float* pyr_colors = makePyramidColors(height, base_points);
    // int* pyr_indices = makePyramidIndices(height, base_points);
    
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
    // object = sceneManager->createObject();
    const int num_colors = 2;
    float color_list[num_colors][3] = {
        {((float) 0x11) / 0xFF, ((float) 0x22) / 0xFF, ((float) 0x33) / 0xFF},
        {((float) 0xD) / 0xF, ((float) 0xD) / 0xF, ((float) 0xD) / 0xF}
    };
    // sphere = sceneManager->createSphere(2, 8, 16, num_colors, color_list);
    cone = sceneManager->createCone(2, 10, num_colors, color_list);
    // box = sceneManager->createBox(2.0f, 2.0f, 2.0f, 2, num_colors, color_list);

	// Set up the vertex data
    // VertexData& vertexData = object->vertexData;
    // 
    // // Specify the elements of the vertex data:
    // // - one element for vertex positions
    // vertexData.vertexDeclaration.addElement(0, 0, 3, 3*sizeof(float), RE330::VES_POSITION);
    // // - one element for vertex colors
    // vertexData.vertexDeclaration.addElement(1, 0, 3, 3*sizeof(int), RE330::VES_DIFFUSE);
    // 
    // // Create the buffers and load the data
    // vertexData.createVertexBuffer(0, nVerts*3*sizeof(float), (unsigned char*)pyr_vertices);
    // vertexData.createVertexBuffer(1, nVerts*3*sizeof(float), (unsigned char*)pyr_colors);
    // // The index data that stores the connectivity of the triangles
    // int indices[] = {0,2,3, 0,1,2,           // front face
    //               4,6,7, 4,5,6,          // left face
    //               8,10,11, 8,9,10,       // back face
    //               12,14,15, 12,13,14,    // right face
    //               16,18,19, 16,17,18,    // top face
    //               20,22,23, 20,21,22};   // bottom face
    // vertexData.createIndexBuffer(2 * 3 * base_points, pyr_indices);

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
    Matrix4 m3 = Matrix4::rotateZ(0.01);
    counter++;
    counter = counter % 2000;
    Matrix4 s = Matrix4::scale(1.002, 1.002, 1.002);
    if (counter > 1000) {
        s = Matrix4::scale(1/1.002, 1/1.002, 1/1.002);        
    }
    Matrix4 ma = Matrix4::rotateA(Vector3(3, 2, 1), 0.01);
    // Matrix4 m(cos(0.01),-sin(0.01),0,0, sin(0.01),cos(0.01),0,0, 0,0,1,0, 0,0,0,1);
    // Matrix4 m2(1,0,0,0, 0,cos(0.01), -sin(0.01),0, 0,sin(0.01),cos(0.01),0, 0,0,0,1);
	cone->setTransformation(cone->getTransformation()*m*m2*m3);
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
