#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>
#include <unordered_map>

#include "mesh.h"
#include "coxeter.h"
#include "m_camera.h"
#include "shader.h"
#include "GeometryBuilder.h"

//int main() {
//	CoxeterGenerator coxeter(2, 3, 5);
//	coxeter.generate(0, 2);
//	GrapMesh mesh=coxeter.buildMesh();
//	for (auto i : mesh.vertices) {
//		std::cout <<"(" << i.x <<","<< i.y<<"," << i.z<<")" << std::endl;
//	}
//	
//	return 0;
//}


const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;
Camera camera;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
//===========算法==================

//=========回调函数==============
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
//-------------------主函数---------------------
int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Coxeter", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//=======数据========================
	camera.Position = glm::vec3(0, 0, 3);
	CoxeterGenerator coxeter(2, 3, 4);//coxeter关系：(233)(234)(235)
	coxeter.generate(0);//种子点：(0)(1)(2)(01)(02)(12)
	GrapMesh mesh=coxeter.buildMesh();//包括：顶点坐标，顶点自身的索引，三类反射边索引
	
	GeometryBuilder geo_builder(mesh.vertices);//初始化，数据引用
	auto geometry_edges=geo_builder.buildEdges();//枚举所有边->分类距离->找最短2条边->过滤索引->几何边的索引(创建Mesh)
	
	
	Shader pointShader("Resources/Shaders/Basic.shader");
	Shader edgeShader("Resources/Shaders/Basic01.shader");

	Mesh Point(mesh.vertices, mesh.indices,Primitive::Points);
	
	/*Mesh edge0(mesh.vertices, mesh.edge0, Primitive::Lines);
	Mesh edge1(mesh.vertices, mesh.edge1, Primitive::Lines);
	Mesh edge2(mesh.vertices, mesh.edge2, Primitive::Lines);*/
	
	Mesh geometryedge(mesh.vertices, geometry_edges, Primitive::Lines);
	
	//glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		float currentFram = glfwGetTime();
		deltaTime = currentFram - lastFrame;
		lastFrame = currentFram;

		glClearColor(0., 0., 0., 0.);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(window);		
		//====render=====
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		pointShader.Bind();
		pointShader.setMat4("model",model);
		pointShader.setMat4("view",view);
		pointShader.setMat4("projection",projection);
		glPointSize(12.0f);
		Point.draw();

		
		glPointSize(1.0f);
		edgeShader.Bind();
		edgeShader.setMat4("model", model);
		edgeShader.setMat4("view", view);
		edgeShader.setMat4("projection", projection);

		/*edgeShader.setVec3("color", { 1.0f,0.0f,0.0f });
		edge0.draw();
		edgeShader.setVec3("color", { 0.0f,1.0f,0.0f });
		edge1.draw();
		edgeShader.setVec3("color", { 0.0f,0.0f,1.0f });
		edge2.draw();*/
		edgeShader.setVec3("color", { 1.0f,1.0f,1.0f });
		geometryedge.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();//处理事件队列
	}
	glfwDestroyWindow(window);//销毁指定的窗口及其上下文
	glfwTerminate();//关闭GLFW库本身
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

