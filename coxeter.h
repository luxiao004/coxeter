#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <glm/glm.hpp>
#include "vertex.h"

const double PI = 3.14159265358979323846;

struct Edge {
	unsigned a;
	unsigned b;
	int mirror;
};
struct GrapMesh {
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;

	std::vector<unsigned int> edge0;
	std::vector<unsigned int> edge1;
	std::vector<unsigned int> edge2;
};
struct Node {
	Node* reflect[3]{};
	glm::vec3 pos;
	unsigned id;
};
struct Mirrors {
	glm::vec3 n1, n2, n3;
};
class CoxeterGenerator {
public:
	CoxeterGenerator(int p, int r, int q);//根据coxeter关系生成三镜面法向量
	~CoxeterGenerator();
	void generate(int selfA, int selfB);//初始化种子点
	void generate(int selfA);
	GrapMesh buildMesh()const;
	
private:
	//===========几何==========
	Mirrors buildMirrors();
	glm::vec3 getNormal(int id, const Mirrors& M);
	glm::vec3 sphereIntersectionPoint(glm::vec3 n1, glm::vec3 n2, float R = 1.0f);//取基本域（球面三角形）的顶点
	glm::vec3 wythoffEdgeSeed(int selfA,const Mirrors& M);//取基本域的某条边上的合适点
	//=========图===============
	Node* createNode();
	void connect(Node*, Node*, int);//连接两个节点
	//========算法================
	void reflect(Node* node);//尝试反射
	void huisu(Node* node, int q, int p);//交替回溯算法
	int getLen(int a, int b)const;
	int countState(Node* a)const;
	
private:
	std::vector<Edge> edges;
	Mirrors m_Mirrors;
	std::vector<Node*> m_Nodes;
	std::queue<Node*> m_Queue;
	unsigned m_nextID = 0;
	int m_P;
	int m_Q;
	int m_R;
};



