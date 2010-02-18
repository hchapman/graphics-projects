#include "SceneManager.h"
#include "RenderContext.h"

using namespace RE330;

SceneManager::SceneManager()
	: mCamera(0)
{
}

SceneManager::~SceneManager()
{
	if(mCamera)
	{
		delete mCamera;
	}
	while(mObjectList.size() > 0)
	{
		Object *o = mObjectList.front();
	    mObjectList.pop_front();
		delete o;
	} 
}

Object* SceneManager::createObject()
{	
	Object *o = new Object();
	mObjectList.push_back(o);

	return o;
}

Camera* SceneManager::createCamera()
{
	mCamera = new Camera();
	return mCamera;
}

Object* SceneManager::createSphere(float height, int slices, int points, const int num_colors, float color_list[][3])
{
    Object *sphere = createObject();
    float* sphere_v = sphereVertices(height, slices, points, num_colors);
    float* sphere_c = sphereColors(slices, points, num_colors, color_list);
    int* sphere_i = sphereIndices(slices, points, num_colors);
    int nVerts = (slices + 1) * points * num_colors;
    int nIndices = slices * points * 2 * 3;
    setupObject(sphere, nVerts, nIndices, sphere_v, sphere_c, sphere_i);	
    return sphere;
}

Object* SceneManager::createCone(float height, int base_points, const int num_colors, float color_list[][3])
{
    Object *cone = createObject();
    float* cone_v = coneVertices(height, base_points, num_colors);
    float* cone_c = coneColors(base_points, num_colors, color_list);
    int* cone_i = coneIndices(base_points, num_colors);
    int nVerts = (1 + base_points) * num_colors;
    int nIndices = 3 * base_points;
    setupObject(cone, nVerts, nIndices, cone_v, cone_c, cone_i);
    return cone;
}

Object* SceneManager::createBox(float height, float width, float depth, const int num_colors, float color_list[][3])
{
    Object *box = createObject();
    float* box_v = boxVertices(height, width, depth, num_colors);
    float* box_c = boxColors(num_colors, color_list);
    int* box_i = boxIndices(num_colors);
    int nVerts = 8 * num_colors;
    int nIndices = 3 * 12;
    setupObject(box, nVerts, nIndices, box_v, box_c, box_i);
    return box;
}

float* SceneManager::coneVertices(float height, int base_points, const int num_colors)
{
    float* array = new float[3 * num_colors * (1 + base_points)];
    // first make two top vertices for the
    int index = 0;
    for (int i = 0; i < num_colors; i++) {
        array[index++] = 0;
        array[index++] = height;
        array[index++] = 0;
    }
    // now do the base
    float theta;
    for (int i = 0; i < base_points; i++) {
        theta = 2 * M_PI / base_points * i;
        for (int j = 0; j < num_colors; j++) {
            array[index++] = height * cos(theta);
            array[index++] = -height;
            array[index++] = height * sin(theta);
        }
    }
    return array;
}

float* SceneManager::coneColors(int base_points, const int num_colors, float color_list[][3])
{
    float* array = new float[3 * num_colors * (1 + base_points)];
    int index = 0;
    // for the top point(s)
    for (int i = 0; i < num_colors; i++) {
        array[index++] = color_list[i][0];
        array[index++] = color_list[i][1];
        array[index++] = color_list[i][2];
    }
    for (int i = 0; i < base_points; i++) {
        for (int j = 0; j < num_colors; j++) {
            array[index++] = color_list[j][0];
            array[index++] = color_list[j][1];
            array[index++] = color_list[j][2];
        }
    }
    return array;    
}

int* SceneManager::coneIndices(int base_points, const int num_colors)
{
    // connect the top point to adjacent base points.
    int* array = new int[3 * base_points];
    // color_index keeps track of the color of the current triangle
    int color_index = 0;
    int index = 0;
    int i;
    for (i = 0; i < base_points - 1; i++) {
        // top point
        array[index++] = color_index;
        // base points
        array[index++] = num_colors + color_index + num_colors * i;
        array[index++] = 2 * num_colors + color_index + num_colors * i;
        color_index++;
        color_index %= num_colors;
    }
    // finally connect the last base point to the first base point.
    array[index++] = color_index;
    array[index++] = num_colors + color_index + num_colors * i;
    array[index++] = num_colors + color_index;
    return array;
}

float* SceneManager::sphereVertices(float height, int slices, int points, const int num_colors)
{
    float* array = new float[(slices + 1) * points * 3 * num_colors];
    int index = 0;
    float theta, phi;
    for(int s = 0; s <= slices; s++) {
        for(int p = 0; p < points; p++) {
            theta = 2*M_PI/points*p;
            phi = M_PI/slices*s;
            for (int i = 0; i < num_colors; i++) {
                array[index++] = height * cos(theta) * sin(phi);
                array[index++] = height * sin(theta) * sin(phi);
                array[index++] = height * cos(phi);
            }
        }
    }
    return array;
}

float* SceneManager::sphereColors(int slices, int points, const int num_colors, float color_list[][3])
{
    float* array = new float[(slices + 1) * points * 3 * num_colors];
    int index;
    index = 0;
    for(int i = 0; i <= slices; i++) {
        for(int j = 0; j < points; j++) {
            for (int k = 0; k < num_colors; k++) {
                array[index++] = color_list[k][0];
                array[index++] = color_list[k][1];
                array[index++] = color_list[k][2];
            }
        }
    }
    return array;
}

int* SceneManager::sphereIndices(int slices, int points, const int num_colors)
{
    int* array = new int[2 * (slices) * points * 3];
    int index = 0;
    int color_index = 0;
    for (int s = 0; s < slices; s++) {
        for (int p = 0; p < points; p++) {
            array[index++] = num_colors * (s * points + p + 1) + color_index;
            array[index++] = num_colors * (s * points + p) + color_index;
            array[index++] = num_colors * ((s + 1) * points + p) + color_index;
            color_index++;
            color_index %= num_colors;
            array[index++] = num_colors * ((s + 1) * points + p) + color_index;
            array[index++] = num_colors * ((s + 1) * points + p + 1) + color_index;
            array[index++] = num_colors * (s * points + p + 1) + color_index;
            color_index++;
            color_index %= num_colors;
        }
    }
    return array; 
}

float* SceneManager::boxVertices(float height, float width, float depth, const int num_colors)
{
    float* array = new float[3 * 8 * num_colors];
    float coords[3 * 8] = {
        -width / 2, height / 2, -depth / 2,
        -width / 2, height / 2, depth / 2,
        width / 2, height / 2, depth / 2,
        width / 2, height / 2, -depth / 2,
        -width / 2, -height / 2, -depth / 2,
        -width / 2, -height / 2, depth / 2,
        width / 2, -height / 2, depth / 2,
        width / 2, -height / 2, -depth / 2
    };
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < num_colors; j++) {
            for (int k = 0; k < 3; k++) {
                array[3 * num_colors * i + 3 * j + k] = coords[3 * i + k];
            }
        }
    }
    for (int i = 0; i < 3 * 8 * num_colors; i++) {
        printf("%f ", array[i]);
    }
    fflush(NULL);
    return array;
}

float* SceneManager::boxColors(const int num_colors, float color_list[][3])
{
    float* array = new float[num_colors * 3 * 8];
    int index = 0;
    for (int j = 0; j < 8; j++) {
        for (int i = 0; i < num_colors; i++) {
            array[index++] = color_list[i][0];
            array[index++] = color_list[i][1];
            array[index++] = color_list[i][2];
        }
    }
    return array;
}

int* SceneManager::boxIndices(const int num_colors)
{
    int* array = new int[12 * 3];
    int index = 0;
    int color_index = 0;
    array[index++] = 0 * num_colors + color_index;
    array[index++] = 1 * num_colors + color_index;
    array[index++] = 2 * num_colors + color_index;
    color_index++;
    color_index %= num_colors;
    array[index++] = 2 * num_colors + color_index;
    array[index++] = 3 * num_colors + color_index;
    array[index++] = 0 * num_colors + color_index;
    color_index++;
    color_index %= num_colors;
    array[index++] = 1 * num_colors + color_index;
    array[index++] = 5 * num_colors + color_index;
    array[index++] = 6 * num_colors + color_index;
    color_index++;
    color_index %= num_colors;
    array[index++] = 6 * num_colors + color_index;
    array[index++] = 2 * num_colors + color_index;
    array[index++] = 1 * num_colors + color_index;
    color_index++;
    color_index %= num_colors;
    array[index++] = 0 * num_colors + color_index;
    array[index++] = 4 * num_colors + color_index;
    array[index++] = 5 * num_colors + color_index;
    color_index++;
    color_index %= num_colors;
    array[index++] = 5 * num_colors + color_index;
    array[index++] = 1 * num_colors + color_index;
    array[index++] = 0 * num_colors + color_index;
    color_index++;
    color_index %= num_colors;
    array[index++] = 3 * num_colors + color_index;
    array[index++] = 2 * num_colors + color_index;
    array[index++] = 6 * num_colors + color_index;
    color_index++;
    color_index %= num_colors;
    array[index++] = 6 * num_colors + color_index;
    array[index++] = 7 * num_colors + color_index;
    array[index++] = 3 * num_colors + color_index;
    color_index++;
    color_index %= num_colors;
    array[index++] = 0 * num_colors + color_index;
    array[index++] = 3 * num_colors + color_index;
    array[index++] = 7 * num_colors + color_index;
    color_index++;
    color_index %= num_colors;
    array[index++] = 7 * num_colors + color_index;
    array[index++] = 4 * num_colors + color_index;
    array[index++] = 0 * num_colors + color_index;
    color_index++;
    color_index %= num_colors;
    array[index++] = 4 * num_colors + color_index;
    array[index++] = 7 * num_colors + color_index;
    array[index++] = 6 * num_colors + color_index;
    color_index++;
    color_index %= num_colors;
    array[index++] = 6 * num_colors + color_index;
    array[index++] = 5 * num_colors + color_index;
    array[index++] = 4 * num_colors + color_index;
    color_index++;
    color_index %= num_colors;
    return array;
}

void SceneManager::setupObject(Object* obj, int nVerts, int nIndices, float* v, float* c, int* i)
{
    VertexData& vd = obj->vertexData;
	// Specify the elements of the vertex data:
	// - one element for vertex positions
	vd.vertexDeclaration.addElement(0, 0, 3, 3*sizeof(float), RE330::VES_POSITION);
	// - one element for vertex colors
	vd.vertexDeclaration.addElement(1, 0, 3, 3*sizeof(int), RE330::VES_DIFFUSE);
	
	// Create the buffers and load the data
	vd.createVertexBuffer(0, nVerts*3*sizeof(float), (unsigned char*)v);
	vd.createVertexBuffer(1, nVerts*3*sizeof(float), (unsigned char*)c);
	vd.createIndexBuffer(nIndices, i);
}


void SceneManager::renderScene()
{
	RenderContext* renderContext = RenderContext::getSingletonPtr();

	if(mCamera!=0) 
	{
		renderContext->beginFrame();

		renderContext->setProjectionMatrix(mCamera->getProjectionMatrix());
		Matrix4 v = mCamera->getViewMatrix();

		// Iterate through list of objects
		std::list<Object *>::const_iterator iter;
		for (iter=mObjectList.begin(); iter!=mObjectList.end(); iter++)
		{
			Object *o = (*iter);
			Matrix4 m = o->getTransformation();

			renderContext->setModelViewMatrix(v*m);
			renderContext->render(o);
		}

		renderContext->endFrame();
	}
}