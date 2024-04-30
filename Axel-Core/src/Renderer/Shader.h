#pragma once

#include <glm.hpp>

typedef unsigned int GLenum;

namespace Axel
{
	class Shader
	{
	public:
		Shader(const std::string& filePath);
		Shader(const std::string& vertFilePath, const std::string& fragFilePath);
		~Shader();

		static std::shared_ptr<Shader> Create(const std::string& filePath);
		static std::shared_ptr<Shader> Create(const std::string& vertFilePath, const std::string& fragFilePath);

		void Bind();

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& vec);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& vec);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& vec);


		void UploadUniformFloat3Array(const std::string& name, uint32_t count, const float* value);

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformInt2(const std::string& name, const glm::vec2& vec);
		void UploadUniformInt3(const std::string& name, const glm::vec3& vec);
		void UploadUniformInt4(const std::string& name, const glm::vec4& vec);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		std::string ReadFile(const std::string& filePath);
		std::unordered_map<GLenum, std::string> Preprocess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSrcs);

	private:
		uint32_t m_ShaderID;
	};
}