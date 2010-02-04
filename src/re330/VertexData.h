#ifndef __VertexData_h__
#define __VertexData_h__

#include "RE330_global.h"
#include "VertexDeclaration.h"

namespace RE330 
{
	/** This class manages vertex data.
	*/
	class RE330_EXPORT VertexData
	{
	public:
		VertexData();
		~VertexData();

		/** The VertexDeclaration stores a description of the vertex data
			stored in this object. The application program needs to initialize 
			it with the correct values.
		 */
		VertexDeclaration vertexDeclaration;

		/** Create a vertex buffer and store data. This method also assigns
			a given index to the buffer. 
		@remarks
			You also need to pass the index of this buffer and the specification 
			of the data stored in it to the vertexDeclaration. This is done 
			using vertexDeclaration.addElement().
		@param bufferIndex is the index assigned to the buffer.
		@param vertexBufferSize is the size of the buffer in bytes.
		@param data is a pointer to the data to be stored in the buffer.
		*/
		void createVertexBuffer(int bufferIndex, int vertexBufferSize, unsigned char *data);
		
		/** Create an index buffer.
		@param indexCount is the number of indices to be stored in the buffer.
		@param data is a pointer to the index data to be stored in the buffer.
		*/
		void createIndexBuffer(int indexCount, int *data);

		inline int getIndexCount() const { return mIndexCount; }
		inline int* getIndexBuffer() const { return mIndexBuffer; }

		/** The VertexBufferBinding stores a map from vertex buffer indices
			to pointers to the buffers. You do NOT need to access this 
			from the application program.
		*/
		VertexBufferBinding vertexBufferBinding;

	private:
		int mIndexCount;
		int *mIndexBuffer;
	};
	
}

#endif

