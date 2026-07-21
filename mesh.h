#pragma once
#include <vector>

#include "vertex.h"

#include "vertexArray.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"

enum class Primitive {
	Triangles,
	Lines,
	Points
};
class Mesh {
public:
	Mesh(const std::vector<glm::vec3>& vertices,
		const std::vector<unsigned>& indices,
		Primitive primitive
	);
	/*Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;*/
	void draw()const;
private:
	VertexArray m_VAO;
	VertexBuffer m_VBO;
	IndexBuffer m_EBO;
	Primitive m_Primitive;
};