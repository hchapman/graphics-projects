#ifndef __SceneManager_h__
#define __SceneManager_h__

#include "RE330_global.h"
#include "Camera.h"
#include "Object.h"
#include "RenderContext.h"
#include <list>

namespace RE330 
{
	/** This class provides an abstraction of a scene. It manages a camera,
		objects in the scene, etc. It is the main interface for applications
		to the rendering engine.
	*/
	class RE330_EXPORT SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		/** This method creates a default camera.
		*/
		Camera *createCamera();

		/** This method creates an object and adds it to the list of object
			stored in the scene manager.
		*/
		Object *createObject();
		
		/** These methods create basic shapes.
		
		*/
        Object *createSphere(float height, int slices, int points, const int num_colors, float color_list[][3]);
        Object *createCone(float height, int base_points, const int num_colors, float color_list[][3]);
        Object *createBox(float height, float width, float depth, const int num_colors, float color_list[][3]);
        
        float* coneVertices(float height, int base_points, const int num_colors);
        float* coneColors(int base_points, const int num_colors, float color_list[][3]);
        int* coneIndices(int base_points, const int num_colors);
        float* sphereVertices(float height, int slices, int points, const int num_colors);
        float* sphereColors(int slices, int points, const int num_colors, float color_list[][3]);
        int* sphereIndices(int slices, int points, const int num_colors);
        float* boxVertices(float height, float width, float depth, const int num_colors);
        float* boxColors(const int num_colors, float color_list[][3]);
        int* boxIndices(const int num_colors);
        void setupObject(Object* obj, int nVerts, int nIndices, float* v, float* c, int* i);

		/** This method needs to be called in the renderSceneEvent
			event handler of the RenderWidget. 
		@remarks
			You should *not* call this anywhere else in your code except in 
			the renderScene event handler of your RenderWidget. To trigger 
			rendering, call updateScene of your RenderWidget, which will then 
			trigger the renderScene event handler to be called. 
		*/
		void renderScene();

	private:
		Camera *mCamera;
		std::list<Object*> mObjectList;
	};

}

#endif