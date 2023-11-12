#include "Shader.h"

#include <gtc/type_ptr.hpp>

static GLenum ShaderTypeFromString(const std::string& type)
{
	if (type == "vertex")
		return GL_VERTEX_SHADER;
	if (type == "fragment" || type == "pixel")
		return GL_FRAGMENT_SHADER;

	return 0;
}

// Use for single file
Shader::Shader(const std::string& filePath)
{
	std::string sources = ReadFile(filePath);
	auto splitSources = Preprocess(sources);
	Compile(splitSources);
}

// Use for seperate files
Shader::Shader(const std::string& vertFilePath, const std::string& fragFilePath)
{
	std::unordered_map<GLenum, std::string> shaderSources;

	std::string vs = ReadFile(vertFilePath);
	std::string fs = ReadFile(fragFilePath);

	shaderSources.insert({ GL_VERTEX_SHADER, vs });
	shaderSources.insert({ GL_FRAGMENT_SHADER, fs });

	Compile(shaderSources);
}

Shader::~Shader()
{
	glDeleteProgram(m_ShaderID);
}

std::shared_ptr<Shader> Shader::Create(const std::string& filePath)
{
	return std::make_shared<Shader>(filePath);
}

std::shared_ptr<Shader> Shader::Create(const std::string& vertFilePath, const std::string& fragFilePath)
{
	return std::make_shared<Shader>(vertFilePath, fragFilePath);
}

void Shader::Bind()
{
	glUseProgram(m_ShaderID);
}

std::string Shader::ReadFile(const std::string& filePath)
{
	std::string result;
	std::ifstream in(filePath, std::ios::in, std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		result.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		in.close();
	}
	else
	{
		printf("Could not open shader file.");
	}

	return result;

}

std::unordered_map<GLenum, std::string> Shader::Preprocess(const std::string& source)
{
	std::unordered_map<GLenum, std::string> shaderSources;

	const char* typeToken = "#type";
	size_t typeTokenLength = strlen(typeToken);
	size_t pos = source.find(typeToken, 0);
	while (pos != std::string::npos)
	{
		size_t eol = source.find_first_of("\r\n", pos);
		size_t begin = pos + typeTokenLength + 1;
		std::string type = source.substr(begin, eol - begin);

		size_t nextLinePos = source.find_first_not_of("\r\n", eol);
		pos = source.find(typeToken, nextLinePos);
		shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
	}

	return shaderSources;
}

void Shader::Compile(const std::unordered_map<GLenum, std::string>& shaderSrcs)
{
	GLuint program = glCreateProgram();
	std::vector<GLenum> glShaderIDs(shaderSrcs.size());
	for (auto& kv : shaderSrcs)
	{
		GLenum type = kv.first;
		const std::string& src = kv.second;

		GLuint shader = glCreateShader(type);

		const char* source = src.c_str();
		glShaderSource(shader, 1, &source, 0);

		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == false)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(shader);

			printf("Program Compilation falied: ");
			printf(infoLog.data());
			printf("\n");
			break;
		}

		glAttachShader(program, shader);
		glShaderIDs.push_back(shader);
	}

	glLinkProgram(program);

	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == false)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(program);

		for (auto id : glShaderIDs)
			glDeleteShader(id);

		printf("Program Compilation falied: ");
		printf(infoLog.data());
		printf("\n");
		return;
	}

	for (auto id : glShaderIDs)
		glDetachShader(m_ShaderID, id);

	m_ShaderID = program;
}

// ----------- Uniforms ----------- //

void Shader::UploadUniformFloat(const std::string& name, float value)
{
	GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
	glUniform1f(location, value);
}

void Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& vec)
{
	GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
	glUniform2f(location, vec.x, vec.y);
}

void Shader::UploadUniformFloat3(const std::string& name, const glm::vec3& vec)
{
	GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
	glUniform3f(location, vec.x, vec.y, vec.z);
}

void Shader::UploadUniformFloat4(const std::string& name, const glm::vec4& vec)
{
	GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
	glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void Shader::UploadUniformInt(const std::string& name, int value)
{
	GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
	glUniform1i(location, value);
}

void Shader::UploadUniformInt2(const std::string& name, const glm::vec2& vec)
{
	GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
	glUniform2i(location, vec.x, vec.y);
}

void Shader::UploadUniformInt3(const std::string& name, const glm::vec3& vec)
{
	GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
	glUniform3i(location, vec.x, vec.y, vec.z);
}

void Shader::UploadUniformInt4(const std::string& name, const glm::vec4& vec)
{
	GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
	glUniform4i(location, vec.x, vec.y, vec.z, vec.w);
}

void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
{
	GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
{
	GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}