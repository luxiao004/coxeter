#include "coxeter.h"
#include <iomanip>

//求两个大圆的交点(即两个镜面的交线与单位球面的交点)
glm::vec3 CoxeterGenerator::sphereIntersectionPoint(glm::vec3 n1, glm::vec3 n2,float R)
{
	glm::vec3 dir = glm::cross(n1, n2);

	if (glm::dot(dir, dir) < 1e-8f)
		return { 0,0,0 };

	glm::vec3 d = glm::normalize(dir);

	return { d.x * R, d.y * R, d.z * R };
}
glm::vec3 CoxeterGenerator::getNormal(int id, const Mirrors& M) {
	switch (id)
	{
	case 0: return M.n1;
	case 1: return M.n2;
	default:return M.n3;
	}
}


//根据wythoff符号(p,q,r)构建镜面(即3个大圆的法向量)
Mirrors CoxeterGenerator::buildMirrors() {
	double cp = -glm::cos(PI / m_P);
	double cq = -glm::cos(PI / m_Q);
	double cr = -glm::cos(PI / m_R);

	glm::vec3 n1 = { 1,0,0 };

	double a = cp;
	double b = glm::sqrt(1.0 - a * a);

	glm::vec3 n2 = { a,b,0 };
	double c = cr;
	double d = (cq - a * c) / b;
	double temp = 1.0 - c * c - d * d;

	if (temp < 0) {
		std::cout << "Invalid parameters for mirrors." << std::endl;
		exit(0);
	}
	double e = std::sqrt(temp);
	glm::vec3 n3 = { c,d,e };
	n1 = glm::normalize(n1);
	n2 = glm::normalize(n2);
	n3 = glm::normalize(n3);

	return { n1, n2, n3 };
}

Node* CoxeterGenerator::createNode() {
	Node* n = new Node();
	n->id =m_nextID++;
	m_Nodes.push_back(n);
	return n;
}

void CoxeterGenerator::connect(Node* a, Node* b, int gen) {
	//给两个节点建立连接，gen表示连接的类型(0,1,2)
	a->reflect[gen] = b;
	b->reflect[gen] = a;
	edges.push_back({ a->id, b->id,gen });
}
int CoxeterGenerator::getLen(int a, int b)const {
	if ((a == 0 && b == 1) || (a == 1 && b == 0))return m_P;
	if ((a == 0 && b == 2) || (a == 2 && b == 0))return m_R;
	if ((a == 1 && b == 2) || (a == 2 && b == 1))return m_Q;
}
void CoxeterGenerator::huisu(Node* a, int q, int p) {
	int c = 0;
	Node* t = a;//临时指针
	int len = getLen(q, p);//得到q,p的coxeter关系
	int yue = len;
	int huan = 2 * len - 1;

	int j = q;
	while (t->reflect[j] != nullptr && t->reflect[j] != t) {

		if (j == q) {
			t = t->reflect[q];
		}
		else {
			t = t->reflect[p];
		}
		c++;
		j = (j == q ? p : q);
	}
	if (t->reflect[j] == t) {
		c += 1;
		bool isyue = c == yue;
		if (isyue) {
			a->reflect[p] = a;
			
		}
		else {
			Node* v = createNode();
			glm::vec3 n = p == 0 ? m_Mirrors.n1 : (p == 1 ? m_Mirrors.n2 : m_Mirrors.n3);
			v->pos = glm::reflect(a->pos, n);
			connect(a, v, p);
			m_Queue.push(v);
		}
	}
	else {
		bool ishuan = c == huan;
		if (ishuan) {
			connect(a, t, j);
		
		}
		else {

			Node* v = createNode();
			glm::vec3 n = p == 0 ? m_Mirrors.n1 : (p == 1 ? m_Mirrors.n2 : m_Mirrors.n3);
			v->pos =glm::reflect(a->pos, n);
			connect(a, v, p);
			m_Queue.push(v);
		}
	}


}
int CoxeterGenerator::countState(Node* a)const {
	int c = 0;
	for (int i = 0;i < 3;i++) {
		if (a->reflect[i] != nullptr)
			c++;
	}
	return c;
}
void CoxeterGenerator::reflect(Node* a) {
	int q = -1;//找到当前节点是由谁反射而来的,假设1
	int count = 0;
	int temp[3] = { 0 };
	int l = -1;
	for (int i = 0;i < 3;i++) {
		if (a->reflect[i] != nullptr) {
			q = i;
			temp[i] = 1;
			count++;
		}
		else {
			l = i;
		}
	}
	if (count == 2) {
		for (int i = 0;i < 3;i++) {
			if (temp[i] == 1 && countState(a) != 3) {//此处考虑不完备		
				huisu(a, i, l);
			}
		}
	}
	else {
		//那么可能的组合是12,10
		for (int i = 0;i < 3;i++) {
			if (i != q) {
				huisu(a, q, i);
			}
		}
	}


}
GrapMesh CoxeterGenerator::buildMesh()const {
	GrapMesh mesh;
	std::cout << "顶点坐标数据(以下所有数据都是保留3位的近似)：" << std::endl << "	";
	std::cout << std::fixed << std::setprecision(3);
	for (Node* n : m_Nodes) {
		mesh.vertices.push_back(n->pos);
		std::cout <<"(" << n->pos.x << "," << n->pos.y << "," << n->pos.z << ")" << "  ";
		
		mesh.indices.push_back(n->id);
	}
	std::cout << std::endl;
	//std::cout << "反射边索引关系是(a,b,c表示三镜面,0,1...表示顶点编号)：" << std::endl;
	for (auto edge : edges) {
		//std::cout << edge.a << edge.b << std::endl;
		if (edge.mirror == 0) {
			mesh.edge0.push_back(edge.a);
			mesh.edge0.push_back(edge.b);
			//std::cout <<"a:" << edge.a << " -> " << edge.b << std::endl;
		}
		else if (edge.mirror == 1) {
			mesh.edge1.push_back(edge.a);
			mesh.edge1.push_back(edge.b);
			//std::cout << "b:" << edge.a << " -> " << edge.b << std::endl;
		}
		else {
			mesh.edge2.push_back(edge.a);
			mesh.edge2.push_back(edge.b);
			//std::cout << "c:" << edge.a << " -> " << edge.b << std::endl;
		}
	}
	//std::cout << mesh.vertices.size();
	return mesh;
}

void CoxeterGenerator::generate(int selfA, int selfB)
{
	Node* root = createNode();
	root->reflect[selfA] = root;
	root->reflect[selfB] = root;
	glm::vec3 na = selfA == 0 ? m_Mirrors.n1 : (selfA == 1 ? m_Mirrors.n2 : m_Mirrors.n3);
	glm::vec3 nb = selfB == 0 ? m_Mirrors.n1 : (selfB == 1 ? m_Mirrors.n2 : m_Mirrors.n3);
	root->pos = sphereIntersectionPoint(na, nb);
	m_Queue.push(root);
	while (!m_Queue.empty()) {
		Node* u = m_Queue.front();
		m_Queue.pop();
		reflect(u);
	}
	
}

void CoxeterGenerator::generate(int selfA)
{
	Node* root = createNode();
	root->reflect[selfA] = root;


	root->pos =
		wythoffEdgeSeed(
			selfA,
			m_Mirrors
		);
	/*root->reflect[selfA] = root;
	root->pos = sphereEdgeMidPoint(selfA,m_Mirrors);*/
	//std::cout << root->pos.x << root->pos.y << root->pos.z << std::endl;
	m_Queue.push(root);
	while (!m_Queue.empty()) {
		Node* u = m_Queue.front();
		m_Queue.pop();
		reflect(u);
	}
}
CoxeterGenerator::CoxeterGenerator(int p, int r, int q) 
	:m_P(p),m_Q(q),m_R(r)
{
	m_Mirrors = buildMirrors();
}
CoxeterGenerator::~CoxeterGenerator() {
	for (Node* n : m_Nodes) {
		delete n;
	}
}

glm::vec3 CoxeterGenerator::wythoffEdgeSeed(
	int selfA,
	const Mirrors& M)
{
	int mirrorA = (selfA + 1) % 3;
	int mirrorB = (selfA + 2) % 3;
	glm::vec3 nA = getNormal(selfA, M);
	glm::vec3 nB = getNormal(mirrorA, M);
	glm::vec3 nC = getNormal(mirrorB, M);


	// 两个约束面的法向
	glm::vec3 c1 = nA;
	glm::vec3 c2 = nB - nC;


	// 两个平面的交线方向
	glm::vec3 dir = glm::cross(c1, c2);


	return glm::normalize(dir);
}