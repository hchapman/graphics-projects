#include "GLRenderContext.h"

using namespace RE330;

GLRenderContext* GLRenderContext::getSingletonPtr(void)
{
	return static_cast<GLRenderContext *>(ms_Singleton);
}
GLRenderContext& GLRenderContext::getSingleton(void)
{  
	assert( ms_Singleton );  
	return ( *(static_cast<GLRenderContext *>(ms_Singleton)) );  
}

void GLRenderContext::init()
{
	assert(glewInit() == GLEW_OK);
	assert(GL_VERSION_2_0);

	glShadeModel(GL_SMOOTH);    // turns on shading 
	glEnable(GL_DEPTH_TEST);    // activates depth buffer for hidden surface removal
	glEnable(GL_CULL_FACE);     // makes faces invisible
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // clears color and depth buffer
}

void GLRenderContext::setViewport(int width, int height)
{
	glViewport(0,0, width, height);    // sets size of area of window where graphics are displayed
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderContext::beginFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderContext::endFrame()
{
	glFlush();    // forces previously issued OpenGL commands to begin execution
}

void GLRenderContext::setModelViewMatrix(const Matrix4 &m)
{
	glMatrixMode(GL_MODELVIEW);    // selects model view matrix (used to describe affine transformations)
	glLoadMatrixf(m.transpose().getElementPointer());    // sets model view matrix to transpose of m
}

void GLRenderContext::setProjectionMatrix(const Matrix4 &m)
{
	glMatrixMode(GL_PROJECTION);    // selects projection matrix (used to describe orthographic and 
	                                // perspective transformations)
	glLoadMatrixf(m.transpose().getElementPointer());    // sets projection matrix to transpose of m
}

void GLRenderContext::render(Object *object)
{
	VertexData& vertexData = object->vertexData;
	VertexDeclaration& vertexDeclaration = vertexData.vertexDeclaration;
	VertexBufferBinding& vertexBufferBinding = vertexData.vertexBufferBinding;

	// The basic way to draw triangles in OpenGL is to use glBegin(GL_TRIANGLES),
	// then specify vertices, colors, etc. using glVertex(), glColor(), etc., and finish
	// with glEnd(). However, we are using a more advanced technique here that is based
	// on so-called vertex arrays. Vertex arrays are more efficient because they need
	// fewer function calls to OpenGL. Read more about vertex arrays in Chapter 2 of the
	// OpenGL book.

	// Set up vertex arrays
	for(int j=0; j<vertexDeclaration.getElementCount(); j++)
	{
		const VertexElement *element = vertexDeclaration.getElement(j);

		const VertexBuffer& vertexBuffer = vertexBufferBinding.getBuffer(element->getBufferIndex());
		unsigned char* buf = vertexBuffer.getBuffer();

		GLsizei vertexStride = static_cast<GLsizei>(element->getStride());
		GLint vertexSize = static_cast<GLint>(element->getSize());
		int offset = element->getOffset();

		switch(element->getSemantic())
		{
			case VES_POSITION :
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(vertexSize, GL_FLOAT, vertexStride, buf+offset);
				break;
			case VES_NORMAL :
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT, vertexStride, buf+offset);
				break;
			case VES_DIFFUSE :
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(vertexSize, GL_FLOAT, vertexStride, buf+offset);
				glEnable(GL_COLOR_MATERIAL);
				break;
			case VES_TEXTURE_COORDINATES :
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(vertexSize, GL_FLOAT, vertexStride, buf+offset);
				break;
		}
	}
	
	// Draw
	glDrawElements(GL_TRIANGLES, vertexData.getIndexCount(), GL_UNSIGNED_INT, vertexData.getIndexBuffer());

	// Disable the arrays we used
	for(int j=0; j<vertexDeclaration.getElementCount(); j++)
	{
		const VertexElement *element = vertexDeclaration.getElement(j);

		switch(element->getSemantic())
		{
			case VES_POSITION :
				glDisableClientState(GL_VERTEX_ARRAY);
				break;
			case VES_NORMAL :
				glDisableClientState(GL_NORMAL_ARRAY);
				break;
			case VES_DIFFUSE :
				glDisableClientState(GL_COLOR_ARRAY);
				break;
			case VES_TEXTURE_COORDINATES :
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				break;
		}
	}

	assert(glGetError()==GL_NO_ERROR);
}
