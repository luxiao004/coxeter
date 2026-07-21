#pragma once
#include <string>
#include <glm/glm.hpp>

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};
class Shader {
public:
	Shader(const std::string& filename);
	~Shader();

	void Bind()const;
	void UnBind()const;
	
	void setUniform4f(const std::string& name, float v0,float v1,float v2,float v3); 
	void setInt(const std::string&,int);
	void setFloat(const std::string&,float);
	 
	void setVec2();
	void setVec3(const std::string& name,const glm::vec3);
	void setVec4();

	void setMat4(const std::string& name,glm::mat4 m);
private:
	std::string m_FilePath;
	unsigned int ID;
private:
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int GetUniformLocation(const std::string& name);
};