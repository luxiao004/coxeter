#include "GeometryBuilder.h"
#include <iostream>

GeometryBuilder::GeometryBuilder(
	const std::vector<glm::vec3>& vertices
) {
	m_vertices = vertices;
	generateEdges();
}

void GeometryBuilder::generateEdges() {
	int n =m_vertices.size();
	for (int i = 0;i < n;i++) {
		for (int j = i + 1;j < n;j++) {
			float d = glm::length(m_vertices[i] - m_vertices[j]);
			m_edges.push_back({ i,j,d,-1});
		}
	}
}


std::vector<unsigned int> GeometryBuilder::buildEdges() {
	std::vector<unsigned int>indices;
	std::vector<float> layers;
	float eps = 0.0001;

	for (auto& edge : m_edges) {
		bool found = false;
		for (int i = 0;i < layers.size();i++) {
			if (fabs(edge.distance - layers[i]) < eps) {
				found = true;
				edge.layer = i;
				break;
			}
		}
		if (!found) {
			layers.push_back(edge.distance);
			edge.layer = layers.size() - 1;
		}
	}
	int minLayer = -1;
	float minDis = FLT_MAX;
	
	for (int i = 0;i < layers.size();i++) {
		if (layers[i] < minDis) {
			minDis = layers[i];
			minLayer = i;
		}
	}
	for (int i = 0;i < layers.size();i++) {
		std::cout << "layer" << i << "的dis值:" << layers[i] << std::endl;
	}
	std::cout << "minLayer:" << minLayer << std::endl;
	std::cout << "边拓扑关系：" << std::endl << "	";
	for (auto edge : m_edges) {
		if ( edge.layer == minLayer) {
			indices.push_back(edge.a);
			indices.push_back(edge.b);
		std::cout << edge.a << "->" << edge.b<<"  ";
		}
		
	}
	std::cout << std::endl;
	return indices;
}

