#pragma once
#include<glad/glad.h>

class VertexBuffer {
public:
	VertexBuffer(const void* data,unsigned int size);
	~VertexBuffer();

	void bind()const;
	void unbind()const;
	inline unsigned int getCount()const { return m_Count; };
	GLuint GetID() {
		return m_ID;
	}
private:
	GLuint m_ID;
	unsigned int m_Count;
};