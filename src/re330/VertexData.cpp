#include "VertexData.h"
#include "VertexDeclaration.h"
#include <stdio.h>

using namespace RE330;

VertexData::VertexData()
	: mIndexCount(0), mIndexBuffer(0)
{
}

VertexData::~VertexData()
{
	if(mIndexBuffer!=0)
	{
		delete[] mIndexBuffer;
	}
}

void VertexData::createVertexBuffer(int bufferIndex, int vertexBufferSize, unsigned char *data)
{
	VertexBuffer *buf = new VertexBuffer();
	buf->loadBuffer(vertexBufferSize, data);
	vertexBufferBinding.setBinding(bufferIndex, *buf);
}

void VertexData::createIndexBuffer(int indexCount, int *data)
{
	mIndexCount = indexCount;
	mIndexBuffer = new int[indexCount];
	for(int i=0; i<mIndexCount; i++) 
	{
		mIndexBuffer[i] = data[i];
	}
}


