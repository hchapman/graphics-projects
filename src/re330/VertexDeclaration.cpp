#include "VertexDeclaration.h"
#include <assert.h>
#include <cstring>

using namespace RE330;


void VertexDeclaration::addElement(int bufferIndex, int offset, int size, int stride, VertexElementSemantic veSemantic)
{
	mElementList.push_back(VertexElement(bufferIndex, offset, size, stride, veSemantic));
}

int VertexDeclaration::getElementCount() const 
{ 
	return mElementList.size(); 
}

const VertexElement* VertexDeclaration::getElement(int index) const
{
	assert(index < mElementList.size() && "Index out of bounds.");

	VertexElementList::const_iterator i = mElementList.begin();
	for (int n = 0; n < index; ++n)
		++i;

	return &(*i);
}

VertexBuffer::~VertexBuffer()
{
	if(mBuffer!=0)
	{
		delete[] mBuffer;
	}
}

void VertexBuffer::loadBuffer(int bufferSize, unsigned char *data)
{
	mBuffer = new unsigned char[bufferSize];
	memcpy(mBuffer, data, bufferSize);
}


void VertexBufferBinding::setBinding(unsigned short index, const RE330::VertexBuffer &buffer)
{
	mBindingMap[index] = buffer;
}

const VertexBuffer& VertexBufferBinding::getBuffer(unsigned short index) const
{
	VertexBufferBindingMap::const_iterator i = mBindingMap.find(index);

	assert(i!=mBindingMap.end() && "No buffer bound to index.");

	return i->second;
}

