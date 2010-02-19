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