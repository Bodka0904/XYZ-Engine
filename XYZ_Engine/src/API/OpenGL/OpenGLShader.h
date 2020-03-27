#pragma once

#include "XYZ/Renderer/Shader.h"


namespace XYZ {
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& path);
		OpenGLShader(const std::string& name, const std::string& path);
		virtual ~OpenGLShader();
		virtual void Bind() const override;
		virtual void Compute(unsigned int groupX, unsigned int groupY = 1, unsigned int groupZ = 1) const override;
		virtual void Unbind() const override;
		virtual void SetUniforms(unsigned char* data) override;
		virtual void SetSubRoutine(const std::string& name) override;
		virtual void UploadRoutines() override;

		virtual void Reload() override;
		virtual void AddReloadCallback(std::function<void()> callback) override;


		virtual const Uniform* FindUniform(const std::string& name) override;
		virtual const TextureUniform* FindTexture(const std::string& name) override;

		inline virtual std::string GetPath() const override { return m_Path; };
		inline virtual std::string GetName() const override { return m_Name; }

		virtual unsigned int GetUniformSize() { return m_UniformsSize; };
	private:
		std::string readFile(const std::string& filepath);
		std::unordered_map<unsigned int, std::string> preProcess(const std::string& source);
		void compile(const std::unordered_map<unsigned int, std::string>& shaderSources);
		void parseUniforms();
		void parseSubRoutines();
		void parsePredefVariables(const std::string& filepath, std::string& source);
		void addUniform(UniformDataType type, unsigned int size, unsigned int offset, const std::string& name);


		void uploadInt(uint32_t loc, int value);
		void uploadFloat(uint32_t loc, float value);
		void uploadFloat2(uint32_t loc, const glm::vec2& value);
		void uploadFloat3(uint32_t loc, const glm::vec3& value);
		void uploadFloat4(uint32_t loc, const glm::vec4& value);
		void uploadMat3(uint32_t loc, const glm::mat3& matrix);
		void uploadMat4(uint32_t loc, const glm::mat4& matrix);
	private:
		uint32_t m_RendererID;
		ShaderProgramType m_Type;

		std::string m_Name;
		std::string m_Path;

		unsigned int m_NumTextures;
		unsigned int m_UniformsSize;

		std::vector<Uniform> m_Uniforms;
		std::vector<TextureUniform> m_Textures;
		std::vector<Routine> m_Routines;

		std::vector<std::function<void()>> m_ShaderReloadCallbacks;
	};

}