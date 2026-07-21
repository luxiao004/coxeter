#pragma once
#include <glad/glad.h>

class VertexArray {
public:
	VertexArray();
	~VertexArray();

	void bind()const;
	void unbind()const;

	void setLayout();
	GLuint GetID() {
		return m_ID;
	}
private:
	GLuint m_ID;
};