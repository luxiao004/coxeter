#pragma once
#include <glad/glad.h>

class IndexBuffer {
public:
	IndexBuffer(const void* data,unsigned int count);
	~IndexBuffer();
	
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