
#include "Zero/Renderer/Shader.h"
#include <glad/glad.h>

namespace zr
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader();
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
		OpenGLShader(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath);
		virtual ~OpenGLShader();

		// Inherited via Shader
		virtual const std::string& getName() const;
		virtual bool loadFromFile(const std::string& filePath) override;
		virtual bool loadFromFiles(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath) override;
		virtual bool loadFromFiles(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath) override;
		virtual bool loadFromStrings(const std::string& name, const std::string& vertexShader, const std::string& fragmentShader) override;
		virtual bool loadFromStrings(const std::string& name, const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader) override;
		virtual void bind() const override;
		virtual void unbind() const override;
		virtual void setUniform(const char* uniformName, float value) const override;
		virtual void setUniform(const char* uniformName, float valueX, float valueY) const override;
		virtual void setUniform(const char* uniformName, float valueX, float valueY, float valueZ) const override;
		virtual void setUniform(const char* uniformName, float valueX, float valueY, float valueZ, float valueW) const override;
		virtual void setUniform(const char* uniformName, int value) const override;
		virtual void setUniform(const char* uniformName, int valueX, int valueY) const override;
		virtual void setUniform(const char* uniformName, int valueX, int valueY, int valueZ) const override;
		virtual void setUniform(const char* uniformName, int valueX, int valueY, int valueZ, int valueW) const override;
		virtual void setUniform(const char* uniformName, bool valueX) const override;
		virtual void setUniform(const char* uniformName, bool valueX, bool valueY) const override;
		virtual void setUniform(const char* uniformName, bool valueX, bool valueY, bool valueZ) const override;
		virtual void setUniform(const char* uniformName, bool valueX, bool valueY, bool valueZ, bool valueW) const override;
		virtual void setUniform(const char* uniformName, const glm::vec2& value) const override;
		virtual void setUniform(const char* uniformName, const glm::vec3& value) const override;
		virtual void setUniform(const char* uniformName, const glm::vec4& value) const override;
		virtual void setUniform(const char* uniformName, const glm::mat3& value) const override;
		virtual void setUniform(const char* uniformName, const glm::mat4& value) const override;
		virtual void setUniform(const char* uniformName, const Material& value) const override;
		virtual void setUniform(const char* uniformName, const Light& value) const override;
		virtual void setUniform(const char* uniformName, const std::vector<glm::mat4>& value) const override;

	private:
		std::string readFile(const std::string& filePath);
		std::unordered_map<unsigned char, std::string> preProcess(const std::string& source);

		/**
		 * @brief Checks for compilation errors after a shader program is loaded.
		 *
		 * @param shaderType	The type of the shader to check for compilations errors.
		 * @param shaderId  	The handler for the shader to check for compilations errors.
		 *
		 * @returns true when compilated successfully, false otherwise.
		 */
		bool checkCompilationError(ShaderType shaderType, GLuint shaderId) const;

		/**
		 * @brief Tries to create the vertex and fragment shader from files.
		 *
		 * @param vertexShaderPath  	The path of the vertex shader file.
		 * @param fragmentShaderPath	The path of the fragment shader file.
		 *
		 * @returns true when created successfully, false otherwise.
		 */
		bool createVertexAndFragShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

		/**
		 * @brief Tries to create the geometry shader from file.
		 *
		 * @param geometryShaderPath	The path of the geometry shader file.
		 *
		 * @returns true when created successfully, false otherwise.
		 */
		bool createGeometryShader(const std::string& geometryShaderPath);

		/**
		 * @brief Tries to create the shader program and link the shader to it.
		 *
		 * @param whichShaders	The <code>OR</code>ed lp::ShaderType set of shaders to link.
		 *
		 * @returns true when created and linked successfully, false otherwise.
		 *
		 * @sa lp::ShaderType
		 */
		bool createAndLinkProgram(unsigned char whichShaders);

		int getUniformLocation(const char* uniformName) const;

	private:
		GLuint mVertexShaderId;		/**< The handler for the vertex shader of this shader program.*/
		GLuint mFragmentShaderId;	/**< The handler for the fragment shader of this shader program.*/
		GLuint mGeometryShaderId;	/**< The handler for the geometry shader of this shader program.*/
		unsigned mProgramId;		/**< The handler for this shader program.*/
		std::string mName;

		mutable std::unordered_map<std::string, int> mUniformLocationCache;
	};
}
