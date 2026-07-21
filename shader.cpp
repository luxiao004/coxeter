#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
	std::ifstream stream(filepath);
	
	enum class ShaderType {
		NONE=-1,VERTEX=0,FRAGMENT=1
	};
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}
	
	return { ss[0].str(),ss[1].str()};
}

unsigned int Shader::CompileShader(unsigned int type,const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")<<"shader"<<std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);
	glDeleteShader(vs);
	glDeleteShader(fs);
	return program;
}
void Shader::Bind()const {
	glUseProgram(ID);
}
void Shader::UnBind()const {
	glUseProgram(0);
}
Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath), ID(0) 
{
	ShaderProgramSource source = ParseShader(filepath);
	ID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() {
	glDeleteProgram(ID);
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}
void Shader::setVec3(const std::string& name,const glm::vec3 v) {
	glUniform3fv(
		GetUniformLocation(name),
		1,
		glm::value_ptr(v)
	);
}
void Shader::setMat4(const std::string& name, glm::mat4 m) {
	glUniformMatrix4fv(
		GetUniformLocation(name),
		1,//上传几个矩阵
		GL_FALSE,//要不要转置
		glm::value_ptr(m)//把glm::mat4变成float*
	);
}
unsigned int Shader::GetUniformLocation(const std::string& name) {
	unsigned int location=glGetUniformLocation(ID, name.c_str());
	if (location == -1)
		std::cout << "'Warning uniform'" << name << "'doesn't exist!'" << std::endl;
	return location;
}
