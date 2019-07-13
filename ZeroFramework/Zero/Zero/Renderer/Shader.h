#pragma once

#include "../../vendor/glm/include/glm/glm.hpp"

namespace zr
{
	/**
	 * @brief An enum defining the types of shaders.
	 */
	enum ShaderType : unsigned char
	{
		VertexShader =		1 << 0,		/**< Defines a vertex shader. */
		FragmentShader =	1 << 1,		/**< Defines a fragment shader. */
		GeometryShader =	1 << 2		/**< Defines a geometry shader. */
	};

	/**
	 * @brief lp::Shader represents a shader program for the OpenGL state machine.
	 */
	class Shader
	{
	public:
		/**
		 * @brief Default constructor.
		 */
		Shader();

		static Shader* create();

		/**
		 * @brief Constructor from file (vertex and fragment shaders).
		 *
		 * @exception std::runtime_error	When the shader program files can't be loaded from disk.
		 *
		 * @param vertexShaderPath  	The path of the vertex shader file.
		 * @param fragmentShaderPath	The path of the fragment shader file.
		 *
		 * ### todo Change the std::runtime_error to a custom error class.
		 */
		static Shader* create(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

		/**
		 * @brief Constructor from files (vertex, fragment and geometry shaders).
		 *
		 * @exception std::runtime_error	When the shader program files can't be loaded from disk.
		 *
		 * @param vertexShaderPath  	The path of the vertex shader file.
		 * @param fragmentShaderPath	The path of the fragment shader file.
		 * @param geometryShaderPath	The path of the geometry shader file.
		 *
		 * ### todo Change the std::runtime_error to a custom error class.
		 */
		static Shader* create(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath);

		/**
		 * @brief Default destructor.
		 *
		 * Deletes the shader program.
		 */
		~Shader();

		/**
		 * @brief Loads and creates the shader program from files (vertex and fragment shaders).
		 *
		 * @param vertexShaderPath  	The path of the vertex shader file.
		 * @param fragmentShaderPath	The path of the fragment shader file.
		 *
		 * @returns true when successfully loaded, false otherwise.
		 */
		virtual bool loadFromFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) = 0;

		/**
		 * @brief Loads and creates the shader program from files (vertex, fragment and geometry shaders).
		 *
		 * @param vertexShaderPath  	The path of the vertex shader file.
		 * @param fragmentShaderPath	The path of the fragment shader file.
		 * @param geometryShaderPath	The path of the geometry shader file.
		 *
		 * @returns true when successfully loaded, false otherwise.
		 */
		virtual bool loadFromFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath) = 0;

		/**
		 * @brief Loads and generates the shader program from strings (vertex and fragment shaders).
		 *
		 * @param vertexShader  	The string containing the vertex shader.
		 * @param fragmentShader	The string containing the fragment shader.
		 *
		 * @returns true when loaded successfully, false otherwise.
		 */
		virtual bool loadFromStrings(const std::string& vertexShader, const std::string& fragmentShader) = 0;

		/**
		 * @brief Loads and generates the shader program from strings (vertex, fragment and geometry
		 * shaders).
		 *
		 * @param vertexShader  	The string containing the vertex shader.
		 * @param fragmentShader	The string containing the fragment shader.
		 * @param geometryShader	The string containing the geometry shader.
		 *
		 * @returns true when loaded successfully, false otherwise.
		 */
		virtual bool loadFromStrings(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader) = 0;

		/**
		 * @brief Uses this program to perform operations in the OpenGL state machine.
		 *
		 * Changes the state of the OpenGL machine to receive data and perform activities with this
		 * program.
		 */
		virtual void bind() const = 0;

		/**
		 * @brief Unbinds the current bound shader program.
		 *
		 * Just binds the default (0) program.
		 */
		virtual void unbind() const = 0;

		/**
		 * @brief Sets a uniform in the shader program.
		 *
		 * This is a way of passing data to it. It passes one <code>float</code> to the shader program.
		 *
		 * @param uniformName	The name of the uniform which data will be changed.
		 * @param value		 	The value to pass.
		 */
		virtual void setUniform(const char* uniformName, float value) const = 0;

		/**
		 * @brief Sets a uniform in the shader program.
		 *
		 * This is a way of passing data to it. It passes two <code>float</code>s to the shader program.
		 *
		 * @param uniformName	The name of the uniform which data will be changed.
		 * @param valueX	 	The first (x) value of the vector.
		 * @param valueY	 	The second (y) value of the vector.
		 */
		virtual void setUniform(const char* uniformName, float valueX, float valueY) const = 0;

		/**
		 * @brief Sets a uniform in the shader program.
		 *
		 * This is a way of passing data to it. It passes three <code>float</code>s to the shader
		 * program.
		 *
		 * @param uniformName	The name of the uniform which data will be changed.
		 * @param valueX	 	The first (x) value of the vector.
		 * @param valueY	 	The second (y) value of the vector.
		 * @param valueZ	 	The third (z) value of the vector.
		 */
		virtual void setUniform(const char* uniformName, float valueX, float valueY, float valueZ) const = 0;

		/**
		 * @brief Sets a uniform in the shader program.
		 *
		 * This is a way of passing data to it. It passes four <code>float</code>s to the shader program.
		 *
		 * @param uniformName	The name of the uniform which data will be changed.
		 * @param valueX	 	The first (x) value of the vector.
		 * @param valueY	 	The second (y) value of the vector.
		 * @param valueZ	 	The third (z) value of the vector.
		 * @param valueW	 	The fourth (w) value of the vector.
		 */
		virtual void setUniform(const char* uniformName, float valueX, float valueY, float valueZ, float valueW) const = 0;

		/**
		 * @brief Sets a uniform in the shader program.
		 *
		 * This is a way of passing data to it. It passes one <code>int</code> to the shader program.
		 *
		 * @param uniformName	The name of the uniform which data will be changed.
		 * @param value		 	The value to pass.
		 */
		virtual void setUniform(const char* uniformName, int value) const = 0;

		/**
		 * @brief Sets a uniform in the shader program.
		 *
		 * This is a way of passing data to it. It passes two <code>int</code>s to the shader program.
		 *
		 * @param uniformName	The name of the uniform which data will be changed.
		 * @param valueX	 	The first (x) value of the vector.
		 * @param valueY	 	The second (y) value of the vector.
		 */
		virtual void setUniform(const char* uniformName, int valueX, int valueY) const = 0;

		/**
		 * @brief Sets a uniform in the shader program.
		 *
		 * This is a way of passing data to it. It passes three <code>int</code>s to the shader program.
		 *
		 * @param uniformName	The name of the uniform which data will be changed.
		 * @param valueX	 	The first (x) value of the vector.
		 * @param valueY	 	The second (y) value of the vector.
		 * @param valueZ	 	The third (z) value of the vector.
		 */
		virtual void setUniform(const char* uniformName, int valueX, int valueY, int valueZ) const = 0;

		/**
		 * @brief Sets a uniform in the shader program.
		 *
		 * This is a way of passing data to it. It passes four <code>int</code>s to the shader program.
		 *
		 * @param uniformName	The name of the uniform which data will be changed.
		 * @param valueX	 	The first (x) value of the vector.
		 * @param valueY	 	The second (y) value of the vector.
		 * @param valueZ	 	The third (z) value of the vector.
		 * @param valueW	 	The fourth (w) value of the vector.
		 */
		virtual void setUniform(const char* uniformName, int valueX, int valueY, int valueZ, int valueW) const = 0;

		/**
		 * @brief Sets a uniform in the shader program.
		 *
		 * This is a way of passing data to it. It passes one <code>bool</code> to the shader program.
		 *
		 * @param uniformName	The name of the uniform which data will be changed.
		 * @param valueX	 	The value to pass.
		 */
		virtual void setUniform(const char* uniformName, bool valueX) const = 0;

		/**
		 * @brief Sets a uniform in the shader program.
		 *
		 * This is a way of passing data to it. It passes two <code>bool</code>s to the shader program.
		 *
		 * @param uniformName	The name of the uniform which data will be changed.
		 * @param valueX	 	The first (x) value of the vector.
		 * @param valueY	 	The second (y) value of the vector.
		 */
		virtual void setUniform(const char* uniformName, bool valueX, bool valueY) const = 0;

		/**
		 * @brief Sets a uniform in the shader program.
		 *
		 * This is a way of passing data to it. It passes three <code>bool</code>s to the shader program.
		 *
		 * @param uniformName	The name of the uniform which data will be changed.
		 * @param valueX	 	The first (x) value of the vector.
		 * @param valueY	 	The second (y) value of the vector.
		 * @param valueZ	 	The third (z) value of the vector.
		 */
		virtual void setUniform(const char* uniformName, bool valueX, bool valueY, bool valueZ) const = 0;

		/**
		 * @brief Sets a uniform in the shader program.
		 *
		 * This is a way of passing data to it. It passes four <code>bool</code>s to the shader program.
		 *
		 * @param uniformName	The name of the uniform which data will be changed.
		 * @param valueX	 	The first (x) value of the vector.
		 * @param valueY	 	The second (y) value of the vector.
		 * @param valueZ	 	The third (z) value of the vector.
		 * @param valueW	 	The fourth (w) value of the vector.
		 */
		virtual void setUniform(const char* uniformName, bool valueX, bool valueY, bool valueZ, bool valueW) const = 0;

		/**
		 * @brief Sets a uniform in the shader program.
		 *
		 * This is a way of passing data to it. It passes a 3D <code>float</code> vector to the shader
		 * program.
		 *
		 * @param uniformName	The name of the uniform which data will be changed.
		 * @param value		 	The vector to pass to the shader.
		 */
		virtual void setUniform(const char* uniformName, const glm::vec3& value) const = 0;

		/**
		 * @brief Sets a uniform in the shader program.
		 *
		 * This is a way of passing data to it. It passes a 4D <code>float</code> vector to the shader
		 * program.
		 *
		 * @param uniformName	The name of the uniform which data will be changed.
		 * @param value		 	The vector to pass to the shader.
		 */
		virtual void setUniform(const char* uniformName, const glm::vec4& value) const = 0;

		/**
		 * @brief Sets a uniform in the shader program.
		 *
		 * This is a way of passing data to it. It passes a 3x3 <code>float</code> matrix to the shader
		 * program.
		 *
		 * @param uniformName	The name of the uniform which data will be changed.
		 * @param value		 	The matrix to pass to the shader.
		 */
		virtual void setUniform(const char* uniformName, const glm::mat3& value) const = 0;

		/**
		 * @brief Sets a uniform in the shader program.
		 *
		 * This is a way of passing data to it. It passes a 4x4 <code>float</code> matrix to the shader
		 * program.
		 *
		 * @param uniformName	The name of the uniform which data will be changed.
		 * @param value		 	The matrix to pass to the shader.
		 */
		virtual void setUniform(const char* uniformName, const glm::mat4& value) const = 0;
	};
}

/**
* @class lp::Shader
* @ingroup GraphicEngine
*
* lp::Shader is a representation for a shader program that can be loaded to the OpenGL state machine to
* achieve lighting effects, textre positioning, etc.
*
* It loads and sets the OpenGL shader program into the OpenGL machine and feeds it with the data needed.
*
*/
