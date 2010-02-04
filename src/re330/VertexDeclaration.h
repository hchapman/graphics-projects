#ifndef __VertexDeclaration_h__
#define __VertexDeclaration_h__

#include "RE330_global.h"
#include <list>
#include <map>

namespace RE330 
{
	enum VertexElementSemantic {
		VES_POSITION = 1,
		VES_NORMAL = 2,
		VES_DIFFUSE = 3,
		VES_SPECULAR = 4,
		VES_TEXTURE_COORDINATES = 5
	};

	/** This class specificies a vertex element, and how it is stored
		in a buffer. It should NOT be used directly from applications.
	*/
	class RE330_EXPORT VertexElement
	{
	public:
		VertexElement(int bufferIndex, int offset, int size, int stride, VertexElementSemantic veSemantic)
			: mBufferIndex(bufferIndex), mOffset(offset), mSize(size), mStride(stride), mSemantic(veSemantic) {}
		int getBufferIndex() const { return mBufferIndex; }
		int getOffset() const { return mOffset; }
		int getSize() const { return mSize; }
		int getStride() const { return mStride; }
		VertexElementSemantic getSemantic() const { return mSemantic; }

	private:
		int mBufferIndex;
		int mOffset;
		int mSize;
		int mStride;
		VertexElementSemantic mSemantic;
	};

	/** The application program uses this class to specify the format of vertex data. 
		Vertex data is stored in a list of buffers, and each buffer has a certain format. 
		Each buffer may contain one or several vertex elements, such as position, color, 
		etc.
	@remarks
		Vertex buffers store items of type float exclusively.
	*/
	class RE330_EXPORT VertexDeclaration
	{
	public:
		typedef std::list<VertexElement> VertexElementList;

		/** Specify a vertex element and how it is stored in a given buffer.
		@param
			bufferIndex is the index of the buffer where the element is stored.
		@param
			offset is the offset into the buffer (in bytes) where the first data
			item is stored.
		@param
			size is the number of items stored for each vertex. For example, for normal
			vectors with 3 components, size=3.
		@param
			stride is the offset, in bytes, between data for consecutive vertices.
		@param
			veSemantic is the semantic of the vertex element, i.e., position, color,
			etc. Make sure that each vertex declaration has no more than one element of 
			each type!
		*/
		void addElement(int bufferIndex, int offset, int size, int stride, VertexElementSemantic veSemantic);

		int getElementCount() const;
		const VertexElement* getElement(int index) const;

	protected:
		VertexElementList mElementList;
	};

	/** This class stores vertex data. It should NOT be accessed directly from 
		applications. In an advanced implementation of the rendering engine,
		this could be implemented as OpenGL vertex buffer objects.
	*/
	class RE330_EXPORT VertexBuffer
	{
	public:
		VertexBuffer() : mBuffer(0) {}
		~VertexBuffer();
		void loadBuffer(int bufferSize, unsigned char *data);
		unsigned char *getBuffer() const { return mBuffer; }

	private:
		unsigned char *mBuffer;
	};

	/** This class maps vertex buffer indices to pointers to vertex buffers.
		It should NOT be accessed directly from applications.
	*/
	class RE330_EXPORT VertexBufferBinding
	{
	public:
		typedef std::map<unsigned short, VertexBuffer> VertexBufferBindingMap;
	protected:
		VertexBufferBindingMap mBindingMap;
	public:
		void setBinding(unsigned short index, const VertexBuffer& buffer);
		const VertexBuffer& getBuffer(unsigned short index) const;
	};
	
}

#endif

