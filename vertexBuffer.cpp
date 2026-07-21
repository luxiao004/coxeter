#include "vertexBuffer.h"
#include "vertex.h"


VertexBuffer::VertexBuffer(const void* data, unsigned int size)
	:m_Count(size/sizeof(Vertex))
{
	glGenBuffers(1, &m_ID);

	glBindBuffer(GL_ARRAY_BUFFER, m_ID);

	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &m_ID);
}

void VertexBuffer::bind()const {
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void VertexBuffer::unbind()const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

