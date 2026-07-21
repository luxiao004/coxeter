#include "mesh.h"
#include <iostream>

Mesh::Mesh(const std::vector<glm::vec3>& vertices,const std::vector<unsigned>& indices,Primitive primitive)
	:m_VBO(vertices.data(),vertices.size()*sizeof(glm::vec3)),
	m_EBO(indices.data(),indices.size()),
	m_Primitive(primitive)
{
	m_VAO.bind();
	m_VBO.bind();
	m_EBO.bind();
	m_VAO.setLayout();
	/*std::cout << "VAO = " << m_VAO.GetID() << std::endl;
	std::cout << "VBO = " << m_VBO.GetID() << std::endl;
	std::cout << "EBO = " << m_EBO.GetID() << std::endl;*/
	std::cout<< m_VAO.GetID() << "号vao:" << std::endl;
	std::cout<<"	" << m_VBO.GetID() << "号vbo的顶点数量：" << m_VBO.getCount() << std::endl;
	std::cout<<"	" << m_EBO.GetID() << "号ebo的索引数量：" << m_EBO.getCount() << std::endl;
}

void Mesh::draw()const {
	m_VAO.bind();
	m_EBO.bind();
	//std::cout << m_VAO.GetID()<<m_VBO.GetID()<<m_EBO.GetID() << std::endl;
	GLenum mode;
	switch (m_Primitive) {
	case Primitive::Triangles:
		mode = GL_TRIANGLES;
		break;
	case Primitive::Lines:
		mode = GL_LINES;
		break;
	case Primitive::Points:
		mode = GL_POINTS;
		break;
	}
	if (mode == GL_POINTS) {
		glDrawArrays(
			GL_POINTS,
			0,
			m_VBO.getCount()
		);
		//std::cout << "点数是:" << m_VBO.getCount() << std::endl;
	}
	else if (mode == GL_LINES) {
		glDrawElements(
			mode,
			m_EBO.getCount(),
			GL_UNSIGNED_INT,
			nullptr
		);
	}

}