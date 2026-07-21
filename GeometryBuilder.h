#pragma once
#include <vector>
#include <glm/glm.hpp>

struct GeometryEdge {
	int a;
	int b;

	float distance;
	int layer;
};


class GeometryBuilder {
public:
	GeometryBuilder(
		const std::vector<glm::vec3>& vertices
	);
	std::vector<unsigned int> buildEdges();
	
private:
	std::vector<GeometryEdge> m_edges;
	std::vector<glm::vec3> m_vertices;
private:
	void generateEdges();
};