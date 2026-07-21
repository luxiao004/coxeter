#include "vertexArray.h"
#include "vertex.h"

VertexArray::VertexArray() {
	glGenVertexArrays(1, &m_ID);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &m_ID);
}

void VertexArray::bind()const{
	glBindVertexArray(m_ID);
}

void VertexArray::unbind()const {
	glBindVertexArray(0);
}

void VertexArray::setLayout() {
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	/*glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,mirror));*/
}
