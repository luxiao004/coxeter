#include "indexBuffer.h"

IndexBuffer::IndexBuffer(const void* data, unsigned int count)
	:m_Count(count) 
{
	glGenBuffers(1, &m_ID);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER,count*sizeof(unsigned int), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &m_ID);
}

void IndexBuffer::bind()const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void IndexBuffer::unbind()const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

