#include <zr_pch.h>

#include "Zero/Log.h"
#include "OpenGLShader.h"
#include "GL_ERR_CHECK.h"

namespace zr
{
	OpenGLShader::OpenGLShader() :
		Shader(),
		mVertexShaderId(0U),
		mFragmentShaderId(0U),
		mGeometryShaderId(0U),
		mProgramId(0U)
	{
	}

	OpenGLShader::OpenGLShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) :
		Shader()
	{
		if (!createVertexAndFragShaders(vertexShaderPath, fragmentShaderPath)) {
			throw std::runtime_error("Can't load shader from file: ");
		}

		if (!createAndLinkProgram(VertexShader | FragmentShader)) {
			throw std::runtime_error("Can't load shader from file: ");
		}
	}

	OpenGLShader::OpenGLShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath) :
		Shader()
	{
		if (!createVertexAndFragShaders(vertexShaderPath, fragmentShaderPath)) {
			throw std::runtime_error("Can't load shader from file: " + vertexShaderPath + " OR " + fragmentShaderPath);
		}

		if (!createGeometryShader(geometryShaderPath)) {
			throw std::runtime_error("Can't load shader from file: " + geometryShaderPath);
		}

		if (!createAndLinkProgram(VertexShader | FragmentShader | GeometryShader)) {
			throw std::runtime_error("Can't load shader from file: ");
		}
	}

	OpenGLShader::~OpenGLShader()
	{
		if (mProgramId != 0U) {
			GL_ERR_CHECK(glDeleteProgram(mProgramId));
			mProgramId = 0U;
		}
	}

	bool OpenGLShader::loadFromFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		if (!createVertexAndFragShaders(vertexShaderPath, fragmentShaderPath)) {
			return false;
		}

		if (!createAndLinkProgram(VertexShader | FragmentShader)) {
			return false;
		}
		return true;
	}

	bool OpenGLShader::loadFromFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath)
	{
		if (!createVertexAndFragShaders(vertexShaderPath, fragmentShaderPath)) {
			return false;
		}

		if (!createGeometryShader(geometryShaderPath)) {
			return false;
		}

		if (!createAndLinkProgram(VertexShader | FragmentShader | GeometryShader)) {
			return false;
		}

		return true;
	}

	bool OpenGLShader::loadFromStrings(const std::string& vertexShader, const std::string& fragmentShader)
	{
		if (vertexShader.size() != 0 && fragmentShader.size() != 0) {
			const char* vertexShaderSource = vertexShader.c_str();
			// Compile vertex shader and check for errors
			GL_ERR_CHECK(mVertexShaderId = glCreateShader(GL_VERTEX_SHADER));
			GL_ERR_CHECK(glShaderSource(mVertexShaderId, 1, &vertexShaderSource, NULL));
			GL_ERR_CHECK(glCompileShader(mVertexShaderId));
			if (!checkCompilationError(VertexShader, mVertexShaderId)) {
				if (mVertexShaderId) {
					GL_ERR_CHECK(glDeleteShader(mVertexShaderId));
					mVertexShaderId = 0;
				}
				return false;
			}

			const char* fragmentShaderSource = fragmentShader.c_str();
			// Compile fragment shader and check for errors
			GL_ERR_CHECK(mFragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER));
			GL_ERR_CHECK(glShaderSource(mFragmentShaderId, 1, &fragmentShaderSource, NULL));
			GL_ERR_CHECK(glCompileShader(mFragmentShaderId));
			if (!checkCompilationError(FragmentShader, mFragmentShaderId)) {
				if (mFragmentShaderId) {
					GL_ERR_CHECK(glDeleteShader(mVertexShaderId));
					GL_ERR_CHECK(glDeleteShader(mFragmentShaderId));

					mVertexShaderId = 0;
					mFragmentShaderId = 0;
				}
				return false;
			}

			if (!createAndLinkProgram(VertexShader | FragmentShader)) {
				return false;
			}
		}
		return true;
	}

	bool OpenGLShader::loadFromStrings(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader)
	{
		if (vertexShader.size() != 0 && fragmentShader.size() != 0 && geometryShader.size() != 0) {
			const char* vertexShaderSource = vertexShader.c_str();
			// Compile vertex shader and check for errors
			GL_ERR_CHECK(mVertexShaderId = glCreateShader(GL_VERTEX_SHADER));
			GL_ERR_CHECK(glShaderSource(mVertexShaderId, 1, &vertexShaderSource, NULL));
			GL_ERR_CHECK(glCompileShader(mVertexShaderId));
			if (!checkCompilationError(VertexShader, mVertexShaderId)) {
				if (mVertexShaderId) {
					GL_ERR_CHECK(glDeleteShader(mVertexShaderId));
					mVertexShaderId = 0;
				}
				return false;
			}

			const char* fragmentShaderSource = fragmentShader.c_str();
			// Compile fragment shader and check for errors
			GL_ERR_CHECK(mFragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER));
			GL_ERR_CHECK(glShaderSource(mFragmentShaderId, 1, &fragmentShaderSource, NULL));
			GL_ERR_CHECK(glCompileShader(mFragmentShaderId));
			if (!checkCompilationError(FragmentShader, mFragmentShaderId)) {
				if (mFragmentShaderId) {
					GL_ERR_CHECK(glDeleteShader(mVertexShaderId));
					GL_ERR_CHECK(glDeleteShader(mFragmentShaderId));

					mVertexShaderId = 0;
					mFragmentShaderId = 0;
				}
				return false;
			}

			const char* geometryShaderSource = geometryShader.c_str();
			// Compile vertex shader and check for errors
			GL_ERR_CHECK(mGeometryShaderId = glCreateShader(GL_GEOMETRY_SHADER));
			GL_ERR_CHECK(glShaderSource(mGeometryShaderId, 1, &geometryShaderSource, NULL));
			GL_ERR_CHECK(glCompileShader(mGeometryShaderId));
			if (!checkCompilationError(GeometryShader, mGeometryShaderId)) {
				if (mGeometryShaderId) {
					GL_ERR_CHECK(glDeleteShader(mVertexShaderId));
					GL_ERR_CHECK(glDeleteShader(mFragmentShaderId));
					GL_ERR_CHECK(glDeleteShader(mGeometryShaderId));

					mVertexShaderId = 0;
					mFragmentShaderId = 0;
					mGeometryShaderId = 0;
				}
				return false;
			}

			if (!createAndLinkProgram(VertexShader | FragmentShader | GeometryShader)) {
				return false;
			}
		}
		return true;
	}

	void OpenGLShader::bind() const
	{
		GL_ERR_CHECK(glUseProgram(mProgramId));
	}

	void OpenGLShader::unbind() const
	{
		GL_ERR_CHECK(glUseProgram(0));
	}

	void OpenGLShader::setUniform(const char* uniformName, float value) const
	{
		int uniformLocation;
		GL_ERR_CHECK(uniformLocation = glGetUniformLocation(mProgramId, uniformName));
		GL_ERR_CHECK(glUniform1f(uniformLocation, value));
	}

	void OpenGLShader::setUniform(const char* uniformName, float valueX, float valueY) const
	{
		int uniformLocation;
		GL_ERR_CHECK(uniformLocation = glGetUniformLocation(mProgramId, uniformName));
		GL_ERR_CHECK(glUniform2f(uniformLocation, valueX, valueY));
	}

	void OpenGLShader::setUniform(const char* uniformName, float valueX, float valueY, float valueZ) const
	{
		int uniformLocation;
		GL_ERR_CHECK(uniformLocation = glGetUniformLocation(mProgramId, uniformName));
		GL_ERR_CHECK(glUniform3f(uniformLocation, valueX, valueY, valueZ));
	}

	void OpenGLShader::setUniform(const char* uniformName, float valueX, float valueY, float valueZ, float valueW) const
	{
		int uniformLocation;
		GL_ERR_CHECK(uniformLocation = glGetUniformLocation(mProgramId, uniformName));
		GL_ERR_CHECK(glUniform4f(uniformLocation, valueX, valueY, valueZ, valueW));
	}

	void OpenGLShader::setUniform(const char* uniformName, int value) const
	{
		int uniformLocation;
		GL_ERR_CHECK(uniformLocation = glGetUniformLocation(mProgramId, uniformName));
		GL_ERR_CHECK(glUniform1i(uniformLocation, value));
	}

	void OpenGLShader::setUniform(const char* uniformName, int valueX, int valueY) const
	{
		int uniformLocation;
		GL_ERR_CHECK(uniformLocation = glGetUniformLocation(mProgramId, uniformName));
		GL_ERR_CHECK(glUniform2i(uniformLocation, valueX, valueY));
	}

	void OpenGLShader::setUniform(const char* uniformName, int valueX, int valueY, int valueZ) const
	{
		int uniformLocation;
		GL_ERR_CHECK(uniformLocation = glGetUniformLocation(mProgramId, uniformName));
		GL_ERR_CHECK(glUniform3i(uniformLocation, valueX, valueY, valueZ));
	}

	void OpenGLShader::setUniform(const char* uniformName, int valueX, int valueY, int valueZ, int valueW) const
	{
		int uniformLocation;
		GL_ERR_CHECK(uniformLocation = glGetUniformLocation(mProgramId, uniformName));
		GL_ERR_CHECK(glUniform4i(uniformLocation, valueX, valueY, valueZ, valueW));
	}

	void OpenGLShader::setUniform(const char* uniformName, bool value) const
	{
		int uniformLocation;
		GL_ERR_CHECK(uniformLocation = glGetUniformLocation(mProgramId, uniformName));
		GL_ERR_CHECK(glUniform1i(uniformLocation, value));
	}

	void OpenGLShader::setUniform(const char* uniformName, bool valueX, bool valueY) const
	{
		int uniformLocation;
		GL_ERR_CHECK(uniformLocation = glGetUniformLocation(mProgramId, uniformName));
		GL_ERR_CHECK(glUniform2i(uniformLocation, valueX, valueY));
	}

	void OpenGLShader::setUniform(const char* uniformName, bool valueX, bool valueY, bool valueZ) const
	{
		int uniformLocation;
		GL_ERR_CHECK(uniformLocation = glGetUniformLocation(mProgramId, uniformName));
		GL_ERR_CHECK(glUniform3i(uniformLocation, valueX, valueY, valueZ));
	}

	void OpenGLShader::setUniform(const char* uniformName, bool valueX, bool valueY, bool valueZ, bool valueW) const
	{
		int uniformLocation;
		GL_ERR_CHECK(uniformLocation = glGetUniformLocation(mProgramId, uniformName));
		GL_ERR_CHECK(glUniform4i(uniformLocation, valueX, valueY, valueZ, valueW));
	}

	void OpenGLShader::setUniform(const char* uniformName, const glm::vec2& value) const
	{
		int uniformLocation;
		GL_ERR_CHECK(uniformLocation = glGetUniformLocation(mProgramId, uniformName));
		GL_ERR_CHECK(glUniform2f(uniformLocation, value.x, value.y));
	}

	void OpenGLShader::setUniform(const char* uniformName, const glm::vec3& value) const
	{
		int uniformLocation;
		GL_ERR_CHECK(uniformLocation = glGetUniformLocation(mProgramId, uniformName));
		GL_ERR_CHECK(glUniform3f(uniformLocation, value.x, value.y, value.z));
	}

	void OpenGLShader::setUniform(const char* uniformName, const glm::vec4& value) const
	{
		int uniformLocation;
		GL_ERR_CHECK(uniformLocation = glGetUniformLocation(mProgramId, uniformName));
		GL_ERR_CHECK(glUniform4f(uniformLocation, value.x, value.y, value.z, value.w));
	}

	void OpenGLShader::setUniform(const char* uniformName, const glm::mat3& value) const
	{
		int uniformLocation;
		GL_ERR_CHECK(uniformLocation = glGetUniformLocation(mProgramId, uniformName));
		GL_ERR_CHECK(glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value)));
	}

	void OpenGLShader::setUniform(const char* uniformName, const glm::mat4& value) const
	{
		int uniformLocation;
		GL_ERR_CHECK(uniformLocation = glGetUniformLocation(mProgramId, uniformName));
		GL_ERR_CHECK(glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value)));
	}

	void OpenGLShader::setUniform(const char* uniformName, const Material& value) const
	{
		std::string name(uniformName);
		setUniform((name + ".Ambient").c_str(), value.Ambient);
		setUniform((name + ".Diffuse").c_str(), value.Diffuse);
		setUniform((name + ".Emissive").c_str(), value.Emissive);
		setUniform((name + ".Specular").c_str(), value.Specular);
		setUniform((name + ".Shininess").c_str(), value.Shininess);
	}

	void OpenGLShader::setUniform(const char* uniformName, const Light& value) const
	{
		std::string name(uniformName);
		setUniform((name + ".Ambient").c_str(), value.Ambient);
		setUniform((name + ".Diffuse").c_str(), value.Diffuse);
		setUniform((name + ".Specular").c_str(), value.Specular);
		
		if (value.Type == Light::LightType::Directional) {
			DirectionalLight* light = (DirectionalLight*)&value;
			setUniform((name + ".Direction").c_str(), light->Direction);
			setUniform((name + ".Type").c_str(), Light::LightType::Directional);
		}
	}

	bool OpenGLShader::checkCompilationError(ShaderType shaderType, GLuint shaderId) const
	{
		int success;
		char info[5 * 1024];
		std::stringstream errorStream;
		GL_ERR_CHECK(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success));

		if (shaderType == VertexShader) {
			errorStream << "Vertex ";
		}
		else if (shaderType == FragmentShader) {
			errorStream << "Fragment ";
		}
		else if (shaderType == GeometryShader) {
			errorStream << "Geometry ";
		}

		if (!success) {
			GL_ERR_CHECK(glGetShaderInfoLog(shaderId, 5 * 1024, NULL, info));
			errorStream << "compilation failed:\n" << info;
			ZR_CORE_ERROR("[SHADER] {0}", errorStream.str());
			return false;
		}

		return true;
	}

	bool OpenGLShader::createVertexAndFragShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		std::ifstream vertexShaderFile;
		std::ifstream fragmentShaderFile;

		vertexShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
		fragmentShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

		try {
			vertexShaderFile.open(vertexShaderPath.c_str(), std::ios_base::in);
			fragmentShaderFile.open(fragmentShaderPath.c_str(), std::ios_base::in);

			std::stringstream vertexStringStream;
			std::stringstream fragmentStringStream;

			vertexStringStream << vertexShaderFile.rdbuf();
			vertexShaderFile.close();

			fragmentStringStream << fragmentShaderFile.rdbuf();
			fragmentShaderFile.close();

			std::string vertexShaderSource = vertexStringStream.str();
			std::string fragmentShaderSource = fragmentStringStream.str();

			const char* vertexShader = vertexShaderSource.c_str();
			const char* fragmentShader = fragmentShaderSource.c_str();

			// Compile vertex shader and check for errors
			GL_ERR_CHECK(mVertexShaderId = glCreateShader(GL_VERTEX_SHADER));
			GL_ERR_CHECK(glShaderSource(mVertexShaderId, 1, &vertexShader, NULL));
			GL_ERR_CHECK(glCompileShader(mVertexShaderId));
			if (!checkCompilationError(VertexShader, mVertexShaderId)) {
				if (mVertexShaderId) {
					GL_ERR_CHECK(glDeleteShader(mVertexShaderId));
					mVertexShaderId = 0;
				}
				return false;
			}

			// Compile fragment shader and check for errors
			GL_ERR_CHECK(mFragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER));
			GL_ERR_CHECK(glShaderSource(mFragmentShaderId, 1, &fragmentShader, NULL));
			GL_ERR_CHECK(glCompileShader(mFragmentShaderId));
			if (!checkCompilationError(FragmentShader, mFragmentShaderId)) {
				if (mFragmentShaderId) {
					GL_ERR_CHECK(glDeleteShader(mVertexShaderId));
					GL_ERR_CHECK(glDeleteShader(mFragmentShaderId));

					mVertexShaderId = 0;
					mFragmentShaderId = 0;
				}
				return false;
			}
		}
		catch (...) {
			return false;
		}
		return true;
	}

	bool OpenGLShader::createGeometryShader(const std::string& geometryShaderPath)
	{
		std::ifstream geometryShaderFile;

		geometryShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

		try {
			geometryShaderFile.open(geometryShaderPath.c_str(), std::ios_base::in);

			std::stringstream geometryStringStream;

			geometryStringStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();

			std::string geometryStringSource = geometryStringStream.str();

			const char* geometryShader = geometryStringSource.c_str();

			// Compile vertex shader and check for errors
			GL_ERR_CHECK(mGeometryShaderId = glCreateShader(GL_GEOMETRY_SHADER));
			GL_ERR_CHECK(glShaderSource(mGeometryShaderId, 1, &geometryShader, NULL));
			GL_ERR_CHECK(glCompileShader(mGeometryShaderId));
			if (!checkCompilationError(GeometryShader, mGeometryShaderId)) {
				if (mGeometryShaderId) {
					GL_ERR_CHECK(glDeleteShader(mVertexShaderId));
					GL_ERR_CHECK(glDeleteShader(mFragmentShaderId));
					GL_ERR_CHECK(glDeleteShader(mGeometryShaderId));

					mVertexShaderId = 0;
					mFragmentShaderId = 0;
					mGeometryShaderId = 0;
				}
				return false;
			}
		}
		catch (...) {
			return false;
		}

		return true;
	}

	bool OpenGLShader::createAndLinkProgram(unsigned char whichShaders)
	{
		if (whichShaders == 0) {
			// throw exception
			#ifdef DEBUG_TO_STDOUT
			std::cout << "Invalid shader" << std::endl;
			#endif

			return false;
		}

		// Attach shaders, link the program and check for errors
		int success;
		char info[1024];

		GL_ERR_CHECK(mProgramId = glCreateProgram());
		GL_ERR_CHECK(glAttachShader(mProgramId, mVertexShaderId));
		GL_ERR_CHECK(glAttachShader(mProgramId, mFragmentShaderId));
		if (whichShaders & GeometryShader) {
			GL_ERR_CHECK(glAttachShader(mProgramId, mGeometryShaderId));
		}

		GL_ERR_CHECK(glLinkProgram(mProgramId));
		GL_ERR_CHECK(glGetProgramiv(mProgramId, GL_LINK_STATUS, &success));

		if (!success) {
			GL_ERR_CHECK(glGetProgramInfoLog(mProgramId, 1024, NULL, info));
			#ifdef DEBUG_TO_STDOUT
			std::cout << "Program linking failed:\n" << info << std::endl;
			#endif

			GL_ERR_CHECK(glDeleteProgram(mProgramId));
			mProgramId = 0U;


			GL_ERR_CHECK(glDeleteShader(mVertexShaderId));
			GL_ERR_CHECK(glDeleteShader(mFragmentShaderId));
			mVertexShaderId = 0U;
			mFragmentShaderId = 0U;
			if (whichShaders & GeometryShader) {
				GL_ERR_CHECK(glDeleteShader(mGeometryShaderId));
				mGeometryShaderId = 0U;
			}
			return false;
		}

		// Always detach shaders after a successful link.
		GL_ERR_CHECK(glDetachShader(mProgramId, mVertexShaderId));
		GL_ERR_CHECK(glDetachShader(mProgramId, mFragmentShaderId));

		if (whichShaders & GeometryShader) {
			GL_ERR_CHECK(glDetachShader(mProgramId, mGeometryShaderId));
		}

		return true;
	}
}
